/* $Id$

	Kernel-supported swapping

	2001/01/21 ska
	started
*/

#include "config.h" 

#ifdef FEATURE_KERNEL_SWAP_SHELL

#include <assert.h>
#include <dos.h>
#include <string.h>

#include <suppl.h>
#include <environ.h>
#include <mcb.h>

#include "alias.h"
#include "command.h"
#include "kswap.h"

#if 0 		// Install part of loading Criter
		if((segm = allocSysBlk(sizeof(kswap_t), 0x82)) != 0) {
			kswapContext = segm;
					/* invalidate segment */
			_fmemset(MK_FP(segm, 0), 0, sizeof(kswap_t));

			r.r_ax = 0x4bfd;	/* Set kswap argument structure segm */
			r.r_bx = segm;			/* none set */
			r.r_dx = 'FD';
			intr(0x21, &r);
			if((r.r_flags & 1) == 0)	/* success */
				return FALSE;			/* but not re-invoked */

			freeSysBlk(segm);
		}
#endif


/* Lock kswap feature within kernel and invalidate a previous external prg
	Return:  FALSE  no swap feature within kernel */
int kswapInit(void)
{	struct REGPACK r;

	r.r_ax = 0x4bfe;		/* Get kswap argument structure segm */
	r.r_dx = 'FD';
	intr(0x21, &r);

	if((r.r_flags & 1) == 0) {
		dprintf(("[KSWAP: using kernel swapping support]\n"));
		if(r.r_bx)	 {			/* segment found */
			kswapContext = (kswap_p)r.r_bx;
				/* invalidate external program if this shell
								aborts accidently */
			kswapContext->prg = 0;
			dprintf(("[KSWAP: static context found at 0x%04x]\n", r.r_bx));
			return TRUE;		/* re-invoked */
		}
#if 0
		r.r_ax = 0x4bfd;
		/* BX := 0; DX := 'FD' */
		intr(0x21, &r);		/* by calling the "Set Context"
								API function, it is locked if it gets
								called by accident by a secondary instance
								of FreeCOM */
#endif
		return FALSE;			/* not _re-_ invoked */
	} else if(r.r_ax == 5) {	/* Access denied -> there exists a
						static context with embedded Criter, but this
						copy of FreeCOM is NOT allowed to alter it */
		kswapContext = (kswap_p)r.r_bx;
		dprintf(("[KSWAP: static context found at 0x%04x]\n", r.r_bx));
	}

	dprintf(("[KSWAP: kernel swapping support is not used]\n"));
	swapOnExec = ERROR;		/* No swapping allowed */
	return FALSE;
}

void kswapRegister(kswap_p ctxt)
{	struct REGPACK r;

	dprintf(("[KSWAP: Registering static context at: 0x%04x]\n", (word)ctxt));
	assert(ctxt);
	/* our own PSP gets patched in order to load the values of the
		Criter and ^Break handlers of the context on termination
		of this instance of FreeCOM */
	*(void far* far*)MK_FP(_psp, 0xe) = kswapContext->cbreak_hdlr;
	*(void far* far*)MK_FP(_psp, 0x12) =
	 MK_FP(FP_SEG(kswapContext->cbreak_hdlr), kswapContext->ofs_criter);

	r.r_ax = 0x4bfd;	/* Set kswap argument structure segm */
	r.r_bx = (word)ctxt;
	r.r_dx = 'FD';
	intr(0x21, &r);
	if((r.r_flags & 1) != 0) {	/* failed */
		swapOnExec = ERROR;		/* cannot register -> cannot use */
		dprintf(("[KSWAP: Registering failed, kernel swap deactivated]\n"));
		return;
	}
	/* Initialize the constant values */
	/* environment block required only, if swapping is avail */
	ctxt->envSize = mcb_length(env_glbSeg);
	ctxt->envSegm = allocSysBlk(ctxt->envSize, 0x82);
	if(!ctxt->envSegm) {
		error_kswap_allocmem();
		swapOnExec = ERROR;
	}
}
void kswapDeRegister(kswap_p ctxt)
{
	if(swapOnExec != ERROR) {	/* context belongs to this FreeCOM */
		dprintf(("[KSWAP: DeRegistering static context at: 0x%04x]\n", (word)ctxt));
		assert(ctxt);
		ctxt->shell = 0;		/* causes the kernel swap support to exit */
	}
}


/* Update the kswap argument block
	Return: 0 on error <-> no swapping possible
		else: segment of structure */
unsigned kswapMkStruc(const char * const prg, const char * const cmdline)
{	unsigned size;		/* size of dynamic segment */
#ifdef FEATURE_ALIASES
	int al_size;		/* size of aliases */
#endif
	char far *p;
	word shellname;
	word segm;

	assert(prg);
	assert(cmdline);

	if(swapOnExec == ERROR)	/* missing kernel support */
		return FALSE;

	assert(kswapContext);

	/* To update the static context is a good idea even if we don't
		swap after all */

	/* preserve the environment */
	assert(kswapContext->envSegm);
	assert(kswapContext->envSize);
	assert(mcb_length(env_glbSeg) <= kswapContext->envSize);
	segm = kswapContext->envSegm;
	_fmemcpy(MK_FP(segm, 0), MK_FP(env_glbSeg, 0), mcb_length(env_glbSeg));

	/* Update the shell name as maybe %COMSPEC% was changed */
	/* COMSPEC is the central and traditionally the only place of the name of
		the shell */
	if((shellname = env_findVar(segm, "COMSPEC") + 8) == (unsigned)-1 + 8) {
		/* set the string #0 */
		char *shell;

		shell = comFile();
		assert(shell);

		env_strput(segm, shell, 0);
		shellname = env_string(segm, 0);
	}
	kswapContext->shell = MK_FP(segm, shellname);

	/* Update central settings of FreeCOM */
	kswapContext->canexit = canexit;

	/* Create the dynamic portion of the context */
	assert((unsigned long)strlen(prg) + (unsigned long)strlen(cmdline)
	 < 32767);

	size = strlen(prg) + strlen(cmdline) + 2;	/* two NUL terminators */
#ifdef FEATURE_ALIASES
	if((al_size = alias_streamsize()) <= 0) {
		error_kswap_alias_size();
		return FALSE;
	}
	size += al_size;
#else
	++size;			/* robustness <-> fake an empty alias entry */
#endif

	if((segm = allocSysBlk(size, 0x82)) == 0) {
		error_kswap_allocmem();
		return FALSE;
	}
	dprintf(("[KSWAP: dynamic context allocated at 0x%04x/%u]\n", segm, size));

	kswapContext->dyn_ctxt = segm;
	p = _fstpcpy(kswapContext->prg = MK_FP(segm, 0), prg);
	p = _fstpcpy(kswapContext->cmdline = p + 1, cmdline);
#ifdef FEATURE_ALIASES
	alias_streamto(p + 1);
#else
	p[1] = '\0';	/* robustness <-> eof-of-aliases */
#endif

	return TRUE;
}

/* Restore the kswap argument block */
int kswapLoadStruc(void)
{
	char far *p;

	if(swapOnExec == ERROR)	/* missing kernel support */
		return FALSE;

	assert(kswapContext);

	/* kswapContext->prg had been disabled in kswapInit() */
	assert(!kswapContext->prg);
	perform_exec_result(decode_exec_result(kswapContext->execErr));

	canexit = kswapContext->canexit;
	grabComFilename(1, kswapContext->shell);
	if(kswapContext->dyn_ctxt) {
#ifdef FEATURE_ALIASES
			/* Skip prg name */
		p = _fstrchr(MK_FP(kswapContext->dyn_ctxt, 0), 0);
			/* Skip cmdline */
		p = _fstrchr(p + 1, 0);
		alias_streamfrom(p + 1);
#endif
		freeSysBlk(kswapContext->dyn_ctxt);
		dprintf(("[KSWAP: dynamic context deallocated at 0x%04x]\n", kswapContext->dyn_ctxt));
		kswapContext->dyn_ctxt = 0;
	}

	/* if the SHELL= statement specified the size of the environment, it
		must be applied each time FreeCOM is re-invoked, because DOS does
		not preserve the size of the original environment. */
	env_resizeCtrl |= ENV_USEUMB | ENV_ALLOWMOVE;
	/* if the size does not change this function performs no actions */
	env_setsize(0, kswapContext->envSize);

	return TRUE;
}

#endif
