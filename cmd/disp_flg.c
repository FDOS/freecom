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
	 , F(dispPrompt), F(echo)
	 , F(swap), F(call)
	 , F(canexit), F(interactive)
	 , F(trace), F(debug)
	 , F(shiftlevel), F(base_shiftlevel)
	 , F(errorlevel), F(persistentMSGs)
	 , F(batchlevel)

	 , dispPrompt, echoBatch
	 , swap, called
	 , implicitVerbose, interactive
	 , traceMode, rewindBatchFile
	 , doExit, doCancel
	 , doQuit, gotoLabel? gotoLabel: "");
	return ctxtView(CTXT_TAG_FLAG, TEXT_FLAG_CTXT_EMPTY);
}
