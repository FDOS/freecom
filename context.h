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

typedef unsigned char BYTE;			/* exactly 8bit unsigned */
typedef unsigned short WORD;		/* exactly 16bit unsigned */

	/* one-byte alignment */
#pragma -a-

typedef struct {		/* type of a control area */
	WORD ctxt_autofail;
} context_t;
	/* standard alignment */
#pragma -a.

/****
 **** Easy to use macros
 ****/
#define autofail context.ctxt_autofail

extern context_t context;

#if sizeof(BYTE) != 1
	/* Well, per ANSI this never can happen */
#error "BYTE must be typedef'ed to a 8bit type"
#endif
#if sizeof(WORD) != 2
#error "WORD must be typedef'ed to a 16bit type"
#endif

#endif
