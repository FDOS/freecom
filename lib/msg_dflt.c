/*	$Id$

	Create default message or display Out-of-memory error onto screen

	This file bases on MESSAGES.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2004/06/29 14:14:56  skaus
	fix: help screen of internal commands causes "Unknown command error" {Bernd Blaauw}

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/misc.h"
#include "../strings.h"

static struct {
	unsigned id;
	const char *string;
} cached_ids[] = {
	{ TEXT_ERROR_OUT_OF_MEMORY,
#undef TEXT_ERROR_OUT_OF_MEMORY
		TEXT_ERROR_OUT_OF_MEMORY }
	, { TEXT_ERROR_OPEN_FILE,
#undef TEXT_ERROR_OPEN_FILE
		TEXT_ERROR_OPEN_FILE }
	, { TEXT_ERROR_FCOM_INVALID,
#undef TEXT_ERROR_FCOM_INVALID
		TEXT_ERROR_FCOM_INVALID }
	, { TEXT_ERROR_SET_ENV_VAR,
#undef TEXT_ERROR_SET_ENV_VAR
		TEXT_ERROR_SET_ENV_VAR }
	, { ~0, 0 }
};

char *defaultMessage(unsigned id)
{
	char *h;
	int i;

	for(i = 0; cached_ids[i].id != (unsigned)~0; ++i) {
		if(id == cached_ids[i].id) {
			h = strdup(cached_ids[i].string);
			if(h) return h;
			break;
		}
	}

  /* Allocate 5 bytes for the numerical ID itself */
  if(id != cached_ids[0].id
   && (h = malloc(sizeof(MSG_DFL_PATTERN) + 5)) != 0) {
    /* Got enough memory to allocate the stuff */
    sprintf(h, MSG_DFL_PATTERN, id);
    return h;
  }

  puts(cached_ids[0].string);
  return 0;
}
