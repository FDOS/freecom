/* $Id$

	Kernel-supported swapping

	2001/01/21 ska
	started
*/

#include "../config.h"

#ifdef FEATURE_KERNEL_SWAP_SHELL

#include <assert.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

#include <suppl.h>
#include <environ.h>
#include <mcb.h>

#include "../include/context.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/kswap.h"

/* Lock kswap feature within kernel and invalidate a previous external prg
	Return:  FALSE  no swap feature within kernel */
int kswapInit(void)
{	struct REGPACK r;

	r.r_ax = 0x4bfe;		/* Get kswap argument structure segm */
	r.r_dx = 'FD';
	intr(0x21, &r);

	if((r.r_flags & 1) == 0) {
		dprintf(("[KSWAP: using kernel swapping support (KSSF eventually at %04x)]\n", r.r_ax));
		if(r.r_bx)	 {			/* segment found */
			kswapContext = (kswap_p)r.r_bx;
				/* invalidate external program if this shell
								aborts accidently */
			kswapContext->prg = 0;
			dprintf(("[KSWAP: static context found at 0x%04x]\n", r.r_bx));
			return TRUE;		/* re-invoked */
		}
		return FALSE;			/* not _re-_ invoked */
	} else if(r.r_ax == 5) {	/* Access denied -> there exists a
						static context with embedded Criter, but this
						copy of FreeCOM is NOT allowed to alter it */
		kswapContext = (kswap_p)r.r_bx;
		dprintf(("[KSWAP: static context found at 0x%04x]\n", r.r_bx));
	}

	dprintf(("[KSWAP: kernel swapping support is not used]\n"));
	gflag_swap = ERROR;		/* No swapping allowed */
	return FALSE;
}

static void kswapSetISR(void)
{
	*(void far* far*)MK_FP(_psp, 0xe) = kswapContext->cbreak_hdlr;
	/* The ^Break handler has been set already in INIT.C
		as it is an internal one (no part of the module) */
	*(void far* far*)MK_FP(_psp, 0x12) =
	 MK_FP(FP_SEG(kswapContext->cbreak_hdlr), kswapContext->ofs_criter);
	setvect(0x24, (void interrupt(*)())
	 MK_FP(FP_SEG(kswapContext->cbreak_hdlr), kswapContext->ofs_criter));
}

void kswapRegister(kswap_p ctxt)
{	struct REGPACK r;

	dprintf(("[KSWAP: Registering static context at: 0x%04x]\n", (word)ctxt));
	assert(ctxt);
	/* our own PSP gets patched in order to load the values of the
		Criter and ^Break handlers of the context on termination
		of this instance of FreeCOM.
		It is save, because this function is activated only, if
		the KSS is present, which faker has the previous values
		stored. */
	kswapSetISR();

	r.r_ax = 0x4bfd;	/* Set kswap argument structure segm */
	r.r_bx = (word)ctxt;
	r.r_dx = 'FD';
	intr(0x21, &r);
	if((r.r_flags & 1) != 0) {	/* failed */
		gflag_swap = ERROR;		/* cannot register -> cannot use */
		dprintf(("[KSWAP: Registering failed, kernel swap deactivated]\n"));
		return;
	}
	/* Initialize the constant values */
	/* environment block required only, if swapping is avail */
	ctxt->envSize = mcb_length(env_glbSeg);
	ctxt->envSegm = allocSysBlk(ctxt->envSize, 0x82);
	if(!ctxt->envSegm) {
		error_kswap_allocmem();
		gflag_swap = ERROR;
	}
	else
		dprintf(("[KSWAP: master environment allocated at 0x%04x]\n"
		 , ctxt->envSegm));
}
void kswapDeRegister(kswap_p ctxt)
{
	if(gflag_swap != ERROR) {	/* context belongs to this FreeCOM */
		dprintf(("[KSWAP: DeRegistering static context at: 0x%04x]\n", (word)ctxt));
		assert(ctxt);
		ctxt->shell = 0;		/* causes the kernel swap support to exit */
	}
}


/* Update the kswap argument block
	Return: 0 on error <-> no swapping possible
		else: segment of structure */
unsigned kswapMkStruc(const char * const prg, const char * const cmdline)
{
	word shellname;
	word segm;
	char *q, *h;

	assert(prg);
	assert(cmdline);

	if(gflag_swap == ERROR)	/* missing kernel support */
		return FALSE;

	assert(kswapContext);

	/* To update the static context is a good idea even if we don't
		swap after all */

	/* preserve the environment */
	assert(kswapContext->envSegm);
	assert(kswapContext->envSize);
	segm = kswapContext->envSegm;
	dprintf(("[KSWAP: Updating master environment at 0x%04x]\n", segm));
	assert(isMCB(SEG2MCB(segm)));
	assert(isMCB(SEG2MCB(env_glbSeg)));
	assert(mcb_length(env_glbSeg) <= mcb_length(segm));
	_fmemcpy(MK_FP(segm, 0), MK_FP(env_glbSeg, 0), mcb_length(env_glbSeg));

	/* Update the shell name as maybe %COMSPEC% was changed */
	/* COMSPEC is the central and traditionally the only place of the name of
		the shell */
	if(isSwapFile
	 || (shellname = env_findVar(segm, "COMSPEC") + 8) == (unsigned)-1 + 8) {
		ctxtSet(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_SHELLNAME, comResFile());
		if((kswapContext->shell	/* fetch first in case of failure */
		 = ctxtAddress(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_SHELLNAME)) == 0)
		   return FALSE;
	} else
		kswapContext->shell = MK_FP(segm, shellname);

/* Create the dynamic portion of the context */
	/* Construct command line string */
	if(*cmdline) {
		if((q = malloc(strlen(cmdline) + 4)) == 0) {
			error_out_of_memory();
			return FALSE;
		}
		h = stpcpy(q + 1, cmdline);
		*q = h - q - 1;			/* command line length */
		*h = '\r';				/* command line terminator */
		h[1] = 0;				/* ASCIIZ */
		ctxtSet(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_CMDLINE, q);
		myfree(q);
	} else
		ctxtSet(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_CMDLINE, "\1 \r");
	ctxtSet(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_PRGNAME, prg);
	kswapContext->dyn_ctxt = ctxtSegm;

	return (kswapContext->cmdline	/* fetch first in case of failure */
	   = ctxtAddress(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_CMDLINE)) != 0
	 && (kswapContext->prg
	   = ctxtAddress(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_PRGNAME)) != 0;

}

/* Restore the kswap argument block */
int kswapLoadStruc(void)
{
	kswapSetISR();
	if(gflag_swap == ERROR)	/* missing kernel support */
		return FALSE;

	assert(kswapContext);
	ctxtFlagsP = &kswapContext->flags;

	/* kswapContext->prg had been disabled in kswapInit() */
	assert(!kswapContext->prg);
	perform_exec_result(decode_exec_result(kswapContext->execErr));

	grabComFilename(1, kswapContext->shell);
	if((ctxtSegm = kswapContext->dyn_ctxt) == 0) {
		error_no_context_after_swap();
		ctxtCreate();
	}

	/* if the SHELL= statement specified the size of the environment, it
		must be applied each time FreeCOM is re-invoked, because DOS does
		not preserve the size of the original environment. */
	env_resizeCtrl = ENV_ALLOWMOVE | ENV_LASTFIT | ENV_USEUMB;
	/* if the size does not change this function performs no actions */
	env_setsize(0, kswapContext->envSize);

	return TRUE;
}

#endif
