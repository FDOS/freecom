/*	$Id$

 * Scan command line and handle options
 *  line may be NULL and is treated as empty.
 *  opt may be NULL and is treated as "no valid option".
 *    This will print an error message, if an option is specified.
 *  Return:
 *    == NULL: error, message already on screen any argument possibly
 *      modified
 *    != NULL: argument buffer, must be freed with "freep()"
 *      + *argc := number of non-option arguments within array, &
 *      + *opts := number of parsed options.
 *      + array contains *argc entries now.

	This file bases on CMDLINE.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

	Revision 1.2  2001/08/15 00:12:27  skaus
	bugfix: scancmd.c: /B and /A let COPY fail (ec == E_Ignore) (bug 829)
	upd: HTML docs
	
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
#include <stdlib.h>

#include "../include/command.h"
#include "../include/cmdline.h"
#include "../err_fcts.h"

/*
 * Parse global options
 */
static int parseOptions(optScanner fct, void * const arg, char **argv, int *argc, int *optcnt)
{ int ec = E_None;
  char **argp, *a;

  assert(argv);
  assert(argc);
  assert(optcnt);

  *optcnt = 0;
  argp = argv;
  while((*argp++ = a = *argv++) != 0)
    if(isoption(a))
      if((ec = scanOption(fct, arg, a)) == E_None) {
        free(*--argp);  /* ignore (overwrite) it */
        ++*optcnt;
      } else if(ec != E_Ignore) { /* the entry caused an error */
        /* Copy the remaining entries */
        while((*argp++ = *argv++) != 0);
        break;
      }

  *argc -= *optcnt;
  return ec == E_Ignore? E_None: ec;    /* everything done */
}

char **scanCmdline(char *line, optScanner fct, void * const arg
  , int *argc, int *opts)
{ char **argv;

  assert(argc);
  assert(opts);

  if((argv = split(line, argc)) == 0) {
    error_out_of_memory();
    return 0;
  }

  if(parseOptions(fct, arg, argv, argc, opts) != E_None) {
    freep(argv);
    return 0;
  }

  return argv;
}
