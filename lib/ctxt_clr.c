/* $Id$

	Clear all items of a specified context tag

*/

#include "../config.h"

#include <assert.h>
#include <environ.h>

#include "../include/context.h"
#include "../include/misc.h"

static int clear(void *arg, word segm, word ofs)
{
	assert(segm);
	assert(ofs != 0xffff);

	while(ctxtProbeItemTag(segm, ofs, arg))
	 	env_subVarOffset(segm, ofs);

	return 0;		/* proceed loop */
}

int ctxtClear(const Context_Tag tag)
{	ctxt_info_t *info;

	ctxtCheckInfoTag(tag);
	assert(ctxtFromTag(tag) != CTXT_INVALID);

	/* Clear the useage information */
	info = &CTXT_INFO_STRUCT(tag);
	info->c_sizecur = 0;
	info->c_nummax = info->c_nummin= 0;

	/* Clear the items within the context */
	return env_forAll(ctxtFromTag(tag), clear, (void*)tag);
}
