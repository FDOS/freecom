/*	$Id$

	Parses a string as PROMPT string and display the result onto the screen

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2004/05/03 20:36:50  skaus
	fix: PROMPT $V: include FreeCOM version information [#1776]

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
#define FEATURE_ENVVARS_IN_PROMPT

#include <assert.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/context.h"
#include "../include/command.h"
#include "../include/misc.h"
#include "../err_fcts.h"

#define PROMPTVAR "PROMPT"

void displayPrompt(const char *pr)
{
#ifdef FEATURE_ENVVARS_IN_PROMPT
	char *buf = strdup(pr);
	char *expanded = malloc(MAX_INTERNAL_COMMAND_SIZE + sizeof(errorlevel) * 8);

	if(buf && expanded) {
		if(!expandEnvVars(buf, expanded))
			error_line_too_long();
		else
			pr = expanded;
	}
	free(buf);
#endif

  while (*pr)
  {
    if(*pr != '$') {
      putchar(*pr);
    } else {
      switch (toupper(*++pr)) {
      case 'A': putchar('&'); break;
      case 'B': putchar('|'); break;
      case 'C': putchar('('); break;
      /* case 'D': see below */
      case 'E': putchar(27);  break;
      case 'F': putchar(')');  break;
      case 'G': putchar('>'); break;
      case 'H': putchar(8);   break;
      case 'L': putchar('<'); break;
      /* case 'M': putchar('<'); break; remote name of current drive */
      /* case 'N': see below */
      /* case 'P': see below */
      case 'Q': putchar('='); break;
      case 'S': putchar(' '); break;
      /* case 'T': see below */
      /* case 'V': see below */

      case '$': putchar('$'); break;
      case '_': putchar('\n'); break;
      /* case '+': see below */

        case 'D':
          {	 char *p;
          	 if((p = curDateLong()) != 0) {
          	 	fputs(p, stdout);
          	 	free(p);
          	 }
            break;
          }
        case 'N':
          {
            putchar(getdisk() + 'A');
            break;
          }
        case 'P':
          {
			char *p;

			if((p = cwd(0)) != 0) {
				fputs(p, stdout);
				free(p);
			}

            break;
          }
        case 'T':
          {
            char *p;

            if((p = curTime()) != 0) {
				fputs(p, stdout);
				free(p);
			}

            break;
          }
        case 'V':
          {
            /* #1776 fputs(shellname, stdout); */
            printf("%s v%s", shellname, shellver);
            break;
          }
        case '+':	/* Levels of PUSHD */
         {
#ifdef INCLUDE_CMD_PUSHD
			ctxt_info_t *info;
			int i;

			info = &CTXT_INFO_STRUCT(CTXT_TAG_DIRSTACK);
			assert(info);
			if((i = info->c_nummax) > 0) do {
				putchar('+');
			} while(--i);
#endif
         }
      }
    }
    pr++;
  }
#ifdef FEATURE_ENVVARS_IN_PROMPT
	free(expanded);
#endif
}
