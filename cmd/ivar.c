/* $Id$

	Internal command IVAR

	IVAR name [ '=' [ value ] ]

 */

#include "../config.h"

#include <stdlib.h>
#include <string.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"

int cmd_ivar(char *param)
{	char *value;

	if(!param || !*param) {
		error_syntax(0);
		return E_Syntax;
	}

	if((value = strchr(param, '=')) != 0)
		*value++ = '\0';

	return ctxtSetS(CTXT_TAG_IVAR, param, value);
}
