/*
 * SET.C
 *
 *  06/14/97 (Tim Norman)
 *     changed static var in set() to a malloc'd space to pass to putenv.
 *     need to find a better way to do this, since it seems it is wasting
 *     memory when variables are redefined.
 *
 *  07/08/1998 (John P. Price)
 *  - removed call to show_environment in set command.
 *  - moved test for syntax before allocating memory in set command.
 *  - misc clean up and optimization.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 28-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added set_env function to set env. variable without needing set command
 *
 * 24-Mar-1999 (John P Price <linux-guru@gcfl.net>)
 * - Changed set command so you can use a space between the variable and
 *   the value.  Prints Syntax error if you don't have a space or a '='.
 *
 */

#include "config.h"

#ifdef INCLUDE_CMD_SET

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <environ.h>

#include "command.h"
#include "cmdline.h"

/*
 *  Helper function for displaying all environment variables
 */
#pragma argsused
static int set_view(void *arg, word segm, word ofs)
{
  printf("%Fs\n", MK_FP(segm, ofs));
  return 0;                     /* don't stop */
}

static int optC;


#pragma argsused
optScanFct(opt_set)
{ switch(ch) {
  case 'C': return optScanBool(optC);
  }
  optErr();
  return E_Useage;
}

/*
 * set environment variables
 *
 *
 */
#pragma argsused
int cmd_set(char *rest)
{ int rv;
  char *p;

  if(!rest)
    rest = "";

  if(leadOptions(&rest, opt_set, NULL) != E_None)
    return 1;

  /* if no parameters, show the environment */
  if (rest[0] == 0)
  {
    env_forAll(0, set_view, 0);
    return 0;
  }

  /* make sure there is an = in the command */
  if ((p = strpbrk(rest, "= ")) == NULL) {
      error_syntax(NULL);
      return 1;
    }

  if (optC) {   /* case-sensitive variable name */
    *p = '\0';
    rv = chgEnv1(rest, p + 1);
  }
  else {
    *p = '=';
    rv = putEnv(rest);
  }

  if (rv)
  {
    *p = '\0';
    error_env(rv, rest);
    return 1;
  }

  return 0;
}
#endif
