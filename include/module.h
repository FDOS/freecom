/*
 *	Special FreeCOM declaration for the module management
 *
 * 2000/07/12 ska
 * started
 */

#ifndef MODULE_H
#define MODULE_H

#include "../include/kswap.h"

#ifdef FREECOM_NEED_MODULES

	/* Retruns the pointer of the context pointer of the
		Critical Error handler */
extern kswap_p modContext(void);

#endif

#endif
