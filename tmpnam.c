/*
 *  TMPNAM.C - temporary file names
 *
 *  char *tmpfn(void)
 *  char *mktempfile(char *path, char *ext)
 *  void killtmpfn(char *fn);
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
 *  1998/10/27 ska
 *    started.
 *
 * 1999/07/02 ska
 * chg: replaced access() by dfnstat() [reduces size of image]
 *
 */

#include "config.h"

#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <dfn.h>

#include "command.h"

#define nomem error_out_of_memory

/*
 *  Try to create a temporary file with the supplied path, also
 *  fully-qualify it
 *
 *  If ext != NULL, its contents is appended to a randomized prefix,
 *  most useful, if 'ext' begins with a dot, to give the file to be
 *  created an extension.
 */
char *mktempfile(char *path, char *ext)
{
  char *fn;
  int cnt,
    fd;

  if (!path)                    /* to simplify the caller function */
    return 0;

  if ((fn = dfnexpand(path, 0)) == 0)
  {                             /* out-of-mem */
    nomem();
    return 0;
  }

  if (!ext)                     /* make sure it's a legal string */
    ext = "";

  /* The completed name consits of:
     + the absolute path name,
     + the '\\' path component delimited path vs. filename
     + eight (8) characters randomized filename
     + the user-supplied extension
     + the '\0' terminator byte
   */
  if ((path = realloc(fn, strlen(fn) + strlen(ext) + 10)) == 0)
  {
    /* out of mem */
    nomem();
    goto errRet;
  }

  fn = strchr(path, '\0');      /* where the '\\' is to be */
  if (fn[-1] != '\\')           /* maybe 1 byte is wasted here */
    *fn++ = '\\';

  cnt = 0;
  do
  {                             /* randomize filename and probe if it can be created */
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
//  while (access(path, 0) == 0); /* such entry exists */
  while(dfnstat(path)); /* such entry already exists */

  /* Make semaphore test */
  fd = open(path, O_CREAT | O_EXCL | O_WRONLY, S_IREAD | S_IWRITE);
  if (fd != -1)
  {                             /* success */
    close(fd);
    return path;
  }

errRet:
  free(path);
  return 0;
}

#define probefn(path) mktempfile(path, 0)

char *tmpfn(void)
{
  char *fn;                     /* filename */
  static char buf[] = "?:\\";

  /* normally I'd write something like:
     if((pa = getEnv("TEMP")) == 0
     && (pa = getEnv("TMP")) == 0)
     ... )
     pa = ".";
     But if the found 'pa' (path) is not writeble the next
     location has to be probed, therefore the "probing" is
     moved into a sub-function.
   */

  if ((fn = probefn(getEnv("TEMP"))) == 0
      && (fn = probefn(getEnv("TMP"))) == 0
      && (fn = probefn(getEnv("TEMPDIR"))) == 0
      && (fn = probefn(getEnv("TMPDIR"))) == 0
      && (fn = probefn("\\TEMP")) == 0
      && (fn = probefn("\\TMP")) == 0
      && (fn = probefn(".")) == 0)
  {
    /* everything failed --> probe the boot drive */

    _AX = 0x3305;               /* Get Boot drive, DOS 4+ */
    geninterrupt(0x21);
    buf[0] = _CFLAG ? 'C' : _DL + 'A' - 1;
    fn = probefn(buf);          /* If fn is NULL still, no temporary
                                   file can be created */
    if (!fn)
      error_tempfile();
  }

  return fn;
}

/*
 *  Remove the temporary file
 *  also de-allocate the filename
 */
void killtmpfn(char *fn)
{
  if (fn)
  {
    remove(fn);
    free(fn);
  }
}
