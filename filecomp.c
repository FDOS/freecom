/*
 *  FILECOMP.C - handles filename completion
 *
 *
 *  Comments:
 *
 * 30-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - moved from command.c file
 * - made second TAB display list of filename matches
 * - made filename be lower case if last character typed is lower case
 *
 * 30-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - fixed mode of search for filenames to not include volume ID.  This
 *   was causing filename completion not to work under the FD kernel.
 *
 * 1999/04/23 ska
 * bugfix: complete_filename(): makelower undefined if count < 0
 */

#include "config.h"

#ifdef FEATURE_FILENAME_COMPLETION

#include <assert.h>
#include <conio.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>

#include "command.h"

#define FILE_SEARCH_MODE FA_RDONLY | FA_ARCH | FA_DIREC

void goxy(unsigned char x, unsigned char y);

void complete_filename(char *str, unsigned charcount)
{
  // varibles found within code
  struct ffblk file;

  int found_dot = 0;
  int curplace = 0;
  int start;
  int count;
  int perfectmatch = 1;
  int makelower;
  char path[128];
  char fname[14];
  char maxmatch[13] = "";
  char directory[128];

  assert(str);

  // expand current file name
  count = charcount - 1;
  if (count < 0)
    makelower = count = 0;
  else
  {
    // if last character is lower case, then make lookup lower case.
    makelower = islower(str[count]);
  }

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

    do
    {
      if (file.ff_name[0] == '.') // ignore . and ..

        continue;

      strcpy(fname, file.ff_name);
      if (makelower)
        strlwr(fname);

      if (file.ff_attrib == FA_DIREC)
        strcat(fname, "\\");
      else
        strcat(fname, " ");

      if (!maxmatch[0] && perfectmatch)
        strcpy(maxmatch, fname);

      else
      {
        for (count = 0; maxmatch[count] && fname[count]; count++)
          if (maxmatch[count] != fname[count])
          {
            perfectmatch = 0;
            maxmatch[count] = 0;
            break;
          }
      }
    }
    while (FINDNEXT(&file) == 0);

    strcpy(&str[start], directory);
    strcat(&str[start], maxmatch);

    if (!perfectmatch)
      beep();
  }
  else                          /* no match found */
    beep();
}

int show_completion_matches(char *str, unsigned charcount)
// returns 1 if at least one match, else returns 0
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

    putchar('\n');
    count = 0;
    do
    {
      if (file.ff_name[0] == '.') // ignore . and ..

        continue;

      if (file.ff_attrib == FA_DIREC)
        sprintf(fname, "[%s]", file.ff_name);
      else
        strcpy(fname, file.ff_name);

      printf("%-14s", fname);
      if (++count == 5)
      {
        putchar('\n');
        count = 0;
      }
    }
    while (FINDNEXT(&file) == 0);

    if (count)
      putchar('\n');

  }
  else
    /* no match found */
  {
    beep_low();
    return 0;
  }
  return 1;
}
#endif
