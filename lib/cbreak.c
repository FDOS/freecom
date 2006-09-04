/*	$Id$

 * Check if Ctrl-Break was pressed during the last calls

	This file bases on MISC.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.6  2006/09/04 20:14:23  blairdude
	Support for BC

	Revision 1.5  2006/06/17 06:10:31  blairdude
	Optimization
	
	Revision 1.4  2006/06/13 02:10:19  blairdude
	Cleaned up some code, moved write in outc to fwrite to make everybody happy (thanks to Arkady for the reports)
	
	Revision 1.3  2006/06/12 04:55:42  blairdude
	All putchar's now use outc which first flushes stdout and then uses write to write the character to the console.  Some potential bugs have been fixed ( Special thanks to Arkady for noticing them :-) ).  All CONIO dependencies have now been removed and replaced with size-optimized functions (for example, mycprintf, simply opens "CON" and directly writes to the console that way, and mywherex and mywherey use MK_FP to access memory and find the cursor position).  FreeCOM is now
	significantly smaller.
	
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
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

#include <string.h> /* strlen */
#include <stdlib.h> /* free */
#include <fcntl.h>  /* O_WRONLY */
#include <stdarg.h>
#include <io.h>     /* _open, _write, _close */

#include "../include/batch.h"
#include "../include/misc.h"
#include "../include/command.h"
#include "../strings.h"

/* This is to prevent all that Turbo C CONIO stuff from being linked in */
static void mycprintf( char *fmt, ... )
{
    va_list args;
    char buffer[ 512 ];
    int consolehandle = _open( "CON", O_WRONLY );
    /* 
     * Just as cprintf _ensures_ printing directly to the console, so will
     * opening the console and writing to it do the same
     */

    va_start( args, fmt );
    vsprintf( buffer, fmt, args );
    _write( consolehandle, buffer, strlen( buffer ) );
    _close( consolehandle );
    va_end( args );
}

int chkCBreak(int mode)
{
  static int leaveAll = 0;      /* leave all batch files */

  switch (mode)
  {
    case BREAK_ENDOFBATCHFILES:
      leaveAll = 0;
      return 0;

    case 0:
      if (!bc)
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

      	if(!getPromptString(PROMPT_CANCEL_BATCH, &chars, &fmt)) {
      			/* Fatal error <-> Terminate all batches */
      		leaveAll = 1;
      		break;
      	}
      	if(bc && bc->bfnam)
			mycprintf(fmt, bc->bfnam);
      	else {
      		char *fnam = getString( TEXT_UNKNOWN_FILENAME );
            
            mycprintf( fmt, fnam ? fnam : "<<unknown>>" );
            free( fnam );
		}

		while((ch = cgetchar()) == 0 || (ch = mapMetakey(chars, ch)) == 0)
			beep();

		mycprintf("\r\n");
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
