/*
 *	Special FreeCOM declaration for the module management
 *
 * 2000/07/12 ska
 * started
 */

#ifndef MODULE_H
#define MODULE_H

#include "context.h"

	/* Retruns the pointer of the context pointer of the
		Critical Error handler */
extern context_t far* far* modCriter(void);

#endif
