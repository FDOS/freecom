/* $id$

	Create the name of the next available special internal variable,
	lock it wihin the dynamic context and return its name in a
	dynamically allocated buffer.

	Special internal variables are of the format:
		#<hexadecimal_number>

	Return:
		0: on failure, message already displayed
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"

char *ecMkIVar(void)
{	char ivar[sizeof(int) * 2 + 2 + 1];
	unsigned num;

	ivar[0] = CTXT_TAG_IVAR;
	ivar[1] = '#';
	num = 0;
	do {
		sprintf(ivar + 2, "%x", num);
		assert(strlen(ivar) < sizeof(ivar));
		if(ctxtGetItem(0, CTXT_TAG_IVAR, ivar, (char*)0) == 1) {
			/* No such item --> return it! */
			/* Create an empty entry, so it's locked for next attempts */
			if(chgCtxt(CTXT_TAG_IVAR, ivar, "") == 0)
				return estrdup(ivar + 1);
			/* else failed to create lock entry */
			return 0;
		}
	} while(++num);

	error_context_out_of_memory();
	return 0;		/* All probed */
}
