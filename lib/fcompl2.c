/*	$id$
	$Locker$	$Name$	$State$

	Show files matching a given file completion pattern
	returns 1 if at least one match, else returns 0

	This function should somehow use:
		DIR/B/W <pattern>

	This file bases on FILECOMP.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.1  2001/06/25 20:06:36  skaus
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

#include <assert.h>
#include <dir.h>
#include <string.h>

#include <dfn.h>

#include "../include/command.h"
#include "../strings.h"
#include "../err_fcts.h"

int show_completion_matches(char * const str, const unsigned charcount)
{	int count, is_dir;
	struct ffblk f;
	char *fnam, *end, buf[sizeof(f.ff_name) + 2];

	assert(str);
	assert(charcount);
	assert(strlen(str) < charcount);

	fnam = dfnfilename(str);
	assert(fnam);
	if(4 >= charcount - (fnam - str)) {
		error_long_internal_line();
		return 0;
	}

	end = strchr(fnam, '\0');
	if(strchr(fnam, '.'))
		/* There is a dot --> search for extension only */
		strcat(fnam, "*");
	else	strcat(fnam, "*.*");

	count = 1;
	if(findfirst(str, &f, FA_DIREC) == 0) {
		do if((is_dir = f.ff_attrib & FA_DIREC) == 0
		 || (strcmp(f.ff_name, ".") && strcmp(f.ff_name, ".."))) {
		 	if(--count == 0) {
		 		putchar('\n');
		 		count = 5;
		 	}
		 	if(is_dir)
		 		sprintf(buf, "[%s]", f.ff_name);
			displayString(TEXT_FILE_COMPLATION_DISPLAY
			 , is_dir? buf: f.ff_name);
		} while(findnext(&f) == 0);
		putchar('\n');
		*end = 0;			/* Make the string unchanged */
		return 1;
	}

	/* no match found */
	beep();
	*end = 0;				/* Make the string unchanged */

	return 0;
}
