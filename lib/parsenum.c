/*	$Id$

 *  Parse any number of unsigned integer numbers in the range:
 *    0..9999
 *
 *  Input:  s: String, != NULL
 *    maxCnt: maximum number of numbers to parse
 *    *cnt: receives the number of parsed numbers, != NULL
 *    nums[]: receives the parsed numbers
 *
 *  The string may consists of:
 *  string ::= [whitespaces] [num_list [whitespaces]]
 *  whitespaces ::= whitespace | whitespaces whitespace
 *  whitespace ::= any_character_isspace()_returns_true_for
 *  num_list ::= delimited_numbers [delimiter]
 *  delimited_numbers ::= number | delimited_numbers delimiter number
 *  number ::= digit | number digit
 *  digit ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
 *  delimiter ::= any_character_ispunct()_and_isascii()_are_true_for
 *
 *  There can be at maximum one spurious delimiter behind all the
 *  numbers, except behind the maxCnt'th number. This allows to
 *  type in "half" dates, e.g.: "3.3." for 3rd March (equal to: "3.3");
 *  but this is forbidden for: "3.3.1999.", because maxCnt is equal to
 *  three, when dates are parsed.
 *
 *  Return:
 *    NULL: Fatal error, e.g. number exceeds boundary
 *    else: pointer to first character not parsed

	This file bases on PARSENUM.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

	Revision 1.2  2001/04/29 11:33:51  skaus
	chg: default heap size (tools\ptchsize) set to 6KB
	chg: error displaying functions centralized into lib\err_fcts.src
	add: displayError()
	chg: all errors are displayed through functions void error_*()
	bugfix: somtimes error messages are not displayed (see displayError())
	bugfix: docommand(): type:file must pass ":file" to TYPE
	bugfix: error_sfile(): string _SFILE_
	bugfix: error message on empty redirection
	bugfix: comma and semicolon ';' are recognized as argument seperators
		of internal commands
	
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

#include "../include/misc.h"

char *parsenum(const char *s, int maxCnt, int *cnt, int nums[])
{ int n;

  if(!s || !cnt || !nums || maxCnt <= 0)
    return 0;

  *cnt = 0;

  /* skip leading whitespaces */
  s = ltrimsp(s);

  while(isdigit(*s)) {
    /* scan next number */
    n = 0;
    do {
      if(n >= 10000)
        return 0;
      n = n * 10 + *s - '0';
    } while(isdigit(*++s));
    nums[(*cnt)++] = n;
    if(!isascii(*s) || !ispunct(*s) || !--maxCnt)
      break;
    ++s;      /* skip delimiter */
  }

  return ltrimsp(s);
}
