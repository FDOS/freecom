/* $Id$

	Break a command line and push the arguments into
	the ARG tag (6)

	The base and shiftlevel flags are not changed by this function.

	Return:
		internal error codes
*/

#include "../config.h"

#include <assert.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/cmdline.h"
#include "../include/ierror.h"

int setArguments(const char * const line)
{	char *start, *s;
	char c;
	int ec;

	ec = E_None;
	if((s = (char*)line) != 0)
		while(ec == E_None && *(start = skipdm(s)) != '\0') {
			s = skip_word(start);		/* end of argument */
			assert(s);
			c = *s;
			*s = 0;
			ec = ctxtPush(CTXT_TAG_ARG, start);
			*s = c;
		}

	return ec;
}
