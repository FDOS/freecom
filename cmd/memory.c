/* $Id$

	Internal command MEMORY to display the amount of memory used by
	the various internal data structures.

*/

#include "../config.h"

#include <alloc.h>

#include <environ.h>
#include <mcb.h>

#include "../include/context.h"
#include "../include/command.h"
#include "../strings.h"

static void displayTag(int string, Context_Tag tag)
{	ctxt_info_t *info;

	info = &CTXT_INFO_STRUCT(tag);

	displayString(string, info->c_sizemax, info->c_sizecur
	 , info->c_nummin < info->c_nummax
	 	? info->c_nummax - info->c_nummin : 0);
}
static void displayTag1(int string, Context_Tag tag)
{	ctxt_info_t *info;

	info = &CTXT_INFO_STRUCT(tag);

	displayString(string, info->c_sizecur
	 , info->c_nummin < info->c_nummax
	 	? info->c_nummax - info->c_nummin : 0);
}

int cmd_memory (char * param) {
        (void)param;
	displayString(TEXT_MEMORY_ENVIRONMENT
		, mcb_length(env_glbSeg), env_freeCount(env_glbSeg));
	displayString(TEXT_MEMORY_CONTEXT
		, mcb_length(ctxt), env_freeCount(ctxt));
	displayTag(TEXT_MEMORY_CTXT_ALIAS, CTXT_TAG_ALIAS);
	displayTag(TEXT_MEMORY_CTXT_HISTORY, CTXT_TAG_HISTORY);
	displayTag(TEXT_MEMORY_CTXT_DIRSTACK, CTXT_TAG_DIRSTACK);
	displayTag1(TEXT_MEMORY_CTXT_LASTDIR, CTXT_TAG_LASTDIR);
/*	displayTag1(TEXT_MEMORY_CTXT_BATCH, CTXT_TAG_BATCH);
	not used, yet -- 2001/06/11 ska*/
	displayTag1(TEXT_MEMORY_CTXT_SWAPINFO, CTXT_TAG_SWAPINFO);
	displayString(TEXT_MEMORY_HEAP, (unsigned long)coreleft());

	return 0;
}
