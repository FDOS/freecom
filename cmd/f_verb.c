/* $Id$

	function: verbatim(string)

	Cause that the command line parser does not expand the next
	characters until string is found or end of string (EOS) is reached.

	Return:
		0: ignore anything til EOS (also in case of error)
		else: pointer to dynamically allocated string
*/

#include "../config.h"

#include "../include/command.h"
#include "../include/context.h"
#include "../include/cmdline.h"
#include "../include/misc.h"

char *fct_verbatim(char *param)
{	
	assert(verbatim_token == 0);
	if(!(param && *param && (verbatim_token = estrdup(param)) == 0))
		verbatim = 1;
	return 0;		/* nothing to replace */
}
