/*
 *  DATE.C - date internal command
 *
 *  Comments:
 *
 *  08 Jul 1998 (John P. Price)
 *    started.
 *
 *  20 Jul 1998 (John P. Price)
 *  - corrected number of days for December from 30 to 31.
 *    (Thanx to Steffen Kaiser for bug report)
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 29-Jul-1998 (Rob Lake)
 * - fixed stand-alone mode.  Added Pacific C compatible dos_getdate functions
 *
 * 1999/03/17 ska
 * bugfix: detection for invalid date strings.
 *  Now the whole string must match: ^\s*\d+(.(\d+(.\d*)?)?)?\s*$
 *  The numbers can be delimited by any character from the 7-bit ASCII set,
 *  which is printable, but not alphanumerical.
 *  One need not specify all three parts (in which case the delimiter
 *  may be omitted, too). If one specify;
 *    1 number --> only the day changes
 *    2 numbers --> month/day; year remains unchanged
 *    3 numbers --> month/day/year
 */

#include "config.h"

#ifdef INCLUDE_CMD_DATE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <ctype.h>
#include <string.h>

#include "command.h"
#include "datefunc.h"
#include "strings.h"
#include "cmdline.h"


static unsigned char months[2][13] =
{
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

#ifdef DEBUG_STANDALONE
const char *day_strings[] =
{
  "Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"
};

#else
extern const char *day_strings[];
#endif


static int noPrompt = 0;

#pragma argsused
optScanFct(opt_date)
{ switch(ch) {
  case 'D':
  case 'T': return optScanBool(noPrompt);
  }
  optErr();
  return E_Useage;
}



int parsedate(char *s)
{
  struct dosdate_t d;
  unsigned char leap;
  int nums[3],
    items;

  assert(s);

  /* Parse at maximum three numbers */
  s = parsenum(s, 3, &items, nums);

  if (!s || *s)                 /* general error or too many characters */
    return 0;

  _dos_getdate(&d);             /* fetch current info */
  switch (items)
  {
    case 0:                    /* empty line --> always OK */
      return 1;

    case 1:                    /* single number --> day only */
      d.day = nums[0];
      break;

    case 3:                    /* three numbers --> year, month & day */
      d.year = nums[2];
      /* fall through */

    case 2:                    /* two numbers --> month & day */
      d.day = nums[1], d.month = nums[0];
      break;
  }

  /* if only entered two digits for year, assume 1900's */
  if (d.year < 80)
    d.year += 2000;
  else if (d.year < 100)
    d.year += 1900;

  leap = (!(d.year % 4) && (d.year % 100)) || !(d.year % 400);

  if ((d.month >= 1 && d.month <= 12) &&
      (d.day >= 1 && d.day <= months[leap][d.month]) &&
      (d.year >= 1980 && d.year <= 2099))
  {
    _dos_setdate(&d);
    return 1;
  }

  return 0;
}

#pragma argsused
int cmd_date(char *rest)
{
  char s[40];
  int ec;

  noPrompt = 0;

  if((ec = leadOptions(&rest, opt_date, NULL)) != E_None)
      return ec;

  if (!*rest)
  {
    struct dosdate_t d;

    _dos_getdate(&d);

    displayString(TEXT_MSG_CURRENT_DATE,
                   day_strings[d.dayofweek], d.month, d.day, d.year);

    rest = NULL;
  }

  while (1)                     /*forever loop */

  {
    if (!rest)
    {
		if(noPrompt) return 0;

      if ((rest = getMessage(TEXT_MSG_ENTER_DATE)) == NULL)
        return 1;               /* failed, error message on screan already */

      fputs(rest, stdout);      /* put onto screen */
      free(rest);
      fgets(s, sizeof(s), stdin);
      if (cbreak)
        return 1;
      if (parsedate(s))
        return 0;
    }
    else
    {
      if (parsedate(rest))
        return 0;
    }
    displayString(TEXT_ERROR_INVALID_DATE);
    // force input the next time around.
    rest = NULL;
  }
}
#endif

#ifdef DEBUG_STANDALONE
int main(void)
{
  cmd_date("date", "");
  return 1;
}
#endif
