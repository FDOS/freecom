/*	$id$
	$Locker$	$Name$	$State$

 	Try to create a temporary file in a specified directory
 	with a given extension.
 	On success the name of the temporary file is returned in a
 	dynamically allocated buffer.
 	The extension is simply appended to the randomized filename and
 	must contain the leading dot.
 *
 *  Comments:
 *    Creates unique filenames and creates the files, too.
 *    The filenames do not contain an extension, but my
 *    consume up to eight (8) characters as filename.
 *
 *    The filenames are prefixed by %TEMP%, %TMP%, %TEMPDIR%,
 *    %TMPDIR%, "\\TEMP", "\\TMP", fullpath_of("."), "<bootdrive>:\\",
 *    "."; in that order, whichever returns a writeable location first
 *
 *    The filenames are created within dymamic memory and must be
 *    free()'ed by the caller himself.
 *    The files are touched, meaning they are created with zero
 *    length and no special attributes, but closed; the caller must
 *    remove the file himself.
 *

	This file bases on TMPNAM.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.2  2004/02/01 13:24:22  skaus
	bugfix: misidentifying unspecific failures from within SUPPL

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

#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <dfn.h>

#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/misc.h"

#define nomem error_out_of_memory

/*
 *  Try to create a temporary file with the supplied path, also
 *  fully-qualify it
 *
 *  If ext != NULL, its contents is appended to a randomized prefix,
 *  most useful, if 'ext' begins with a dot, to give the file to be
 *  created an extension.
 */
char *mktempfile(const char * const path, const char *ext)
{
  char *fn, *newpath;
  int cnt,
    fd;

  if (!path)                    /* to simplify the caller function */
    return 0;

  if((fn = abspath(path, 0)) == 0)  /* out-of-mem */
    return 0;

  if (!ext)                     /* make sure it's a legal string */
    ext = "";

  /* The completed name consits of:
     + the absolute path name,
     + the '\\' path component delimited path vs. filename
     + eight (8) characters randomized filename
     + the user-supplied extension
     + the '\0' terminator byte
   */
  if ((newpath = realloc(fn, strlen(fn) + strlen(ext) + 10)) == 0)
  {
    /* out of mem */
    free(fn);
    nomem();
    return 0;
  }

  fn = strchr(newpath, '\0');      /* where the '\\' is to be */
  if (fn[-1] != '\\')           /* maybe 1 byte is wasted here */
    *fn++ = '\\';

  cnt = 0;
  do {             /* randomize filename and probe if it can be created */
    /* OK, this is not that efficient, but easy to implement
       right now -- 1998/10/27 ska */
    sprintf(fn, "CMD%x%s", cnt, ext);
    if (!++cnt)                 /* overflow */
      goto errRet;

    /* loop until either the open succeeded or failed with
       something different then "file already exists"

       However, this failed in Win98 DOSbox on root of
       CD-Rom drive:
       (fd = open(path, O_CREAT | O_EXCL | O_WRONLY
       , S_IREAD | S_IWRITE)) == -1 && errno == EACCES);

       Because DOS returned: EACCES

       Let's assume access() does not fail on directories:
     */
  }
  while(dfnstat(newpath)); /* such entry already exists */

  /* Make semaphore test */
  fd = open(newpath, O_CREAT | O_EXCL | O_WRONLY, S_IREAD | S_IWRITE);
  if (fd != -1)
  {                             /* success */
    close(fd);
    return newpath;
  }

errRet:
  free(newpath);
  return 0;
}
