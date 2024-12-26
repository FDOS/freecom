/*	$Id$

 *  Skip a quoted word, stop at end of string or at the given string
 	compared case-sensitively or, if not specified, any argument delimter.

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

#include "../include/command.h"
#include "../include/cmdline.h"

#if 0
char *skipqword(const char *pp, const char * const stop)
{	size_t len;
	int quote = 0;

	len = stop? strlen(stop): 0;

	if(*pp) do {
		if(quote) {
			if(quote == *pp)
				quote = 0;
		} else if(strchr(QUOTE_STR, *pp))
			quote = *pp;
		else if(len? (memcmp(pp, stop, len) == 0): isargdelim(*pp))
			break;
	} while(*++pp);

	return (char *) pp;		/* strip const */
}
#else
char *skipqword(char *pp, const char * const stop)
{	size_t len;
	int quote = 0;
	char *pp_trunc = 0;

	len = stop? strlen(stop): 0;

	if(*pp) do {
        /* if we find a quote then continue until terminator or end of string found */
		if(quote) {
			if(quote == *pp) {
                pp_trunc = 0;
				quote = 0;
            } else if(len && (memcmp(pp, stop, len) == 0)) {
                /* no matching quote, so back up to delimiter and mark end of pp */
				char *p = pp;
				while(*p && (!quote || (quote != *p)) && !isargdelim(*p)) p--;
                p++;
                if (p < pp) *p='\0';
                break;
            } else if(!pp_trunc && isargdelim(*pp)) {
                pp_trunc = pp; /* if end of string but no matching quote, terminate at 1st delimiter instead */
            }
		} else if(strchr(QUOTE_STR, *pp))
			quote = *pp;
		else if(len && (memcmp(pp, stop, len) == 0)) {
                /* no matching quote, so back up to delimiter and mark end of pp */
				char *p = pp;
				while(*p && (!quote || (quote != *p)) && !isargdelim(*p)) p--;
                p++;
                if (p < pp) *p='\0';
                break;
        } else if(!pp_trunc && isargdelim(*pp)) {
            pp_trunc = pp;  /* truncate at delimter if not in quoted string */
			break;
        }
	} while(*++pp);
	if (pp_trunc) {
        *pp_trunc = '\0'; /* never found end of quote so stop at first delimiter */
        dprintf(("returning [%s]\n", pp_trunc+1));
        return pp_trunc+1;
    }

    dprintf(("returning [%s]\n", pp));
	return (char *) pp;		/* strip const */
}
#endif
