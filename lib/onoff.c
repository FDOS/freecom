/*	$Id$

 *	Tests if a string is ON or OFF

	This file bases on MISC.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.4  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

	Revision 1.3  2002/11/12 18:56:48  skaus
	bugfix: onOffStr(): zaps trailing argument delimiters, e.g. ECHO set=
	
	Revision 1.2  2001/04/29 12:24:36  skaus
	bugfix: >>PATH<< with empty %PATH% --> PATH=(null)
	fix: BREAK/VERIFY ignore trailing spaces
	fix: >>PATH ;<< remove PATH environment variable
	fix: cache 3 environment variables to overcome nested useage (e.g. when
		loading message segment to print environment variable)
	
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
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/misc.h"

enum OnOff onoffStr(char *line)
{	enum OnOff state = OO_Other;

	if(!line)
		return OO_Null;
	line = ltrimcl(line);
	if(!*line)
		return OO_Empty;
	if(matchtok(line, D_OFF))
		state = OO_Off;
	else if(matchtok(line, D_ON))
		state = OO_On;
	return *line? OO_Other: state;
}
