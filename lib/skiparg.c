/*	$id$
	$Locker$	$Name$	$State$

 *  Skip a quoted word, stop at any argument delimter.

	This file bases on CMDLINE.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.2.1  2001/07/16 20:28:38  skaus
	Update #9

	Revision 1.2.4.3  2001/07/08 17:23:43  skaus
	Update #7
	
	Revision 1.2.4.2  2001/06/21 21:40:35  skaus
	Update #2
	
	Revision 1.2.4.1  2001/06/19 20:42:23  skaus
	Update #1
	
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

#include "../include/misc.h"
#include "../include/cmdline.h"

char *skipQuoteArg(const char * const pp)
{
	char *p;

	assert(pp);

	p = (char*)pp - 1;
	while(*(p = skipquote(p + 1)) != 0 && !isargdelim(*p));

	return p;
}
