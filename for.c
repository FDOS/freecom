/*
 *  FOR.C - for command.
 *
 *  Comments:
 *
 * 16 Jul 1998 (Hans B Pufal)
 *   started.
 *
 * 16 Jul 1998 (John P Price)
 *   Seperated commands into individual files.
 *
 * 19 Jul 1998 (Hans B Pufal) [HBP_001]
 *   Implementation of FOR
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 10-Aug-1998 ska
 * - added malloc() checking
 *
 * 1999/04/23 ska
 * bugfix: cmd_for(): missing out-of-memory check with batch_params()
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "command.h"
#include "batch.h"
#include "cmdline.h"
#include "strings.h"

#pragma argsused
int cmd_for(char *rest)
{
  /*
   * Perform FOR command.
   *
   * First check syntax is correct : FOR %v IN ( <list> ) DO <command>
   *   v must be alphabetic, <command> must not be empty.
   *
   * If all is correct build a new bcontext structure which preserves
   *   the necessary information so that readbatchline can expand
   *   each the command prototype for each list element.
   *
   * You might look on a FOR as being a called batch file with one line
   *   per list element.
   */

  char *pp;
  char var;

  assert(rest);

  /* Check that first element is % then an alpha char followed by space */

  if (*rest != '%' || !isalpha(rest[1]) || !isspace(rest[2]))
  {
    displayString(TEXT_ERROR_BAD_VERABLE);
    return 1;
  }

  var = rest[1];               /* Save FOR var name */
  rest = ltrim(rest + 2);   /* skip whitespaces */

  /* Check next element is 'IN' */

  if (!matchtok(rest, "in"))
  {
    displayString(TEXT_ERROR_IN_MISSING);
    return 1;
  }

  /* Folowed by a '(', find also matching ')' */

  if (*rest != '(' || 0 == (pp = strchr(rest, ')')))
  {
    displayString(TEXT_ERROR_MISSING_PARENTHESES);
    return 1;
  }

  *pp = '\0';
  rest++;                      /* rest now points at null terminated list */

  pp = ltrim(pp + 1);

  /* Check DO follows */

  if (!matchtok(pp, "do"))
  {
    displayString(TEXT_ERROR_DO_MISSING);
    return 1;
  }

  /* Check that command tail is not empty */

  if (*pp == '\0')
  {
    displayString(TEXT_ERROR_NO_COMMAND_AFTER_DO);
    return 1;
  }

  /* OK all is correct, build a bcontext.... */

  {
    struct bcontext
     *new = newBatchContext();

    if (!new)
      return 1;

    if ((bc->forproto = strdup(pp)) == 0)
    {
      error_out_of_memory();
      exit_batch();   /* remove the newly created batch context */
      return 1;
    }

    if(!setBatchParams(rest)) { /* Split out list */
      exit_batch();
      return 1;
    }

    bc->forvar = var;
    bc->shiftlevel = 1;     /* skip %0 <=> filename */
  }

  return 0;
}
