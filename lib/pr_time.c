/*	$id$
	$Locker$	$Name$	$State$

	parsetime() parses the ASCII representation of a time into
	a dostime_t

 *  One can specify:
 *    ^\s*\d+.\d+(.(\d+(.\d*)?)?)?\s*([aApP]([mM]|\.[mM]\.)?\s*$
 *  If one specifies:
 *    1 number --> error
 *    2 numbers --> hour:minute, seconds & hundreds default to zero
 *    3 numbers --> hour:minute:seconds, hundreds defaults to zero
 *    4 numbers --> hour:minute:seconds.hundreds
 *  The numbers may be delimited by any character from the 7-bit ASCII set,
 *  which is printable, but not alphanumerical.

	This file bases on TIME.C of FreeCOM v0.81 beta 1.

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
#include <string.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/nls.h"


int parsetime(const char *s, struct dostime_t * const timep)
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
    return E_Syntax;

  /* 12 hour time format? */
  pm = 0;                       /* no such flag */
  switch(toupper(*s))
  {
    case 'P':
      ++pm;                     /* post meridian */
    case 'A':
      ++pm;                     /* ante meridian */
      /* strip ?M or ?.M. */
      if(toupper(s[1]) == 'M')
      	s += 2;
      else if(memicmp(s + 1, ".M.", 3) == 0)
        s += 4;
  }
  if (*ltrim(s))
    return E_Syntax;            /* too many characters on line */

  switch (items)
  {
    case 0:                    /* empty line --> always OK */
      return E_Empty;

    case 1:                    /* single number --> error */
      return E_Useage;

#if 0
    default:                   /* four numbers --> 1/100s, s, min, h */
      /* else: 1/100s and/or s default to zero */
      break;
#endif
  }

  t.hour = nums[0];
  t.minute = nums[1];
  t.second = nums[2];		/* if missing defaults to previously set */
  t.hsecond = nums[3];		/* values, aka 0 */

  switch (pm)
  {
    case 2:                    /* post meridian */
      if(t.hour != 12) t.hour += 12;
      break;
    case 1:                    /* antes meridian */
      if (t.hour == 12)
        t.hour = 0;
      break;
      /* default:   not specified --> nothing to do */
  }

  if (t.hour >= 24 || t.minute >= 60 || t.second >= 60 || t.hsecond > 99)
    return E_Range;

  if(timep)
  	memcpy(timep, &t, sizeof(t));
  return E_None;
}
