/*
 *	Special FreeCOM declaration for the context management
 *
 *	A context specifies the data used to communicate between FreeCOM
 *	and modules, such as the Critical Error handler.
 *
 * 2000/07/12 ska
 * started
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "portable.h"

#if 0
typedef unsigned char byte;			/* exactly 8bit unsigned */
typedef unsigned short word;		/* exactly 16bit unsigned */
#endif

#include "context.h_c"

/****
 **** Easy to use macros
 ****/
#define autofail context.int24.autoFail

extern context_t context;

#ifndef LINT
#if sizeof(byte) != 1
	/* Well, per ANSI this never can happen */
#error "BYTE must be typedef'ed to a 8bit type"
#endif
#if sizeof(word) != 2
#error "WORD must be typedef'ed to a 16bit type"
#endif
#endif

#endif
