/*
 *  HISTORY.C - command line history.
 *
 *
 *
 *  Comments:
 *
 *  14/01/95 (Tim Norman)
 *    started.
 *
 *  08/08/95 (Matt Rains)
 *    i have cleaned up the source code. changes now bring this source into
 *    guidelines for recommended programming practice.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 */

#include "config.h"

#ifdef FEATURE_HISTORY

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "strings.h"

#define MAXLINES 64

unsigned history_size = 256;    /* make this configurable later */

char *historyptr = NULL;
unsigned int curline = 0;
unsigned int numlines = 0;
unsigned int maxpos = 0;


void history(int dir, char *commandline)
{
  static char *lines[MAXLINES];
  int count;
  int length;

  assert(commandline);

  if (!historyptr)
  {
    historyptr = malloc(history_size * sizeof(char));
    lines[0] = historyptr;
    historyptr[0] = 0;
  }

  if (dir > 0)                  /* next command */
  {
    if (curline < numlines)
      curline++;

    if (curline == numlines)
      commandline[0] = 0;
    else
      strcpy(commandline, lines[curline]);
  }
  else if (dir < 0)             /* prev command */
  {
    if (curline > 0)
      curline--;

    strcpy(commandline, lines[curline]);
  }
  else
    /* add to history */
  {
    /* remove oldest string until there's enough room for next one */
    /* strlen (commandline) must be less than history_size! */
    while (maxpos + strlen(commandline) + 1 > history_size || numlines >= MAXLINES)
    {
      length = strlen(lines[0]) + 1;

      for (count = 0; count < maxpos && count + (lines[1] - lines[0]) < history_size; count++)
        historyptr[count] = historyptr[count + length];

      maxpos -= length;

      for (count = 0; count <= numlines && count < MAXLINES; count++)
        lines[count] = lines[count + 1] - length;

      numlines--;
    }

    strcpy(lines[numlines], commandline);
    numlines++;
    lines[numlines] = lines[numlines - 1] + strlen(commandline) + 1;
    maxpos += strlen(commandline) + 1;
    curline = numlines;         /* last line, empty */
  }
}

/*
 * history command
 *
 */
#pragma argsused
int cmd_history(char *rest)
{
  unsigned x;

  if (rest && *rest)
  {
    x = atoi(rest);
    if (x < 256 || x > 32768U)
    {
      displayString(TEXT_ERROR_HISTORY_SIZE);
      return 1;
    }

    history_size = x;

    free(historyptr);
    historyptr = NULL;
    curline = 0;
    numlines = 0;
    maxpos = 0;
  }
  else
    displayString(TEXT_MSG_HISTORY_SIZE, history_size);

  return 0;
}
#endif
