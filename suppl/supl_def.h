#ifndef __SUPPL_COMPILER_DEFINES
#define __SUPPL_COMPILER_DEFINES

/**********************************
 ***** Pacific HiTech C
 **********************************/
#ifdef HI_TECH_C
	/* Probe if PAC is used in conjunction with CLib */
#include <stddef.h>

#ifdef _CLIB_
#define _PAC_	1
#else
#define _PAC_NOCLIB_	1
#endif	/* _CLIB_ */

#endif /* HI_TECH_C */


/**********************************
 ***** Early Turbo C++
 **********************************/
#ifdef __TURBOC__
#if __TURBOC__ <= 0x297
#define _TC_EARLY_
#else
#define _TC_LATER_
#endif
#endif

#endif
