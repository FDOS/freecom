/* $Id$
 *  DEL.C - del command.
 *
 *  Comments:
 *
 *  06/29/98 (Rob Lake rlake@cs.mun.ca)
 *      rewrote del to support wildcards
 *      added my name to the contributors
 *
 * 07/13/98 (Rob Lake)
 *  - fixed bug that caused del not to delete file with out attribute.
 *  - moved set, del, ren, and ver to there own files
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 09-Aug-1998 (Rob Lake <rlake@cs.mun.ca>)
 * - changed check for ^C
 * - alloced mem for char **arg
 *
 * 10-Aug-1998 ska
 * - fixed allocation of char **arg incl. out-of-memory check
 *
 * 1998/12/04 ska
 * - chg: vcgetchar() displays a space rather beep()s for no-printable
 *  characters. the caller can beep()
 * - chg: moved vcgetchar() to misc.c as it is multiply used now
 * - chg: use vcgetcstr() instead of vcgetchar() loop
 *
 * 1999/04/27 ska
 * chg: uniform command line parser
 * add: allow multiple patterns on command line
 * add: delete count
 *
 * 1999/07/02 ska
 * chg: replaced stat() by dfnstat() [reduces size of image]
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dfn.h>

#include "../include/lfnfuncs.h"
#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../strings.h"

#ifdef FEATURE_LONG_FILENAMES
#define abspath( x, y ) abspath( getshortfilename( x ), y )
#endif

static int optP, verbose;

#pragma argsused
optScanFct(opt_del)
{
  switch(ch) {
  case 'P': return optScanBool(optP);
  case 'V': return optScanBool(verbose);
  }
  optErr();
  return E_Useage;
}

int cmd_del(char *param)
{
	int ec = E_None;    /* exit code */
	int i;
	unsigned count = 0;

	struct ffblk f;

	/* Make fullname somewhat larger to ensure that appending
		a matched name, one backslash and one hope. */
	char fullname[MAXPATH + sizeof(f.ff_name) + 2],
	*p, *q;
	int len;

	char **arg;
	int argc, optc;

	/* initialize options */
	verbose = optP = 0;

	if((arg = scanCmdline(param, opt_del, 0, &argc, &optc)) == 0)
		return E_Other;

	if(!argc) {
		error_req_param_missing();
		ec = E_Useage;
	} else {
		i = 0;
		do {
			assert(arg[i]);

		/* Get the pattern fully-qualified */
			/* Note: An absolute path always contains:
				A:\\
				--> It's always three bytes long at minimum
				and always contains a backslash */
			p = abspath(arg[i], 1);
			if(!p)
				return E_Other;
			assert(strlen(p) >= 3);

			if((len = strlen(p)) >= MAXPATH) {
				error_filename_too_long(p);
				free(p);
				ec = E_Other;
				goto errRet;
			}
			strcpy(fullname, p);  /* Operating over a local buffer simplifies
					 the process; rather than keep the pattern
					 within dynamic memory */
			free(p);
			p = fullname + len;

			/* check if it is a directory */
			if(dfnstat(fullname) & DFN_DIRECTORY) {
				if (p[-1] != '\\')
					*p++ = '\\';
			}

			if(p[-1] == '\\')    /* delete a whole directory */
				p = stpcpy(p, "*.*");

			/* p := address to copy the filename to to form the fully-qualified
				filename */
			/* There is at least one backslash within fullname,
				because of dfnexpand() */
			while(*--p != '\\') ;
			++p;

			/* make sure user is sure if all files are to be
			 * deleted */
            /*
             * Also make sure to find the LAST '.', as long filenames
             * are allowed to contain several
             */
			if(!optP && *p == '*'
			 && ((q = strrchr(p, '.')) == 0 || q[1] == '*')) {
				if(userprompt(PROMPT_DELETE_ALL, p) != 1) {
					ec = E_Other;
					goto errRet;
				}
			}

			if (FINDFIRST(fullname, &f, FA_ARCH)) {
				error_sfile_not_found(fullname);
			} else do {
				strcpy(p, f.ff_name);       /* Make the full path */

				if(optP) {
					switch(userprompt(PROMPT_DELETE_FILE, fullname)) {
					case 4:             /* Quit/^Break pressed */
						ec = E_CBreak;
						goto errRet;
					case 3:				/* all */
						optP = 0;
					case 1:
						break;                /* yes, delete */
					default:
					case 2:
						continue;             /* no, don't delete */
					}
				}
				else if(cbreak) {  /* is also probed for in vcgetstr() */
					ec = E_CBreak;
					goto errRet;
				}

#ifdef NODEL
				/* define NODEL if you want to debug */
				puts(fullname);
#else
				if(verbose && !optP)
					displayString(TEXT_DELETE_FILE, fullname);
				if(unlink(fullname) != 0) {
					myperror(fullname);   /* notify the user */
				} else
					++count;
#endif

			} while (FINDNEXT(&f) == 0);
		} while(++i < argc && *arg[i]);/* arg[i] can be NULL with LFNS?? */
	}

errRet:
    FINDSTOP(&f);

	if(echo)
		dispCount(count, TEXT_MSG_DEL_CNT_FILES);

	freep(arg);
	return ec;
}
