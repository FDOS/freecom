/* $Id$
 * Returns a pointer to the n'th parameter of the current batch file.
 * If no such parameter exists returns pointer to empty string.
 * If no batch file is current, returns NULL
 */

#include "../config.h"

#include <assert.h>
#include <limits.h>

#include "../include/context.h"
#include "../err_fcts.h"
#include "../include/misc.h"

char *find_arg(int n)
{	int num;
	char *buf;

	dprintf(("[find_arg(%u), shift=%u, base=%u]\n", n
	 , gflag_shiftlevel, gflag_base_shiftlevel));

	assert((unsigned long)gflag_shiftlevel + gflag_base_shiftlevel
	 < INT_MAX);

	num = gflag_shiftlevel + gflag_base_shiftlevel;
	/* Because a base == 0 means that there is absolutely no arg[] string
		registered so far, because the very first argument string always
		is 1 (one), return 0 to indicate this situation */
	if(!num)
		return (char*)0;

	assert(num <= CTXT_INFO(CTXT_TAG_ARG, nummax));

	if(n < 0 || (num += n) <= 0)
		return "";		/* Overflow <-> no argument definitely */

	switch(ctxtGet(0, CTXT_TAG_ARG, num, &buf)) {
	case 0:		/* OK */
		assert(buf);
		return regStr(buf);
	case 2:
		error_out_of_memory();
		break;
#ifdef DBEUG
	case 1: /* no such item */
		break;
	default:
		dprintf(("find_arg(): Invalid return code from ctxtGet()\n"));
#endif
	}
	return "";
}
