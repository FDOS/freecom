/* $id$

	Changes a context tag item 

	Return: E_None on success
*/

#include "../config.h"

#include "../include/context.h"
#include "../include/ierror.h"

int ctxtSet(const Context_Tag tag
	, const unsigned num
	, const char * const value)
{	char name[CTXT_LENGTH_ITEMNAME];
	int rc;

	ctxtMkItemName(name, tag, num);
	if((rc = chgCtxt(tag, name, value)) == E_None) {
		/* Update min/max to reflect the added value */
		if(CTXT_INFO(tag, nummin) > num)
			CTXT_INFO(tag, nummin) = num;
		if(CTXT_INFO(tag, nummax) < num)
			CTXT_INFO(tag, nummax) = num;
	}
	return rc;
}
