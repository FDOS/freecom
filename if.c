/*
 *  IF.C - if command.
 *
 *  Comments:
 *
 * 16 Jul 1998 (Hans B Pufal)
 *   started.
 *
 * 16 Jul 1998 (John P Price)
 *   Seperated commands into individual files.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 1999/11/04 ska
 * + bugfix: if: case-insensitive compare  in "==" format
 * + bugfix: if: added support for quoted operands of "==" format
 * + add: if: detailed error messages
 * + bugfix: if: keyword "EXIST" misspelled
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>

#include "command.h"
#include "cmdline.h"
#include "batch.h"

#pragma argsused
int cmd_if(char *rest)
{
/*
 * Perform IF command.
 *
 */

#define X_EXEC 1

  int x_flag = 0;               /* when set cause 'then' clause to be exec'ed */
  int negate = 0;       /* NOT keyword present */

  char *pp;

  /* First check if rest string begins with word 'not' */
  assert(rest);

  if (matchtok(rest, "not"))
    negate = X_EXEC;            /* Remember 'NOT' */

  /* Check for 'exist' form */

  if (matchtok(rest, "exist"))
  {
      struct ffblk f;

    if (!*rest) {
      /* syntax error */
      error_syntax("IF EXIST: missing filename");
      return 0;
    }

    pp = skipwd(rest);
      *pp++ = '\0';

      if(FINDFIRST(rest, &f, FA_NORMAL) == 0)
      x_flag = X_EXEC;
  }

  /* Check for 'errorlevel' form */

  else if (matchtok(rest, "errorlevel"))
  {
    int
      n = 0;

  if(!isdigit(*rest)) {
    error_syntax("IF ERRORLEVEL: missing number");
    return 0;
  }

    pp = rest;
    do  n = n * 10 + (*pp - '0');
    while (isdigit(*++pp));

    if(*pp && !isspace(*pp)) {
      error_syntax("IF ERRORLEVEL: invalid number");
      return 0;
    }

    if(errorlevel >= n)
      x_flag = X_EXEC;
  }

  /* Check that '==' is present, syntax error if not */
  else {
    size_t len;
    char *r;      /* right operand */

    pp = ltrim(skipqword(rest, "=="));

  if(*pp != '=' || pp[1] != '=') {
    error_syntax(NULL);
    return 0;
  }

    *pp = '\0';     /* rest[] points to the left operand */

      /* skip over the '==' and subsquent spaces and
        assign the end of the right operator to pp */
    pp = skipqword(r = ltrim(pp + 2), NULL);

    /* now: rest := beginning of the left operand
        r := beginning of the right operand
        pp := end of right operand
    */

    rtrim(rest);      /* ensure that spurious whitespaces are ignored */
    len = strlen(rest);

  if((pp - r) == len && memicmp(rest, r, len) == 0) /* strings differ */
    x_flag = X_EXEC;
  }

  if (x_flag ^ negate)
    if(!*(pp = ltrim(pp)))
      error_syntax("IF: missing command");
    else
    parsecommandline(pp);

  return 0;
}
