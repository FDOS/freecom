/* $id$

	Create a symbolic tagged item name

	Return: 0 on failure
		else: dynamically allocated name
*/

#include "../config.h"

#include <stdlib.h>
#include <string.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

char *ctxtMkSymName(const Context_Tag tag, const char * const tail)
{	char *name;

	assert(tail);

	if((name = emalloc(strlen(tail) + 2)) == 0)
		return 0;
	name[0] = tag;
	strcpy(&name[1], tail);
	return name;
}
