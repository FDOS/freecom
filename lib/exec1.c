/*	$Id$

	Decode the return code of the execution of an external program:
		errorlevel := input? input
					   : normal exit? errorlevel
					                : ^CBreak errorlevel

	This file bases on EXEC.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.5  2005/07/10 04:46:39  perditionc
	build fixes and minor enhancements

	Revision 1.4  2004/12/01 21:15:48  skaus
	add: /Z: Display exit code after external command {Bernd Blaauw}
	
	Revision 1.3  2004/10/25 19:37:34  skaus
	fix: LH: Errorlevel of program effects LH's error reporting {Eric Auer}
	
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.1  2001/04/12 00:33:53  skaus
	chg: new structure
	chg: If DEBUG enabled, no available commands are displayed on startup
	fix: PTCHSIZE also patches min extra size to force to have this amount
	   of memory available on start
	bugfix: CALL doesn't reset options
	add: PTCHSIZE to patch heap size
	add: VSPAWN, /SWAP switch, .SWP resource handling
	bugfix: COMMAND.COM A:\
	bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
	add: command MEMORY
	bugfix: runExtension(): destroys command[-2]
	add: clean.bat
	add: localized CRITER strings
	chg: use LNG files for hard-coded strings (hangForEver(), init.c)
		via STRINGS.LIB
	add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
	add: fixstrs.c: prompts & symbolic keys
	add: fixstrs.c: backslash escape sequences
	add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
	chg: splitted code apart into LIB\*.c and CMD\*.c
	bugfix: IF is now using error system & STRINGS to report errors
	add: CALL: /N
	
 */

#include "../config.h"

#include <assert.h>
#include <dos.h>

#include "../include/command.h"

#ifdef DISP_EXITCODE
	extern int exitReason;	/* global variable, defined in dispexit.c, set in exec1.c */
	void displayExitcode(void); /* also in include/misc.h */
#endif

void setErrorLevel(int rc)
{	struct REGPACK rp;

	dprintf(("[exec: DOS error code of exec(): %d]\n", rc));

	if(!rc) {
#ifndef DISP_EXITCODE
		int exitReason;		/* else we use the global variable */
#endif
		rp.r_ax = 0x4d00;           /* get return code */
		intr(0x21, &rp);
		rc = rp.r_ax & 0xFF;
		exitReason = (rp.r_ax >> 8) & 0xFF;
			/*	0 -> normal
				1 -> ^Break / ^C
				2 -> Critical Error
				3 -> TSR
			*/
		dprintf(("[exec: exit code: %u:%u]\n", exitReason, rc));
		if(exitReason == 0x1)
			ctrlBreak = 1;
		if(exitReason == 0x2)	/* Shallt change in the future */
			ctrlBreak = 1;
		if(ctrlBreak && !rc)	/* Make sure this condition is reflected */
			rc = CBREAK_ERRORLEVEL;
	}
#ifdef DISP_EXITCODE
	else
		exitReason = -1;
#endif

	errorlevel = rc;	/* assign DOS error code, if the call failed itself */
#ifdef DISP_EXITCODE
	displayExitcode();
#endif
}

