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

	for(rv = 0; !rv && id > CTXT_INFO(tag, nummin); --id) {
		chkHeap
		if((buf = ctxtAddress(tag, id)) != 0)
			rv = fct(id, buf, arg);
#ifdef DEBUG
		else {
			dprintf(("[CTXT: Hole in %u #%u]\n", tag, id));
		}
#endif
		chkHeap
	}
	return rv;
}
