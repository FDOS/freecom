/* $Id$

	Get the n'th item of the specified context tag.
	It is duplicated into dynamic memory.

	Returns:	0: OK
				1: no such item
				2: out of memory
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include "environ.h"
#include "fmemory.h"
#include "../include/context.h"
#include "../include/misc.h"

int ctxtGet (const int remove, const Context_Tag tag,
             const unsigned num, char ** const buf) {
        word ofs, segm;
	char name[CTXT_ITEMNAME_LENGTH];

	ctxtCheckInfoTag(tag);
	assert(ctxtFromTag(tag) != CTXT_INVALID);

	segm = (word)ctxtFromTag(tag);
	assert(segm);
	ctxtMkItemName(name, tag, num);
	if((ofs = env_findVar(segm, name)) != (word)-1) {
		assert(peekb(segm, ofs + strlen(name)) == '=');
		if(buf) {		/* Retreive the contents */
			if((*buf = _fdupstr(MK_FP(segm, ofs + strlen(name) + 1))) == 0)
				return 2;		/* out of memory */
		}
		if(remove) {	/* remove the item from context */
			if(CTXT_INFO(tag, sizecur) < env_varlen(segm, ofs))
				CTXT_INFO(tag, sizecur) = 0;	/* inconsistent redundant
														information */
			else
				CTXT_INFO(tag, sizecur) -= env_varlen(segm, ofs);
			env_subVarOffset(segm, ofs);
		}
		return 0;
	}

	return 1;		/* No such item */
}
