/* $id$

	Perform a C or c context (cmd)
	c context will ignore any active Exit/Cancel/Quit flags.

	Return:
		0: on error
		else: cmdline, but context gets poped
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <fmemory.h>

#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"

char *readCommand(char far * const ctxt)
{
	char *line, *p;
	int mode;

	dprintf(("readCommand()\n"));

	assert(ctxt);
	assert(*ctxt == EC_TAG_COMMAND);

	lflag_doQuit = cbreak;

	if((p = line = regStr(edupstr(ctxt + 1))) == 0)
		return 0;
	if(1 != sscanf(p, "%u", &mode) || 0 == (p = strchr(p, ' '))) {
		error_context_corrupted();
		return 0;
	}
	if(!*++p)
		return 0;		/* empty command --> ignore */

	if((mode & EC_CMD_IGNORE_EXIT) == 0) {
		if(lflag_doExit || lflag_doCancel || lflag_doQuit) {
#ifdef DEBUG
			dprintf(("[CTXT: Skipping command: %s]\n", p));
#endif
			return 0;
		}
	} else {
		dprintf(("[CTXT: Completely ignore ^Break status]\n"));
		lflag_ignoreCBreak = 1;
	}

	if(mode & EC_CMD_SILENT) {
		dprintf(("[CTXT: disable local ECHO status]\n"));
		lflag_echo = 0;
	}
	if(mode & EC_CMD_NO_TRACE) {
		dprintf(("[CTXT: disable local TRACE status]\n"));
		lflag_trace = 0;
	}
	if(mode & EC_CMD_NONINTERACTIVE) {
		dprintf(("[CTXT: disable local INTERACTIVE status]\n"));
		lflag_interactive = 0;
	}
	if(p != line)
		memset(line, ' ', p - line);

	ecPop();		/* Remove this context as it has been done
						completely now */

	return line;
}
