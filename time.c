/*
 *  TIME.C - time internal command
 *
 *  Comments:
 *
 *  07/08/1998 (John P. Price)
 *    started.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 1999/03/17 ska
 * bugfix: Detection of invalid time strings
 *  One can specify:
 *    ^\s*\d+.\d+(.(\d+(.\d*)?)?)?\s*([aApP]([mM]|\.[mM]\.)?\s*$
 *  If one specifies:
 *    1 number --> error
 *    2 numbers --> hour:minute, seconds & hundreds default to zero
 *    3 numbers --> hour:minute:seconds, hundreds defaults to zero
 *    4 numbers --> hour:minute:seconds.hundreds
 *  The numbers may be delimited by any character from the 7-bit ASCII set,
 *  which is printable, but not alphanumerical.
 */

#include "config.h"

#ifdef INCLUDE_CMD_TIME

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <ctype.h>
#include <string.h>

#include "command.h"
#include "timefunc.h"
#include "strings.h"

int parsetime(char *s)
{
  struct dostime_t t;
  int nums[4],
    items;
  int pm;

  assert(s);

  memset(nums, 0, sizeof(nums));  /* let default everything to zero */

  /* Parse at maximum three numbers */
  s = parsenum(s, 4, &items, nums);

  if (!s)                       /* general error */
    return 0;

  /* 12 hour time format? */
  pm = 0;                       /* no such flag */
  switch (*strupr(s))
  {
    case 'P':
      ++pm;                     /* post meridian */
    case 'A':
      ++pm;                     /* ante meridian */
      /* strip ?M or ?.M. */
      if (s[1] == 'M')
        ++s;
      else if (memcmp(s + 1, ".M.", 3) == 0)
        s += 3;
      /* strip whitespaces */
      while (isspace(*++s)) ;   /* skip whitespaces */
  }
  if (*s)
    return 0;                   /* too many characters on line */

  switch (items)
  {
    case 0:                    /* empty line --> always OK */
      return 1;

    case 1:                    /* single number --> error */
      return 0;

#if 0
    default:                   /* four numbers --> 1/100s, s, min, h */
      /* else: 1/100s and/or s default to zero */
      break;
#endif
  }

  t.hour = nums[0];
  t.minute = nums[1];
  t.second = nums[2];
  t.hsecond = nums[3];

  switch (pm)
  {
    case 2:                    /* post meridian */
      t.hour += 12;
      break;
    case 1:                    /* antes meridian */
      if (t.hour == 12)
        t.hour = 0;
      break;
      /* default:   not specified --> nothing to do */
  }

  if (t.hour > 23 || t.minute >= 60 || t.second >= 60 || t.hsecond > 99)
    return 0;

  _dos_settime(&t);
  return 1;
}

#pragma argsused
int cmd_time(char *rest)
{
  struct dostime_t t;
  char ampm;
  char s[40];

  if (!rest || !*rest)
  {
    _dos_gettime(&t);

    if (t.hour > 12)
    {
      ampm = 'p';
      t.hour -= 12;
    }
    else
    {
      ampm = 'a';
      if (t.hour == 0)
        t.hour += 12;
    }

    displayString(TEXT_MSG_CURRENT_TIME, t.hour, t.minute,
                   t.second, t.hsecond, ampm);
    rest = NULL;
  }

  while (1)
  {
    if (rest)
    {
      if (parsetime(rest))
        return 0;
    } else {
      if ((rest = getMessage(TEXT_MSG_ENTER_TIME)) == NULL)
        return 1;

      fputs(rest, stdout);
      free(rest);
      fgets(s, sizeof(s), stdin);
      if (cbreak)
        return 1;
      if (parsetime(s))
        return 0;
    }
    displayString(TEXT_ERROR_INVALID_TIME);
    // force input the next time around.
    rest = NULL;
  }
}

#endif

#ifdef DEBUG_STANDALONE
int main(void)
{
  cmd_time("time", "");
  return 1;
}
#endif
