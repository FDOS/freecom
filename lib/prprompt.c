/*	$id$
	$Locker$	$Name$	$State$

	Parses a string as PROMPT string and display the result onto the screen

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

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
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../include/command.h"
#include "../include/misc.h"


#define PROMPTVAR "PROMPT"

void displayPrompt(const char *pr)
{
  while (*pr)
  {
    if (*pr != '$')
    {
      putchar(*pr);
    }
    else
    {
      switch (toupper(*++pr))
      {
        case 'Q':
          {
            putchar('=');
            break;
          }
        case '$':
          {
            putchar('$');
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
        case 'D':
          {	 char *p;
          	 if((p = curDateLong()) != 0) {
          	 	fputs(p, stdout);
          	 	free(p);
          	 }
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
        case 'V':
          {
            fputs(shellname, stdout);
            break;
          }
        case 'N':
          {
            putchar(getdisk() + 'A');
            break;
          }
        case 'G':
          {
            putchar('>');
            break;
          }
        case 'L':
          {
            putchar('<');
            break;
          }
        case 'B':
          {
            putchar('|');
            break;
          }
        case '_':
          {
            putchar('\n');
            break;
          }
        case 'E':
          {
            putchar(27);
            break;
          }
        case 'H':
          {
            putchar(8);
            break;
          }
      }
    }
    pr++;
  }
}
