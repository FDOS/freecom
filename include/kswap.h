/* Kernel-supported swapping */

#ifndef FEATURE_XMS_SWAP

#ifndef __KSWAP__H
#define __KSWAP__H

#include <portable.h>

#include "../context.h_c"

#define	FREECOM_NEED_MODULES

typedef  kswap_t _seg *kswap_p;
extern kswap_p kswapContext;		/* static context */

/* Lock kswap feature within kernel and invalidate a previous external prg
	Return:  TRUE if re-invoked */
int kswapInit(void);

/* (De-)Register the static context with the kernel
 */
void kswapRegister(kswap_p ctxt);
void kswapDeRegister(kswap_p ctxt);

/* Update the kswap argument block
	Return: 0 on error <-> no swapping possible
		else: segment of structure */
unsigned kswapMkStruc(const char * const prg, const char * const cmdline);

/* Restore the kswap argument block */
int kswapLoadStruc(void);

	/* Default alignment */
#pragma -a.

#endif

#endif
