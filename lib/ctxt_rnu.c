/* $id$

	Renumbers all items of the context tag

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <environ.h>

#include "../include/context.h"
#include "../include/command.h"

static int renumber(void *arg, word segm, word ofs)
{	char buf[CTXT_LENGTH_ITEMNAME];
	char *p;

#define tag (Context_Tag)arg

	assert(segm);
	assert(ofs != (word)-1);
	p = ctxtP(segm, ofs);

	if(ctxtProbeItemTag(p, arg)) {
		ctxtMkItemName(buf, tag, ++CTXT_INFO(tag, nummax));
		assert(p[strlen(buf)] == '=');
			/* do not copy trailing '\0' */
		memcpy(p, buf, strlen(buf));
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
