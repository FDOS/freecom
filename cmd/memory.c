/* $Id$

	Internal command MEMORY to display the amount of memory used by
	the various internal data structures.

*/

#include "../config.h"

#include <alloc.h>

#include <environ.h>
#include <mcb.h>
#include <suppl.h>

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


#pragma argsused
int cmd_memory(char *param)
{	unsigned size;

	displayString(TEXT_MEMORY_ENVIRONMENT
		, mcb_length(env_glbSeg), env_freeCount(env_glbSeg));
	displayString(TEXT_MEMORY_CONTEXT
		, mcb_length(ctxtMain), env_freeCount(ctxtSegm));
	displayTag(TEXT_MEMORY_CTXT_ALIAS, CTXT_TAG_ALIAS);
	displayTag(TEXT_MEMORY_CTXT_HISTORY, CTXT_TAG_HISTORY);
	displayTag(TEXT_MEMORY_CTXT_DIRSTACK, CTXT_TAG_DIRSTACK);
	displayTag1(TEXT_MEMORY_CTXT_LASTDIR, CTXT_TAG_LASTDIR);
	displayTag1(TEXT_MEMORY_CTXT_FLAG, CTXT_TAG_FLAG);
	displayTag1(TEXT_MEMORY_CTXT_ARG, CTXT_TAG_ARG);
	displayTag1(TEXT_MEMORY_CTXT_IVAR, CTXT_TAG_IVAR);
	displayTag1(TEXT_MEMORY_CTXT_SWAPINFO, CTXT_TAG_SWAPINFO);
	displayString(TEXT_MEMORY_HEAP, (unsigned long)coreleft()
	 , (unsigned long)
#if sizeof(void*)==2
		my_coreleft()
#else
		my_farcoreleft()
#endif
	 );
	size = DOSalloc(0, 0x80 | 0x10);
	displayString(TEXT_MEMORY_DOSMEM, size, (unsigned long)size << 4);

	return 0;
}
