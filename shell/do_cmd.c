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

char *readCommand(ctxtEC_t far * const ctxt)
{
	char *line;
	ctxtEC_Cmd_t far *cc;
	int mode;

	dprintf(("readCommand()\n"));

	assert(ctxt);
	assert(ctxt->ctxt_type == EC_TAG_COMMAND);
	assert(ctxt->ctxt_length >= sizeof(ctxtEC_Cmd_t));

	lflag_doQuit = cbreak;

	cc = ecData(ctxt, ctxtEC_Cmd_t);
	assert(_fstrlen(cc->ec_cmd) == ctxt->ctxt_length - sizeof(ctxtEC_Cmd_t));

	if(((mode = cc->ec_flags) & EC_CMD_IGNORE_EXIT) == 0) {
		if(lflag_doExit || lflag_doCancel || lflag_doQuit) {
#ifdef DEBUG
			char far*cmd = ecData(ctxt, char);
			dprintf(("[CTXT: Skipping command: %Fs]\n", cmd));
#endif
			return 0;
		}
	} else {
		dprintf(("[CTXT: Completely ignore ^Break status]\n"));
		lflag_ignoreCBreak = 1;
	}

	if((line = emalloc(_fstrlen(cc->ec_cmd) + 4)) != 0) {
		char *q = line;

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
		if(mode & EC_CMD_FORCE_INTERNAL)
			q = stpcpy(line, FORCE_INTERNAL_COMMAND_STRING);
		_fstrcpy(TO_FP(q), cc->ec_cmd);

		ecPop();		/* Remove this context as it has been done
							completely now */
	}

	return line;
}
