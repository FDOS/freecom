/*	$id$
	$Locker$	$Name$	$State$

	make a filename of a temporary file.
	For consistency (atomic operation) the file is created as a
	zero-length file without special file permissions.
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
	Revision 1.1.4.1  2001/06/21 21:40:35  skaus
	Update #2

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

#include <portable.h>

#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/misc.h"

#define probefn(path) mktempfile(path, ext)

char *tmpfn(void)
{	return tmpefn(0);
}

char *tmpefn(const char * const ext)
{	USEREGS
  char *fn;                     /* filename */
  static char buf[] = "?:\\";

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
