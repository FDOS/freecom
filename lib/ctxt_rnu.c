/* $Id$

	Renumbers all items of the context tag

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <environ.h>

#include "../include/context.h"
#include "../include/command.h"

static int renumber(void *arg, word segm, word ofs)
{	char buf[CTXT_ITEMNAME_LENGTH];

#define tag (Context_Tag)arg

	assert(segm);
	assert(ofs != (word)-1);

	if(ctxtProbeItemTag(segm, ofs, arg)) {
		ctxtMkItemName(buf, tag, ++CTXT_INFO(tag, nummax));
		assert(peekb(segm , ofs + strlen(buf)) == '=');
		_fmemcpy(MK_FP(segm, ofs), TO_FP(buf), strlen(buf));
	}

#undef tag
	return 0;
}

void ctxtRenumberItems(const Context_Tag tag)
{
	ctxtCheckInfoTag(tag);
	assert(ctxtFromTag(tag) != CTXT_INVALID);

	CTXT_INFO(tag, nummin) = CTXT_INFO(tag, nummax) = 0;

	env_forAll(ctxtFromTag(tag), renumber, (void*)tag);
}
