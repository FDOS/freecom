/* $id$

	Return if the given name is an alias

*/

#include "../config.h"

#include <assert.h>

#include <environ.h>

#include "../include/command.h"
#include "../include/context.h"

int is_alias(const char * const name)
{	assert(name);
	return env_findVar(ctxtAlias, name) != (unsigned)-1;
}
