/* $id$

	Output functions

	Test if the current stdout stream is connected to the console.

*/

#include "../config.h"

#include <stdio.h>

#include <portable.h>

#include "out.h"
#include "../include/misc.h"

int oConsole(void)
{
	USEREGS

	_BX = fileno(outStream);
	_AX = 0x4400;		/* IOCTL: Get handle flags */
	geninterrupt(0x21);
	return !_CFLAG				/* success */
	 && (_DX & 0x82) == 0x82;	/* is a device & stdout */
}
