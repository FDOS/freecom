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

#if sizeof(void*)==2
#define getFree		(unsigned long)my_coreleft()
#else
#define getFree		(unsigned long)my_farcoreleft()
#endif


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
static void displayTag2(int string, Context_Tag tag)
{	ctxt_info_t *info;

	info = &CTXT_INFO_STRUCT(tag);

	displayString(string, info->c_sizecur);
}
static void displaySize(int string, unsigned size)
{
	displayString(string, size, (unsigned long)size << 4);
}


#pragma argsused
int cmd_memory(char *param)
{
	displayString(TEXT_MEMORY_ENVIRONMENT
		, mcb_length(env_glbSeg), env_freeCount(env_glbSeg));
	displayString(TEXT_MEMORY_CONTEXT
		, mcb_length(ctxtMain), env_freeCount(ctxtSegm));
	displayTag(TEXT_MEMORY_CTXT_ALIAS, CTXT_TAG_ALIAS);
	displayTag(TEXT_MEMORY_CTXT_HISTORY, CTXT_TAG_HISTORY);
	displayTag(TEXT_MEMORY_CTXT_DIRSTACK, CTXT_TAG_DIRSTACK);
	displayTag1(TEXT_MEMORY_CTXT_LASTDIR, CTXT_TAG_LASTDIR);
	displayTag1(TEXT_MEMORY_CTXT_ARG, CTXT_TAG_ARG);
	displayTag1(TEXT_MEMORY_CTXT_SWAPINFO, CTXT_TAG_SWAPINFO);
	displayTag2(TEXT_MEMORY_CTXT_FLAG, CTXT_TAG_FLAG);
	displayTag2(TEXT_MEMORY_CTXT_IVAR, CTXT_TAG_IVAR);
	displaySize(TEXT_MEMORY_CTXT_EXEC, ctxtECLenPar());
	displayString(TEXT_MEMORY_HEAP, (unsigned long)coreleft(), getFree);
	displaySize(TEXT_MEMORY_DOSMEM, DOSalloc(0, 0x80 | 0x10));

	return 0;
}
