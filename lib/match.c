/*	$Id$

 *  Check if the passed line begins with a specified word
 *  The word is matched case-insensitively.
 *  'len' is the length of the word.
 *  On success, the pointer is placed onto the next word.
 *  Return: 0: on failure

	This file bases on CMDLINE.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

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
#include <ctype.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/misc.h"

int match_(char ** const Xp, const char * const word, int len)
{	char *p;

	assert(Xp && *Xp);
	assert(word && *word);
	assert(len > 0);

	if(strncmpi(p = *Xp, word, len) == 0) {
		/* line begins with string, now test if it is a word */
		p += len;
		if(*p) {
			char *q = ltrimcl(p);	/* skip to next token */
			if(q == p) 				/* no word boundary */
				return 0;
			p = q;
		}
		*Xp = p;
		return 1;   /* found */
	}

	return 0;
}
