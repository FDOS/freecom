/*	$id$
	$Locker$	$Name$	$State$

 * Check if Ctrl-Break was pressed during the last calls

	This file bases on MISC.C of FreeCOM v0.81 beta 1.

	$Log$
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

int chkCBreak(int mode)
{
  static int leaveAll = 0;      /* leave all batch files */

  switch (mode)
  {
    case BREAK_ENDOFBATCHFILES:
      leaveAll = 0;
      return 0;

    case 0:
      if (!F(batchlevel))
        goto justCheck;

    case BREAK_BATCHFILE:
      if (leaveAll)
        return 1;
      if (!ctrlBreak)
        return 0;

      /* we need to be sure the string arrives on the screen!
      	Therefore userprompt() is not we need. */
      {	char *fmt, *chars;
      	int ch;
      	ctxtEC_Batch_t far *bc = ecLastF();

      	if(!getPromptString(PROMPT_CANCEL_BATCH, &chars, &fmt)) {
      			/* Fatal error <-> Terminate all batches */
      		leaveAll = 1;
      		break;
      	}
      	if(bc)
			cprintf(fmt, bc->ec_fname);
      	else {
      		char *fnam;

      		if((fnam = getString(TEXT_UNKNOWN_FILENAME)) == 0)
      			cprintf(fmt, (char far*)"<<unknown>>");
      		else {
				cprintf(fmt, (char far*)fnam);
				free(fnam);
			}
		}

		while((ch = cgetchar()) == 0 || (ch = mapMetakey(chars, ch)) == 0)
			beep();

		cputs("\r\n");
		freePromptString(chars, fmt);

		switch(ch) {
		/* case 1:		Yes -> just fall through */
		case 2:			/* No */
			return ctrlBreak = 0;   /* ignore */
		case 3:			/* leave All batchfiles */
		  leaveAll = 1;
		  break;
		}
	  }

      break;

    justCheck:
    case BREAK_FORCMD:         /* FOR commands are part of batch processing */
      if (leaveAll)
        return 1;
      /* fall through */

    case BREAK_INPUT:
      if (!ctrlBreak)
        return 0;
      break;
  }

  ctrlBreak = 0;                /* state processed */
  return 1;
}
