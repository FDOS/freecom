/* $Id$

	create an absolute number from the relative one

*/

#include "../config.h"

#include <assert.h>

#include "../include/context.h"
#include "../include/misc.h"

unsigned realNum(const Context_Tag tag, const int num)
{	ctxt_info_t *info;

	ctxtCheckInfoTag(tag);
	info = &CTXT_INFO_STRUCT(tag);

	if(num == 0)
		return info->c_nummax + 1;
	else if(num < 0) {
		int i = info->c_nummax + 1 + num;
		if(i > (int)info->c_nummin)
			return i;
		return (unsigned)-1;
	}
	return num;
}
