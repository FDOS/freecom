/* $id$

	Changes a context tag item

	Return: E_None on success
*/

#include "../config.h"

#include <stdlib.h>
#include <string.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

int ctxtSetS(const Context_Tag tag
	, const char * const tail
	, const char * const value)
{	char *name;

	assert(tail);
	if((name = regStr(ctxtMkSymName(tag, tail))) == 0)
		return E_NoMem;
	assert((byte)name[0] == (byte)tag);

	return chgCtxt(tag, name, value);
}
