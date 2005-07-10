/* Note: this is file here to allow compilation and is not in FreeCom's cvs,
   as I'm sure Steffen has an implementation already.
*/

#include "../config.h"

#ifdef DISP_EXITCODE

#include "../include/misc.h"
#include "../strings.h"


int exitReason;	        /* defined in dispexit.c, set in exec1.c, reason program terminated */
extern int dispExitcode;  /* defined in shell/init.c, nonzero to display (/Z) */
extern int errorlevel;    /* defined ???, holds errorlevel command returned, set in exec1.c */


void displayExitcode(void)
{
	if (dispExitcode)
	{
		/* printf("Return code (ERRORLEVEL): %i\n", errorlevel); */
		/* Exit code (ERRORLEVEL): %u, reason: %u (%s) */
		displayString(TEXT_DISP_EXITCODE, errorlevel, exitReason,
			(!exitReason)?getMessage(TEXT_EXIT_REASON_0):    /* 0==normal */
			(exitReason==1)?getMessage(TEXT_EXIT_REASON_1):  /* Ctrl-Break */
			(exitReason==2)?getMessage(TEXT_EXIT_REASON_2):  /* Critical Error */
			(exitReason==3)?getMessage(TEXT_EXIT_REASON_3):  /* TSR ??? */
			(exitReason==-1)?getMessage(TEXT_EXIT_REASON_NEG_1): /* DOS API error */
			"Unknown"
		);
	}
}

#endif DISP_EXITCODE
