/*
 *  ENVIRON.C - enviroment handling.
 *
 *
 *
 *  Comments:
 *
 *  01/14/94 (Tim Norman)
 *    started.
 *
 *  08/08/95 (Matt Rains)
 *    i have cleaned up the source code. changes now bring this source into
 *    guidelines for recommended programming practice.
 *
 *  07/08/1998 (John P. Price)
 *  - commented out show_environment function.  Not used anymore.
 *
 *  1998/08/10  ska
 *  - added SUPPL's environment function & their wrappers putEnv(), getEnv()
 *    Note: both putEnv() and getEnv() may alter the variable name to
 *    reflect the case of the variable actually processed
 *  - moved cmd_set() here, because it also uses SUPPL functions
 *  - added to SET: "/C" Case-sensitive variable name (no uppercasing)
 *  - added to SET: no '=' in string will act as ECHO %string%, but
 *    will display an error, if the variable ('string') was not found
 *  - added CONFIG.H: Note the putEnv() / getEnv() still remains _active_
 *    however, PROMPT and the initialization code depend on set_env() as
 *    they add COMSPEC and PROMPT (and PATH) using them.
 *  - added: chgEnv() & chgEnv1(). as putEnv(), but case-sensitive
 *    variable names & different parameter list. chgEnv1() does
 *    not allow empty values (deletes variable instead).
 * - currently only cmd_set() uses putEnv(), so it's also enclosed
 *  within the #ifdef INCLUDE_CMD_SET.
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "environ.h"

/*
 *  Return the value of the variable
 *
 *  The value will be overwritten the next time, getEnv() is called,
 *  because it will be duplicated into dynamic memory fromout the
 *  environment segment.
 *
 *  If the case-insensitive match is used, the var[] array is updated with
 *  the variable name that was retrieved.
 *
 *  Return: NULL, on failure
 */
char *getEnv(char var[])
{
  static char *lastVal = 0;

  assert(var);

  /* To have case-sensitive variable names, just delete the
     env_matchVar() call and the if() */
  /* To let var[] unaltered, use env_findAnyVar() instead of the
     combination of matchVar() & dupvar().
     However, the caller will know the correct variable name, if
     you don't. */
  if (env_matchVar(0, var) & 7) /* found? */
    return 0;                /* no match found */

  /* var now contains the correct variable name and we can be
     sure that's there */
  free(lastVal);                /* paranoids would probe on NULL -- ska */

  return lastVal = dupvar(var);
}

/*
 *  Insert/replace/delete a case-sensitive variable
 *
 *  If value == NULL, the variable is deleted.
 *  If value == "", a variable with an empry body is created.
 *  If name == "", a body with an empty name is created.
 *
 *  Return: 0 on success;; !0 on failure
 */
chgEnv(const char name[], const char value[])
{
  int rv;

  assert(name);

  /* return values 1 and 3 are OK */
  switch(rv = env_change(0, name, value)) {
  case 1: case 3:   /* var replace | inserted ==> OK */
    return 0;
  case 0:       /* Cannot insert */
    return -1;
  }
    /* -2: Variable not found
      -4: no environment
      1: syntax error */
  return -rv;
}

/*  as chgEnv(), but value == "" deletes variable, too */
chgEnv1(const char name[], const char value[])
{
  return chgEnv(name, *value ? value : 0);
}

#ifdef INCLUDE_CMD_SET
/*
 *  Insert/replace/delete an environment variable
 *
 *  If the variable already exists within the environment,
 *  its case is retained {So one can modify "winbootdir", for
 *  example}. If the variable shall be added to the environment,
 *  but is not (in any case-insensitive match) contained within the
 *  environment, the name is uppercased.
 *
 *  Return: 0 on success
 *      !0 on failure
 */
int putEnv(char var[])
{
  char *sign;
  int rv;

  assert(var);

  if ((sign = strchr(var, '=')) == 0)
    return -1;

  *sign = '\0';                 /* delimit varname from varvalue */
  if (env_matchVar(0, var) & 7) /* no match --> upcase variable */
    strupr(var);

  rv = chgEnv1(var, sign + 1);
  *sign = '=';
  return rv;
}

#endif
