/*
 *  DEL.C - del command.
 *
 *  Comments:
 *
 *  06/29/98 (Rob Lake rlake@cs.mun.ca)
 *      rewrote del to support wildcards
 *      added my name to the contributors
 *
 * 07/13/98 (Rob Lake)
 *  - fixed bug that caused del not to delete file with out attribute.
 *  - moved set, del, ren, and ver to there own files
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 09-Aug-1998 (Rob Lake <rlake@cs.mun.ca>)
 * - changed check for ^C
 * - alloced mem for char **arg
 *
 * 10-Aug-1998 ska
 * - fixed allocation of char **arg incl. out-of-memory check
 *
 * 1998/12/04 ska
 * - chg: vcgetchar() displays a space rather beep()s for no-printable
 *  characters. the caller can beep()
 * - chg: moved vcgetchar() to misc.c as it is multiply used now
 * - chg: use vcgetcstr() instead of vcgetchar() loop
 *
 * 1999/04/27 ska
 * chg: uniform command line parser
 * add: allow multiple patterns on command line
 * add: delete count
 *
 * 1999/07/02 ska
 * chg: replaced stat() by dfnstat() [reduces size of image]
 */

#include "config.h"

#ifdef INCLUDE_CMD_DEL

//#define NODEL

#include <assert.h>
#include <dir.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <ctype.h>
#include <dos.h>
#include <stdlib.h>

#include "dfn.h"

#include "command.h"
#include "cmdline.h"
#include "strings.h"

extern int echo;

/*
 *  simple file delete internal command.
 */

static int optP;

#pragma argsused
optScanFct(opt_del)
{
  switch(ch) {
  case 'P': return optScanBool(optP);
  }
  optErr();
  return E_Useage;
}

#pragma argsused
int cmd_del(char *rest)
{
  int ec = E_None;    /* exit code */
  int i;
  unsigned count = 0;

  struct ffblk f;

  /* Make fullname somewhat larger to ensure that appending
     a matched name, one backslash and one hope. */
  char fullname[MAXPATH + sizeof(f.ff_name) + 2],
   *p, *q;
  int len;

  char **arg;
  int argc, optc;

  /* initialize options */
  optP = 0;

  if((arg = scanCmdline(rest, opt_del, NULL, &argc, &optc)) == NULL)
    return E_Other;

  if(!argc) {
      error_req_param_missing();
      ec = E_Useage;
     }
  else {
    i = 0;
    do {
      assert(arg[i]);

    /* Get the pattern fully-qualified */
    /* Note: An absolute path always contains:
       A:\\
       --> It's always three bytes long at minimum
       and always contains a backslash */
    p = dfnexpand(arg[i], NULL);
    assert(strlen(p) >= 3);
    if ((len = strlen(p)) >= MAXPATH)
    {
      error_filename_too_long(p);
      free(p);
     ec = E_Other;
     goto errRet;
    }
    strcpy(fullname, p);        /* Operating over a local buffer simplifies
                     the process; rather than keep the pattern
                     within dynamic memory */
    free(p);
    p = fullname + len;

    /* check if it is a directory */
    if(dfnstat(fullname) & DFN_DIRECTORY)
    {
      if (p[-1] != '\\')
      *p++ = '\\';
    }

    if (p[-1] == '\\')    /* delete a whole directory */
      p = stpcpy(p, "*.*");

    /* p := address to copy the filename to to form the fully-qualified
     filename */
    /* There is at least one backslash within fullname, because of dfnexpand() */
    while (*--p != '\\') ;
    ++p;

    /* make sure user is sure if all files are to be
     * deleted */
    if (!optP && *p == '*' && ((q = strchr(p, '.')) == NULL || q[1] == '*'))
    {
    displayString(TEXT_MSG_DELETE_ALL);
    if (vcgetcstr("YN\n\r") != 'Y')
    {
      ec = E_Other;
      goto errRet;
    }
    }

    if (FINDFIRST(fullname, &f, FA_ARCH))
    {
    error_sfile_not_found(fullname);
    }
    else do {
    strcpy(p, f.ff_name);       /* Make the full path */

    if (optP)
    {
      printf("%s, Delete(Y/N)?", fullname);
      switch (vcgetcstr("YN\n\r"))
      {
      case '\3':             /* ^Break pressed */
        ec = E_CBreak;
        goto errRet;
      case 'Y':
        break;                /* yes, delete */
      default:
        continue;             /* no, don't delete */
      }
    }
    else if (cbreak) {           /* is also probed for in vcgetstr() */
        ec = E_CBreak;
      goto errRet;
    }

#ifdef NODEL
  /* define NODEL if you want to debug */
    puts(fullname);
#else
    if (unlink(fullname) != 0)
    {
      perror(fullname);   /* notify the user */
    }
    else ++count;
#endif

    } while (FINDNEXT(&f) == 0);
    } while(++i < argc);
  }

errRet:

    if(echo) {
      dispCount(count, "no file", "one file", "%u files");
      puts(" removed.");
    }

    freep(arg);
  return ec;
}

#endif
