/*
 *  INTERNAL.C - command.com internal commands.
 *
 *  Comments:
 *
 *  17/08/94 (Tim Norman)
 *    started.
 *
 *  08/08/95 (Matt Rains)
 *    i have cleaned up the source code. changes now bring this source into
 *    guidelines for recommended programming practice.
 *
 *  cd()
 *    started.
 *
 *  dir()
 *    i have added support for file attributes to the DIR() function. the
 *    routine adds "d" (directory) and "r" (read only) output. files with the
 *    system attribute have the filename converted to lowercase. files with
 *    the hidden attribute are not displayed.
 *
 *    i have added support for directorys. now if the directory attribute is
 *    detected the file size if replaced with the string "<dir>".
 *
 *  ver()
 *    started.
 *
 *  md()
 *    started.
 *
 *  rd()
 *    started.
 *
 *  del()
 *    started.
 *
 *  does not support wildcard selection.
 *
 *  todo: add delete directory support.
 *        add recursive directory delete support.
 *
 *  ren()
 *    started.
 *
 *  does not support wildcard selection.
 *
 *    todo: add rename directory support.
 *
 *  a general structure has been used for the cd, rd and md commands. this
 *  will be better in the long run. it is too hard to maintain such diverse
 *  functions when you are involved in a group project like this.
 *
 *  12/14/95 (Tim Norman)
 *    fixed DIR so that it will stick \*.* if a directory is specified and
 *    that it will stick on .* if a file with no extension is specified or
 *    *.* if it ends in a \
 *
 *  1/6/96 (Tim Norman)
 *    added an isatty call to DIR so it won't prompt for keypresses unless
 *    stdin and stdout are the console.
 *
 *    changed parameters to be mutually consistent to make calling the
 *    functions easier
 *
 *  rem()
 *    started.
 *
 *  doskey()
 *    started.
 *
 *  1/22/96 (Oliver Mueller)
 *    error messages are now handled by perror.
 *
 *  02/05/96 (Tim Norman)
 *    converted all functions to accept first/rest parameters
 *
 *  07/26/96 (Tim Norman)
 *     changed return values to int instead of void
 *
 *  path()
 *     started.
 *
 *  12/23/96 (Aaron Kaufman)
 *     rewrote dir() to mimic MS-DOS's dir
 *
 *  01/28/97 (Tim Norman)
 *     cleaned up Aaron's DIR code
 *
 *  06/13/97 (Tim Norman)
 *     moved DIR code to dir.c
 *     re-implemented Aaron's DIR code
 *
 *  06/14/97 (Steffan Kaiser)
 *     ctrl-break handling
 *     bug fixes
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 *	2000/06/22 ska
 *	add: CDD (implies existance of CHDIR)
 */

#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <dos.h>
#include <direct.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <dir.h>
#include <conio.h>
#include <io.h>
#include <sys/stat.h>

#include "command.h"
#include "strings.h"
#include "cmdline.h"
#include <dfn.h>

/*
 *  generic function to handle cd, md, and rd (and their verbose names)
 *
 *
 */

#if defined(INCLUDE_CMD_CHDIR)	\
 || defined(INCLUDE_CMD_MDDIR)	\
 || defined(INCLUDE_CMD_RMDIR)	\
 || defined(INCLUDE_CMD_CDD)

static int directory_handler(char *rest,
                      int (*func) (const char *), char *fctname)
{ char **argv, *dir;
  int argc, opts;
  int cdd, rv, freeDir;

  assert(func);

  if((argv = scanCmdline(rest, NULL, NULL, &argc, &opts)) == NULL)
    return 1;

	cdd = freeDir = 0;
	rv = 1;

  if(func == (int (*)(const char*))cmd_cdd) {
		func = chdir;
		cdd = 1;
	}

  /* if doing a CD and no parameters given, print out current directory */
  if (func == chdir && argc == 0)
  {
    if((dir = cwd(0)) == NULL)
      goto errRet;
    else {
      puts(dir);
      freeDir = 1;
    }
  } else if(argc != 1) {
    error_req_param_missing();
    goto errRet;
  }
  else {
    assert(argv[0]);
    dir = strchr(argv[0], '\0');
  /* take off trailing \ if any, but ONLY if dir is not the root dir */
    if(dir > &argv[0][1] && *--dir == '\\' && dir[-1] != ':')
      *dir = '\0';

	dir = argv[0];
	if(func == chdir) {
#ifdef FEATURE_LAST_DIR
		static char *lastdir = NULL;

		if(strcmp(dir, "-") == 0) {
			/* change to last directory */
			dir = lastdir;
			freeDir = 1;
		}
		else
			free(lastdir);

		lastdir = cwd(0);
		if(!dir) 	/* "CD -" without a CD before at all */
			goto okRet;
#endif
		if(strlen(dir) >= 2 && dir[1] == ':') {
			if(cdd) {
				if(changeDrive(*dir) != 0)
					goto errRet;
				if(*(dir += 2) == '\0')		/* only change drive */
					goto okRet;
			} else if(!dir[2]) {	/* Real CHDIR displays CWD of specified drive */
				assert(freeDir == 0);
				if((dir = cwd(*dir)) != NULL) {
					puts(dir);
					freeDir = 1;
					goto okRet;
				} else
					goto errRet;
			}
		}
	}
    dprintf(("%s: '%s'\n", fctname, dir));
    if (func(dir) != 0)
    {
      perror(fctname);
      goto errRet;
    }
  }

okRet:
	rv = 0;
errRet:
  freep(argv);
  return rv;
}
#endif

#ifdef INCLUDE_CMD_CHDIR
/*
 * CD / CHDIR - makes a call to directory_handler to do its work
 *
 *
 */
int cmd_chdir(char *rest)
{
  return directory_handler(rest, chdir, "CHDIR");
}
#endif

#ifdef INCLUDE_CMD_CDD
int cmd_cdd(char *rest)
{
  return directory_handler(rest, (int (*)(const char*))cmd_cdd, "CDD");
}
#endif

#ifdef INCLUDE_CMD_MKDIR
/*
 * MD / MKDIR - makes a call to directory_handler to do its work
 *
 *
 */
int cmd_mkdir(char *rest)
{
  return directory_handler(rest, mkdir, "MKDIR");
}
#endif

#ifdef INCLUDE_CMD_RMDIR
/*
 * RD / RMDIR - makes a call to directory_handler to do its work
 *
 *
 */
int cmd_rmdir(char *rest)
{
  return directory_handler(rest, rmdir, "RMDIR");
}
#endif

/*
 * set the exitflag to true
 *
 */
#pragma argsused
int internal_exit(char *rest)
{
  exitflag = 1;

  return 0;
}

#ifdef INCLUDE_CMD_REM
/*
 * does nothing
 *
 */
#pragma argsused
int cmd_rem(char *rest)
{
  return 0;
}
#endif

#ifdef FEATURE_HISTORY
/*
 * prints DOSKEY message...  will soon emulate DOSKEY macros
 *
 */
#pragma argsused
int cmd_doskey(char *rest)
{
  displayString(TEXT_MSG_DOSKEY);
  return 0;
}
#endif
