/*	$id$
	$Locker$	$Name$	$State$

 * Check if Ctrl-Break was pressed during the last calls

	This file bases on MISC.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.9  2001/07/25 20:17:28  skaus
	Update #12

	Revision 1.1.4.6  2001/07/08 17:23:43  skaus
	Update #7
	
	Revision 1.1.4.5  2001/07/07 20:37:17  skaus
	Update #6
	
	Revision 1.1.4.4  2001/07/05 22:18:34  skaus
	Update #5
	
	Revision 1.1.4.3  2001/07/01 22:04:31  skaus
	Update #3
	
	Revision 1.1.4.2  2001/06/21 21:40:34  skaus
	Update #2
	
	Revision 1.1.4.1  2001/06/19 20:42:23  skaus
	Update #1
	
	Revision 1.1  2001/04/12 00:33:52  skaus
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
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "../include/misc.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../strings.h"

int chkCBreak(void)
{
	if(lflag_ignoreCBreak)
		return 0;		/* completely ignore this call */

	if(lflag_doExit || lflag_doCancel || lflag_doQuit) {
		ctrlBreak = 0;                /* state processed */
		return 1;
	}
	if(!ctrlBreak)
		return 0;
	ctrlBreak = 0;                /* state processed */

	if(gflag_batchlevel) {
		/* we need to be sure the string arrives on the screen!
			Therefore userprompt() is not what we need. */

		char *fmt, *chars, *fnam;
		int ch;

		if(!getPromptString(PROMPT_CANCEL_BATCH, &chars, &fmt)) {
			/* Fatal error <-> Terminate all batches */
			return lflag_doCancel = 1;
		}
		if((fnam = fct_batchname(0)) == 0
		 && (fnam = getString(TEXT_UNKNOWN_FILENAME)) == 0)
			cprintf(fmt, "<<unknown>>");
		else {
			cprintf(fmt, fnam);
			myfree(fnam);
		}

		while((ch = cgetchar()) == 0
		 || (ch = mapMetakey(chars, ch)) == 0)
			beep();

		cputs("\r\n");
		freePromptString(chars, fmt);

		switch(ch) {
		case 1:		/*	Yes --> leave this batch file */
			lflag_doQuit = 1;
			break;
		case 2:			/* No --> ignore */
			return 0;
		case 3:			/* leave All batchfiles */
			lflag_doCancel = 1;
			break;
		}
	}

	return 1;
}
