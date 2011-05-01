/* $Id$

	Implements an utility that creates the default target for
	the TOOLS sub-directory

	useage: MKTOOLS >makefile.mk

	$Log$
	Revision 1.3  2001/04/01 21:05:44  skaus
	bugfix: CALL doesn't reset options
	add: PTCHSIZE to patch heap size
	add: VSPAWN, /SWAP switch, .SWP resource handling

	Revision 1.2  2001/03/07 19:49:01  skaus
	Merged in Swap Support changes
	
	Revision 1.1.2.2  2001/02/18 21:19:26  skaus
	fix: MKTOOLS must include $(CFG) into list
	
	Revision 1.1.2.1  2001/02/18 18:03:51  skaus
	intermediate update #3
	
*/

#include <stdio.h>

#include "../config.h"

int main(void)
{
	fputs("all : $(CFG) ptchsize.exe", stdout);
#ifdef FEATURE_KERNEL_SWAP_SHELL
	fputs(" kssf.com vspawn.com", stdout);
#endif
#ifdef FEATURE_CALL_LOGGING
	fputs(" ptchldrv.exe", stdout);
#endif
#ifdef FEATURE_INSTALLABLE_COMMANDS
	fputs(" icmd", stdout);
#endif
	putchar('\n');

	return 0;
}
