/*	$id$
	$Locker$	$Name$	$State$

	Parses a date given in ASCIIZ into a dosdate_t structure.

	1 number -> day only (month & year filled with current values)
	2 numbers -> day & month (year filled with current value)
	3 numbers -> day, month,& year

	if datep == NULL, the result is not returned in *datep
		-> thus checking for valid values only

	Return:
		E_None: *datep filled
		E_Empty: empty line
		else: some error

	This file bases on DATE.C of FreeCOM v0.81 beta 1.

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
#include <dos.h>
#include <string.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/nls.h"


static unsigned char months[2][13] =
{
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

int parsedate(const char * s, struct dosdate_t *datep)
{
  struct dosdate_t d;
  unsigned char leap;
  int nums[3],
    items;

  assert(s);

  /* Parse at maximum three numbers */
  s = parsenum(s, 3, &items, nums);

  if (!s || *s)                 /* general error or too many characters */
    return E_Empty;

  _dos_getdate(&d);             /* fetch current info */
#ifdef FEATURE_NLS
	refreshNLS();
	assert(nlsBuf);
	assert(0 <= nlsBuf->datefmt && nlsBuf->datefmt <= 2);
	assert(nlsBuf->dateSep);
	switch(nlsBuf->datefmt) {
	case 0:		/* mm/dd/yy */
#endif
		switch (items) {
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

#ifdef FEATURE_NLS
		break;
	case 1:		/* dd/mm/yy */
		switch (items) {
		case 0:                    /* empty line --> always OK */
		  return 1;

		case 3:                    /* three numbers --> year, month & day */
		  d.year = nums[2];
		  /* fall through */

		case 2:                    /* two numbers --> month & day */
		  d.month = nums[1];
		  /* fall through */

		case 1:
		  d.day = nums[0]; 
		  break;
		}
		break;

	case 2:		/* yy/mm/dd */
		switch (items) {
		case 0:                    /* empty line --> always OK */
		  return 1;

		case 3:                    /* three numbers --> year, month & day */
		  d.year = nums[0];
		  d.month = nums[1];
		  d.day = nums[2]; 
		  break;

		case 2:                    /* two numbers --> month & day */
		  d.month = nums[0];
		  d.day = nums[1]; 
		  break;

		case 1:
		  d.day = nums[0]; 
		  break;
		}
		break;
	}
#endif

  /* if only entered two digits for year, assume 1900's */
  if (d.year < 80)
    d.year += 2000;
  else if (d.year < 200)
    d.year += 1900;

  leap = (!(d.year % 4) && (d.year % 100)) || !(d.year % 400);

  if ((d.month >= 1 && d.month <= 12) &&
      (d.day >= 1 && d.day <= months[leap][d.month]) &&
      (d.year >= 1980 && d.year <= 2099))
  {
    if(datep)
    	memcpy(datep, &d, sizeof(d));
    return E_None;
  }

  return E_Range;
}
