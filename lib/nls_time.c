/*	$id$
	$Locker$	$Name$	$State$

	Return the dynamically allocated ASCII representation of
	the passed in time

	This file bases on NLS.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2001/06/12 22:56:59  skaus
	bugfix: nls_maketime(): 12hour display does not free AM/PM id

	Revision 1.2  2001/04/29 11:33:51  skaus
	chg: default heap size (tools\ptchsize) set to 6KB
	chg: error displaying functions centralized into lib\err_fcts.src
	add: displayError()
	chg: all errors are displayed through functions void error_*()
	bugfix: somtimes error messages are not displayed (see displayError())
	bugfix: docommand(): type:file must pass ":file" to TYPE
	bugfix: error_sfile(): string _SFILE_
	bugfix: error message on empty redirection
	bugfix: comma and semicolon ';' are recognized as argument seperators
		of internal commands
	
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
#include "../include/nls.h"
#include "../strings.h"

char *nls_maketime(int mode, int hour, int minute, int second, int fraction)
{	char buf[4 + 4 + sizeof(int) * 8 * 4 + 6];
	char *p;
	int i, pm;

#ifdef FEATURE_NLS
	refreshNLS();
#define fmt nlsBuf->timefmt
#define sep nlsBuf->timeSep
#define dsep nlsBuf->decimalSep
#else
#define fmt 0
#define sep ":"
#define dsep "."
#endif

	/** Warning: condition always true -- if !NLS **/
	if(fmt == 0) {		/* 12hour display */

		if((pm = hour >= 12) != 0) {
			hour -= 12;
		}
		if(hour == 0)
			hour = 12;

		i = sprintf(p = buf, "%2u%s%02u", hour, sep, minute);
	} else {
		/** Warning: unreachable code -- if !defined(NLS) **/
		i = sprintf(p = buf, "%02u%s%02u", hour, sep, minute);
	}

	assert(strlen(buf) < sizeof(buf));

	if(i == EOF) return 0;
	if(second >= 0) {
		i = sprintf(p += i, "%s%02u", sep, second);
		if(i == EOF) return 0;
		if(fraction) {
			i = sprintf(p += i, "%s%u", dsep, fraction);
			if(i == EOF) return 0;
		}
	}

	assert(strlen(buf) < sizeof(buf));

	/** Warning: conditional always true -- if !NLS **/
	if(fmt == 0) {
		char *q = getString(pm? TEXT_STRING_PM: TEXT_STRING_AM);
		if(!q)		return 0;
		if(mode & NLS_MAKE_SHORT_AMPM) {
			*(p += i) = *ltrimsp(q);
			p[1] = 0;
		} else
			strcpy(p + i, q);
		free(q);
	}

	assert(strlen(buf) < sizeof(buf));

	return strdup(buf);
}

