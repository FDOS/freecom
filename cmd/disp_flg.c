/* $id$

	! internal command to diplsay the flags currently in effect.

*/

#include "../config.h"

#include "../include/command.h"
#include "../include/context.h"
#include "../strings.h"

#pragma argsused
int cmd_dispFlags(char *param)
{
	displayString(TEXT_FLAGS_DISPLAY
	 , gflag_dispPrompt, gflag_echoBatch
	 , gflag_echo
	 , gflag_swap, gflag_called
	 , gflag_canexit, gflag_interactive
	 , gflag_trace, gflag_debug
	 , gflag_shiftlevel, gflag_base_shiftlevel
	 , gflag_errorlevel, gflag_persistentMSGs
	 , gflag_batchlevel

	 , lflag_echo
	 , lflag_swap, lflag_called
	 , implicitVerbose, lflag_interactive
	 , lflag_trace, lflag_rewindBatchFile
	 , lflag_doExit, lflag_doCancel
	 , lflag_doQuit, lflag_gotoLabel? lflag_gotoLabel: "");
	return ctxtView(CTXT_TAG_FLAG, TEXT_FLAG_CTXT_EMPTY);
}
