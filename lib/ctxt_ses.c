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
	int rc;

	if((name = emalloc(strlen(tail) + 2)) == 0)
		return E_NoMem;
	name[0] = tag;
	strcpy(&name[1], tail);
	rc = chgCtxt(tag, name, value);
	free(name);
	return rc;
}
