/*	$Id$

 * Scan an option and update its value within the option array
 *  Return: 0 on success

	This file bases on CMDLINE.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

	Revision 1.2  2002/11/06 20:36:37  skaus
	bugfix: /?: found on all the command line
	
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
#include <ctype.h>
#include <string.h>

#include "../include/command.h"
#include "../include/cmdline.h"
#include "../err_fcts.h"

unsigned currCmdHelpScreen = 0;

int scanOption(optScanner fct, void * const ag, char *rest)
{
  char *line, *arg, *optend;
  int ch, bool;

  assert(rest);

  if(!isoption(line = rest))  return -1;

  /* check for leading boolean character */
  switch(*++line) {
  case '\0':    return E_None;    /* empty option */
  case '-':   bool = -1; ++line; break;
  case '+':   bool = 1; ++line; break;
  default:    bool = 0; break;
  }

  if(!isprint(ch = toupper(*line)) || strchr("-+=:", ch)) {
    error_illformed_option(rest);
    return E_Useage;
  }

  if(ch == '?' && currCmdHelpScreen) {
  	displayString(currCmdHelpScreen);
  	return E_Help;
  }

  if((optend = strpbrk(line, "=:")) != 0) {  /* option has argument */
    arg = optend + 1;
  } else {
    arg = 0;
    optend = strchr(line, '\0');
  }

  /* check for trailing boolean character */
  switch(optend[-1]) {
  case '-': bool = -1;
        --optend;
        break;
  case '+': bool = 1;
        --optend;
        break;
  }

  *optend = '\0';

  if(!fct) {
    error_invalid_lswitch(line);
    return E_Useage;
  }

  return fct(line, line[1]? 0: ch, bool, arg, ag);
}
