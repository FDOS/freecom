/* $Id$

	Implements an utility that creates the default target for
	the TOOLS sub-directory

	useage: MKTOOLS >makefile.mk

	$Log$
	Revision 1.1.2.1  2001/02/18 18:03:51  skaus
	intermediate update #3

*/

#include <stdio.h>

#include "../config.h"

main(void)
{
	fputs("all :", stdout);
#ifdef FEATURE_KERNEL_SWAP_SHELL
	fputs(" kssf.com", stdout);
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
