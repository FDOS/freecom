/* $id$

	Enumerate all items of a stack tag beginning with nummax (or startID)
	downto nummin

	Return:
		0: The callback function always returned 0
		-1: out of memory (error message already issued)
		else: value returned from callback function
*/

#include "../config.h"

#include "../include/context.h"
#include "../err_fcts.h"

int ecEnum(const Context_Tag tag, const unsigned startID, int (*fct)(unsigned id, char *buf, void * const arg), void * const arg)
{	unsigned id;
	int rv;
	char *buf;

	ctxtCheckInfoTag(tag);
	assert(fct);

	id = startID;
	if(!id || id > CTXT_INFO(tag, nummax))
		id = CTXT_INFO(tag, nummax);

	for(rv = 0; !rv && id && id >= CTXT_INFO(tag, nummin); --id) {
		chkHeap
		switch(ctxtGet(0, tag, id, &buf)) {
		case 0:	/* OK */
			rv = fct(id, buf, arg);
			myfree(buf);
			break;
		case 2:	/* out of memory */
			error_out_of_memory();
			return -1;
#ifdef DEBUG
		case 1:	/* no such item */
			dprintf(("[CTXT: Hole in %u #%u]\n", tag, id));
			break;
		default:
			dprintf(("[CTXT: Invalid rv in ctxtEnum]\n"));
			return -1;
#endif
		}
		chkHeap
	}
	return rv;
}
