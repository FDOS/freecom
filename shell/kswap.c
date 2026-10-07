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

#define FD_MAGIC 0x4446 /* 'FD' */

/* Lock kswap feature within kernel and invalidate a previous external prg
    Return:  FALSE  no swap feature within kernel */
int kswapInit(void)
{   IREGS r;

    r.r_ax = 0x4bfe;        /* Get kswap argument structure segm */
    r.r_dx = FD_MAGIC;
    intrpt(0x21, &r);

    if(!( r.r_flags & 1 )) {
        dprintf(("[KSWAP: using kernel swapping support (KSSF eventually at %04x)]\n", r.r_ax));
        if(r.r_bx)   {          /* segment found */
            kswapContext = (kswap_p)MK_SEG_PTR(kswap_t, r.r_bx);
                /* invalidate external program if this shell
                                aborts accidently */
            kswapContext->prg = 0;
            dprintf(("[KSWAP: static context found at 0x%04x]\n", r.r_bx));
            return TRUE;        /* re-invoked */
        }
        return FALSE;           /* not _re-_ invoked */
    } else if(r.r_ax == 5) {    /* Access denied -> there exists a
                        static context with embedded Criter, but this
                        copy of FreeCOM is NOT allowed to alter it */
        kswapContext = (kswap_p)MK_SEG_PTR(kswap_t, r.r_bx);
        dprintf(("[KSWAP: static context found at 0x%04x]\n", r.r_bx));
    }

    dprintf(("[KSWAP: kernel swapping support is not used]\n"));
    swapOnExec = ERROR;     /* No swapping allowed */
    return FALSE;
}

static void kswapSetISR(void)
{
    *(void far* far*)MK_FP(_psp, 0xe) = kswapContext->cbreak_hdlr;
    /* The ^Break handler has been set already in INIT.C
        as it is an internal one (no part of the module) */
    *(void far* far*)MK_FP(_psp, 0x12) =
     MK_FP(FP_SEG(kswapContext->cbreak_hdlr), kswapContext->ofs_criter);
    set_isrfct(0x24,
     MK_FP(FP_SEG(kswapContext->cbreak_hdlr), kswapContext->ofs_criter));
}

void kswapRegister(kswap_p ctxt)
{   IREGS r;

    dprintf(("[KSWAP: Registering static context at: 0x%04x]\n", FP_SEG((void far *)ctxt)));
    assert(ctxt);
    /* our own PSP gets patched in order to load the values of the
        Criter and ^Break handlers of the context on termination
        of this instance of FreeCOM.
        It is save, because this function is activated only, if
        the KSS is present, which faker has the previous values
        stored. */
    kswapSetISR();

    r.r_ax = 0x4bfd;    /* Set kswap argument structure segm */
    r.r_bx = FP_SEG((void far *)ctxt);
    r.r_dx = FD_MAGIC;
    intrpt(0x21, &r);
    if(r.r_flags & 1) { /* failed */
        swapOnExec = ERROR;     /* cannot register -> cannot use */
        dprintf(("[KSWAP: Registering failed, kernel swap deactivated]\n"));
        return;
    }
    /* Allocate the saved environment at swap time: initialization and SET
       may still change its size after this registration. */
    ctxt->envSize = 0;
    ctxt->envSegm = 0;
}
void kswapDeRegister(kswap_p ctxt)
{
    if(swapOnExec != ERROR) {   /* context belongs to this FreeCOM */
        dprintf(("[KSWAP: DeRegistering static context at: 0x%04x]\n", FP_SEG((void far *)ctxt)));
        assert(ctxt);
        ctxt->shell = 0;        /* causes the kernel swap support to exit */
    }
}


/* Context variables are strings. Encode the counters in hex, after reserving
   the entry, so that the saved counters include the entry's own allocation.
   This avoids reconstructing history/dirstack numbering from their contents. */
#define KSWAP_INFO_BYTES (sizeof(ctxt_info_t) * (CTXT_TAG_ALIAS - CTXT_FIRST_TAG + 1))
#define KSWAP_STATUS_BYTES (KSWAP_INFO_BYTES + 1)

static int kswapSaveStatus(void)
{
    static const char hex[] = "0123456789abcdef";
    char reserve[2 * KSWAP_STATUS_BYTES + 1];
    unsigned i, value;
    char far *p;

    memset(reserve, '0', sizeof(reserve) - 1);
    reserve[sizeof(reserve) - 1] = 0;
    if(ctxtSet(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_STATUS, reserve))
        return FALSE;
    p = ctxtAddress(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_STATUS);
    if(!p)
        return FALSE;
    for(i = 0; i < KSWAP_STATUS_BYTES; ++i) {
        value = i < KSWAP_INFO_BYTES ? ((unsigned char *)ctxt_info)[i]
                                    : (forceLow != 0);
        *p++ = hex[value >> 4];
        *p++ = hex[value & 15];
    }
    return TRUE;
}

static int kswapRestoreStatus(void)
{
    unsigned char saved[KSWAP_STATUS_BYTES];
    unsigned i, value;
    char c;
    char far *p = ctxtAddress(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_STATUS);

    if(!p)
        return FALSE;
    for(i = 0; i < 2 * KSWAP_STATUS_BYTES; ++i) {
        c = *p++;
        if(c >= '0' && c <= '9')
            value = c - '0';
        else if(c >= 'a' && c <= 'f')
            value = c - 'a' + 10;
        else
            return FALSE;
        if(!(i & 1))
            saved[i / 2] = value << 4;
        else
            saved[i / 2] |= value;
    }
    if(*p || saved[KSWAP_INFO_BYTES] > 1)
        return FALSE;
    memcpy(ctxt_info, saved, KSWAP_INFO_BYTES);
    forceLow = saved[KSWAP_INFO_BYTES];
    return TRUE;
}

/* Update the kswap argument block
    Return: 0 on error <-> no swapping possible
        else: segment of structure */
unsigned kswapMkStruc(const char * const prg, const char * const cmdline)
{
    word shellname;
    word segm, envSize;
    int shellInContext;
    struct MCB _seg *mcb;
    char *q, *h;

    assert(prg);
    assert(cmdline);

    if(swapOnExec == ERROR) /* missing kernel support */
        return FALSE;

    assert(kswapContext);

    /* To update the static context is a good idea even if we don't
        swap after all */

    /* preserve the environment */
    envSize = env_glbSeg ? mcb_length(env_glbSeg) : 0;
    if(!envSize)
        return FALSE;
    segm = kswapContext->envSegm;
    if(!segm || mcb_length(segm) < envSize) {
        word replacement = allocSysBlk(envSize, forceLow ? 0x02 : 0x82);
        if(!replacement) {
            static const char message[] =
                "KSWAP: Not enough memory to save the environment; not swapping.\r\n";
            /* Loading STRINGS can itself run out of memory and prompt for
               another resource file. Also avoid CRT stdio: release FreeCOM
               uses handle-valued FILE pointers, not CRT FILE structures. */
            dos_write(fileno(stderr), message, sizeof(message) - 1);
            return FALSE;       /* Execute normally; never overrun the backup. */
        }
        if(segm)
            freeSysBlk(segm);
        segm = kswapContext->envSegm = replacement;
    }
    kswapContext->envSize = envSize;
    dprintf(("[KSWAP: Updating master environment at 0x%04x]\n", segm));
    assert(isMCB(SEG2MCB(segm)));
    assert(isMCB(SEG2MCB(env_glbSeg)));
    assert(mcb_length(env_glbSeg) <= mcb_length(segm));
    _fmemcpy(MK_FP(segm, 0), MK_FP(env_glbSeg, 0), envSize);

    /* Update the shell name as maybe %COMSPEC% was changed */
    /* COMSPEC is the central and traditionally the only place of the name of
        the shell */
    shellInContext = isSwapFile
     || (shellname = env_findVar(segm, "COMSPEC") + 8) == (unsigned)-1 + 8;
    if(shellInContext) {
        char *p = comResFile();
        ctxtSet(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_SHELLNAME, p);
        free(p);
        if((kswapContext->shell /* fetch first in case of failure */
         = ctxtAddress(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_SHELLNAME)) == 0)
           return FALSE;
    } else
        kswapContext->shell = MK_FP(segm, shellname);

    /* Update central settings of FreeCOM */
    kswapContext->canexit = canexit;
    kswapContext->dfltSwap = defaultToSwap;
#ifdef NDEBUG
    kswapContext->debug = (word)stderr;
#else
    kswapContext->debug = fddebug;
#endif

/* Create the dynamic portion of the context */
    /* Construct command line string */
    if(*cmdline) {
        if((q = malloc(strlen(cmdline) + 4)) == 0) {
            error_out_of_memory();
            return FALSE;
        }
        h = stpcpy(q + 1, cmdline);
        *q = h - q - 1;         /* command line length */
        *h = '\r';              /* command line terminator */
        h[1] = 0;               /* ASCIIZ */
        ctxtSet(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_CMDLINE, q);
        free(q);
    } else
        ctxtSet(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_CMDLINE, "\1 \r");
    ctxtSet(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_PRGNAME, prg);

    if(!kswapSaveStatus())
        return FALSE;
    /* Adding entries can relocate ctxt; take all pointers only afterwards. */
    if(shellInContext)
        kswapContext->shell = ctxtAddress(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_SHELLNAME);
    kswapContext->cmdline = ctxtAddress(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_CMDLINE);
    if(!kswapContext->shell || !kswapContext->cmdline)
        return FALSE;
    kswapContext->prg = ctxtAddress(CTXT_TAG_SWAPINFO, CTXT_SWAPINFO_PRGNAME);
    if(!kswapContext->prg)
        return FALSE;

    /* The child and the reloaded shell need these strings and counters.
       DOS must not release the context when this shell exits to KSSF. */
    mcb = MK_SEG_PTR(struct MCB, SEG2MCB(ctxt));
    mcb->mcb_ownerPSP = 8;
    kswapContext->dyn_ctxt = ctxt;
    return TRUE;

}

/* Restore the kswap argument block */
int kswapLoadStruc(void)
{
    kswapSetISR();
    if(swapOnExec == ERROR) /* missing kernel support */
        return FALSE;

    assert(kswapContext);

    /* kswapContext->prg had been disabled in kswapInit() */
    assert(!kswapContext->prg);
    canexit = kswapContext->canexit;
    defaultToSwap = kswapContext->dfltSwap;
#ifndef NDEBUG
    fddebug = kswapContext->debug;
#endif
    grabComFilename(1, kswapContext->shell);
    ctxt = kswapContext->dyn_ctxt;
    kswapContext->dyn_ctxt = 0;
    if(ctxt) {
        struct MCB _seg *mcb = MK_SEG_PTR(struct MCB, SEG2MCB(ctxt));
        /* While the shell runs, ordinary context relocation and DOS exit
           own this allocation again. KSSF owns it only across the swap. */
        mcb->mcb_ownerPSP = _psp;
        if(!kswapRestoreStatus()) {
            freeBlk(ctxt);
            ctxt = 0;
        }
    }
    if(!ctxt) {
        error_no_context_after_swap();
        ctxtCreate();
    }

    /* if the SHELL= statement specified the size of the environment, it
        must be applied each time FreeCOM is re-invoked, because DOS does
        not preserve the size of the original environment. */
    env_resizeCtrl = ENV_ALLOWMOVE | ENV_LASTFIT | ENV_USEUMB;
  if(forceLow)
      env_resizeCtrl &= ~ENV_USEUMB;
    /* if the size does not change this function performs no actions */
    env_setsize(0, kswapContext->envSize);

    /* Message resources and context must be ready before an EXEC error is
       diagnosed. A successful child's nonzero AL is not a DOS-4B error. */
    if(kswapContext->execErr)
        setErrorLevel(kswapContext->execErr);
    else
        setChildStatus(kswapContext->childStatus);

    return TRUE;
}

#endif
