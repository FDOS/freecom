/* $Id$

	Pops the last added entry of the specified context tag.
	It is duplicated into dynamic memory.

	If "buf" == NULL, the entry is simply discarded.

	Returns:	true on success
		else displays any appropriate error message except "empty"
*/

#include "../config.h"

#include <assert.h>

#include <environ.h>

#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"

int ctxtPop(const Context_Tag tag, char ** const buf)
{	ctxt_info_t *info;

	ctxtCheckInfoTag(tag);

	info = &CTXT_INFO_STRUCT(tag);
	while(info->c_nummax > info->c_nummin)
		switch(ctxtGet(1, tag, info->c_nummax--, buf)) {
		case 0:			/* OK */
			return 1;
		case 1:			/* no such entry */
			break;		/* --> ignore silently */
		case 2:			/* out of memory */
			return 0;	/* will cause "context empty" message */
		}

	/* Nothing in the stack ==> make sure it's consistent */
	ctxtClear(tag);
	return 0;
}
