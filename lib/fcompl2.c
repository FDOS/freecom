/*	$Id$

	Show files matching a given file completion pattern
	returns 1 if at least one match, else returns 0

	This file bases on FILECOMP.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2006/06/11 02:47:05  blairdude
	Optimized FreeCOM for size, fixed LFN bugs, and started an int 2e handler (which safely fails at the moment)

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

#include <assert.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>

#include "../include/command.h"
#include "../strings.h"

int show_completion_matches(char *str, unsigned charcount)
{
  // varibles found within code
  struct ffblk file;

  int found_dot = 0;
  int curplace = 0;
  int start;
  int count;
  char path[128];
  char fname[14];
  char directory[128];

  assert(str);

  // expand current file name
  count = charcount - 1;
  if (count < 0)
    count = 0;

  while (count > 0 && str[count] != ' ')  // find front of word

    count--;

  if (str[count] == ' ')        // if not at beginning, go forward 1

    count++;

  start = count;

  // extract directory from word
  strcpy(directory, &str[start]);
  curplace = strlen(directory) - 1;
  while (curplace >= 0 && directory[curplace] != '\\' &&
         directory[curplace] != ':')
  {
    directory[curplace] = 0;
    curplace--;
  }

  strcpy(path, &str[start]);

  // look for a . in the filename
  for (count = strlen(directory); path[count] != 0; count++)
    if (path[count] == '.')
    {
      found_dot = 1;
      break;
    }
  if (found_dot)
    strcat(path, "*");
  else
    strcat(path, "*.*");

  curplace = 0;                 // current fname

  if (FINDFIRST(path, &file, FILE_SEARCH_MODE) == 0)
  {                             // find anything

//    putchar('\n');
    write( 1, "\n", 1 );
    count = 0;
    do
    {
      if (file.ff_name[0] == '.') // ignore . and ..

        continue;

      if (file.ff_attrib == FA_DIREC)
        sprintf(fname, "[%s]", file.ff_name);
      else
        strcpy(fname, file.ff_name);

      displayString(TEXT_FILE_COMPLATION_DISPLAY, fname);
      if (++count == 5)
      {
//        putchar('\n');
        write( 1, "\n", 1 );
        count = 0;
      }
    }
    while (FINDNEXT(&file) == 0);

    if (count)
//      putchar('\n');
      write( 1, "\n", 1 );

  }
  else
    /* no match found */
  {
    beep_low();
    return 0;
  }
  return 1;
}
