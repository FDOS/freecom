/*
 *  PROMPT.C - prompt handling.
 *
 *
 *
 *  Comments:
 *
 *  14/01/95 (Tim Normal)
 *    started.
 *
 *  08/08/95 (Matt Rains)
 *    i have cleaned up the source code. changes now bring this source into
 *    guidelines for recommended programming practice.
 *
 *  01/06/96 (Tim Norman)
 *    added day of the week printing (oops, forgot about that!)
 *
 *  08/07/96 (Steffan Kaiser)
 *    small changes for speed
 *
 *  20-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 *   removed redundant day strings. Use ones defined in date.c.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 28-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - moved cmd_prompt from internal.c to here
 *
 * 12-Aug-1998 ska
 * - added SUPPL environment handling (see ENVIRON.C for more details)
 *    Currently a "PROMPT=" will remove the PROMPT variable.
 *    To create an empty one instead, replace chgEnv1() by chgEnv()
 *
 * 2000/06/07 Ron Cemer
 * fix: #include "datefunc.h" if _NO__DOS_DATE is enabled
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <ctype.h>
#include <dir.h>
#include <string.h>
#include <alloc.h>

#include "command.h"
#include "cmdline.h"
#include "datefunc.h"

#define PROMPTVAR "PROMPT"

const char *day_strings[] =
{"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

/*
 * print the command-line prompt
 *
 */
void printprompt(void)
{
  struct time t;
  struct dosdate_t d;

  static char default_pr[] = "$P$G";
  char *pr;
  char direc[128];
  char temp[128];

  dbg_printmem();

  pr = getEnv(PROMPTVAR);        /* get PROMPT environment var. */

  if (!pr || !pr[0])            /* if PROMPT hasn't been set */
  {
    pr = default_pr;
  }

  while (*pr)
  {
    if (*pr != '$')
    {
      putchar(*pr);
    }
    else
    {
      switch (toupper(*++pr))
      {
        case 'Q':
          {
            putchar('=');
            break;
          }
        case '$':
          {
            putchar('$');
            break;
          }
        case 'T':
          {
            gettime(&t);
            printf("%2d:%02d:%02d.%02d", t.ti_hour, t.ti_min, t.ti_sec, t.ti_hund);
            break;
          }
        case 'D':
          {
            _dos_getdate(&d);
            printf("%s %02d-%02d-%04d", day_strings[d.dayofweek], d.month,
                   d.day, d.year);
            break;
          }
        case 'P':
          {
            direc[0] = getdisk() + 'A';
            direc[1] = ':';
            getcurdir(0, temp);

            if (temp[0] == '\\')
            {
              strcpy(&direc[2], temp);
            }
            else
            {
              direc[2] = '\\';
              strcpy(&direc[3], temp);
            }

            fputs(direc, stdout);
            break;
          }
        case 'V':
          {
            printf("%s",shellname);
            break;
          }
        case 'N':
          {
            putchar(getdisk() + 'A');
            break;
          }
        case 'G':
          {
            putchar('>');
            break;
          }
        case 'L':
          {
            putchar('<');
            break;
          }
        case 'B':
          {
            putchar('|');
            break;
          }
        case '_':
          {
            putchar('\n');
            break;
          }
        case 'E':
          {
            putchar(27);
            break;
          }
        case 'H':
          {
            putchar(8);
            break;
          }
      }
    }
    pr++;
  }
}

#ifdef INCLUDE_CMD_PROMPT

/*
 * changes the PROMPT env. var.
 *
 * JPP 17-Jul-1998 - changed to use dynamic memory instead of static.
 *
 */
#pragma argsused
int cmd_prompt(char *rest)
{
  assert(rest);

  if(*rest == '=')    /* skip '=' & spaces */
    rest = ltrim(rest + 1);

  return chgEnv1(PROMPTVAR, rest);
}
#endif
