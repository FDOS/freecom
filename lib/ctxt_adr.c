/* $id$

	Returns the far address to the value of a specified tag item

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <environ.h>

#include "../include/context.h"

char far*ctxtAddress(const Context_Tag tag, const unsigned num)
{	char name[CTXT_ITEMNAME_LENGTH];
	word ofs, segm;

	ctxtMkItemName(name, tag, num);
	segm = ctxtFromTag(tag);
	assert(segm && segm != CTXT_INVALID);
	if((ofs = env_findVar(segm, name)) != (word)-1)
		return MK_FP(segm, ofs + strlen(name) + 1);
	return 0;
}
