/*	$id$
	$Locker$	$Name$	$State$

	int is_fnchar()

	is filename character?

	Note: These are none: ."/\\[]:|<>+=;,

	This file bases on COMMAND.C of FreeCOM v0.81 beta 1.

	$Log$
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

#include <fmemory.h>

#include "../include/nls.h"

int is_fnchar(const int c)
{

#ifdef FEATURE_NLS
  refreshNLS();   /* refresh the NLS information */

#define uc unsigned char
  return !(c < (uc)nlsBuf->inclFirst || c > (uc)nlsBuf->inclLast
   || (c >= (uc)nlsBuf->exclFirst && c <= (uc)nlsBuf->exclLast)
   || _fmemchr(nlsBuf->illegalChars, c, nlsBuf->illegalLen));
#undef uc
#else
  return !(c <= ' ' || c == 0x7f || strchr(".\"/\\[]:|<>+=;,", c));
#endif
}
