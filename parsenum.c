/*
 *  PARSENUM.C - Parse delimited numbers
 *
 *  Currently used by DATE & TIME
 *
 *  1999/03/17 ska
 *    started.
 */

#include "config.h"

#if defined(INCLUDE_CMD_DATE) || defined(INCLUDE_CMD_TIME)

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "command.h"
#include "cmdline.h"

/*
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
 */
char *parsenum(char *s, int maxCnt, int *cnt, int nums[])
{ int n;

  if(!s || !cnt || !nums || maxCnt <= 0)
    return NULL;

  *cnt = 0;

  /* skip leading whitespaces */
  s = ltrim(s);

  while(isdigit(*s)) {
    /* scan next number */
    n = 0;
    do {
      if(n >= 10000)
        return NULL;
      n = n * 10 + *s - '0';
    } while(isdigit(*++s));
    nums[(*cnt)++] = n;
    if(!isascii(*s) || !ispunct(*s) || !--maxCnt)
      break;
    ++s;      /* skip delimiter */
  }

  return ltrim(s);
}

#endif
