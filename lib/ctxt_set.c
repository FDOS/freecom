/* $Id$

	Changes a context tag item 

	Return: E_None on success
*/

#include "../config.h"

#include "../include/context.h"

int ctxtSet(const Context_Tag tag
	, const unsigned num
	, const char * const value)
{	char name[CTXT_ITEMNAME_LENGTH];
	int rc;

	ctxtMkItemName(name, tag, num);
	if((rc = chgCtxt(tag, name, value)) == 0) {
		/* Update min/max to reflect the added value */
		if(CTXT_INFO(tag, nummin) > num)
			CTXT_INFO(tag, nummin) = num;
		if(CTXT_INFO(tag, nummax) < num)
			CTXT_INFO(tag, nummax) = num;
	}
	return rc;
}
