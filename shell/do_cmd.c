/* $id$

	Perform a C context (cmd)

	Return:
		0: on error
		else: cmdline, but context gets poped
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include "../include/context.h"
#include "../include/misc.h"

char *readCommandNoIgnore(ctxtEC_t far * const ctxt)
{	char far* cmd;	/* arg of ctxt */
	char *line;

	dprintf(("readCommandNoIgnore()\n"));

	assert(ctxt);
	assert(ctxt->ctxt_type == EC_TAG_COMMAND_NO_IGNORE);
	assert(ctxt->ctxt_length >= 1);

	cmd = ecData(ctxt, char);
	assert(_fstrlen(cmd) == ctxt->ctxt_length - 1);

	if((line = edupstr(cmd)) != 0) {
		ecPop();
		if(*line == '@') {		/* prevent any echo'ing */
			*line = ' ';		/* remove this character from string */
								/* cannot use ++line as the caller will
									free() the returned string */
			echoBatch = 0;
		}
	}

	return line;
}

char *readCommand(ctxtEC_t far * const ctxt)
{
	dprintf(("readCommand()\n"));

	assert(ctxt);
	assert(ctxt->ctxt_type == EC_TAG_COMMAND);

	if(doExit || doCancel || doQuit)
		return 0;

		/* to fake readCommndNoIgnore()'s asserts() */
	assert((ctxt->ctxt_type = EC_TAG_COMMAND_NO_IGNORE) != 0);
	return readCommandNoIgnore(ctxt);
}
