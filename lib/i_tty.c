/* $id$

	Input functions

	Test if the current stdin stream is connected to the console.

*/

#include "../config.h"

#include <stdio.h>

#include <portable.h>

#include "in.h"
#include "../include/misc.h"

int iConsole(void)
{
	USEREGS

	_BX = fileno(inStream);
	_AX = 0x4400;		/* IOCTL: Get handle flags */
	geninterrupt(0x21);
	return !_CFLAG				/* success */
	 && (_DX & 0x81) == 0x81;	/* is a device & stdout */
}
