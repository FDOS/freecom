/*	$id$
	$Locker$	$Name$	$State$

 *  Defines the functions only necessary while debugging is active

	This file bases on DEBUG.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.1  2001/07/01 22:04:31  skaus
	Update #3

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

#include <conio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <alloc.h>

#include "../include/debug.h"

FILE *dbg_logfile = stdout;
FILE *dbg_logfile2 = 0;
char *dbg_logname2 = 0;

void dbg_print(const char * const fmt, ...)
{	va_list ap;

	va_start(ap, fmt);
	if(dbg_logfile) {
		vfprintf(dbg_logfile, fmt, ap);
		fflush(dbg_logfile);
	}
	if(dbg_logfile2) {
		vfprintf(dbg_logfile2, fmt, ap);
		fflush(dbg_logfile2);
	}
	va_end(ap);
}
