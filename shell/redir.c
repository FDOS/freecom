/*
 * REDIR.C
 *
 * Comments:
 *
 * 12/15/95 (Tim Norman)
 * started.
 *
 * 12 Jul 98 (Hans B Pufal)
 * - Rewrote to make more efficient and to conform to new command.c and
 *   batch.c processing.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 1998/10/27 ska
 * - bugfix: get_redirection() failed for non-paired quotes
 * - changed: joined to loop for checking for pipes & redirections
 * - changed: allocate the filename of redirection here to make the
 *      allocated space fit tightly
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/command.h"
#include "../err_fcts.h"

static int is_redir(char c)
{
  return (c == '<') || (c == '>') || (c == '|');
}

int get_redirection(char *s, char **ifn, char **ofn, int *ofatt)
{
  /*
   * Gets the redirection info from the command line and copies the
   * file names into ifn and ofn removing them from the command line.
   * The names are allocated here and passed back to the caller, on
   * malloc() failure, -1 is returned. These names are trimmed,
   * meaning they do not contain any leading or trailing whitespaces.
   *
   * Converts remaining command line into a series of null terminated
   * strings defined by the pipe char '|'. Each string corresponds
   * to a single executable command. A double null terminates the
   * command strings.
   *
   * Check for, but do not implement, output append redirect.
   *
   * Return number of command strings found.
   *
   */

  int num = 1;
  int ch;

  char *dp = s;
  char *sp = s;

  assert(s);
  assert(ifn);
  assert(ofn);
  assert(ofatt);

  /* find and remove all the redirections first */

  while ((ch = *dp++ = *sp++) != 0)
    switch (ch)
    {
      case '"':
      case '\'':               /* No redirects inside quotes */
        {
          char *p;
          int len;

          /* If there is no closing quote, then go to end of line. */
          if ((p = strchr(sp, ch)) == 0)
          {
            p = sp + strlen(sp) - 1;
          }

          /* closing quote found, move that area */
          /* need memmove() because both areas overlap each other */
          memmove(dp, sp, len = p - sp + 1);
          dp += len;
          sp += len;

        }
        break;

      case '<':
      case '>':
        {
          /* MS-DOS ignores multiple redirection symbols and uses the last */
          /* redirection, so we'll emulate that and not check */

          char **op = (ch == '<') ? ifn : ofn;
          char *p;

          if ((ch == '>') && (*sp == '>'))      /* Append request ? */
          {
            *ofatt = O_CREAT | O_APPEND | O_TEXT | O_WRONLY;
            sp++;
          }

          while (isspace(*sp))
            sp++;

          p = sp--;             /* start of filename */
          while (*++sp && !is_redir(*sp) && !isspace(*sp)) ;
          free(*op);            /* ignore any previous one */
          ch = *sp;
          *sp = '\0';
          if ((*op = strdup(p)) == 0)
          {                     /* out of mem */
            error_out_of_memory();
            return -1;
          }

          *sp = ch;
          --dp;                 /* ignore the already copied '<' or '>' */
        }
        break;

      case '|':

        dp[-1] = '\0';          /* overwrite the already copied '|' */
        ++num;
        break;

    }                           /* end switch */

  return num;
}
