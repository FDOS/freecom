/*	$Id$

 *      Check if the passed filename is a CP/M style device
 *
 *      These device names must not contain any drive or path or
 *      extension component, but have a colon ':' at their end.
 *
 *      Return:
 *              0: no
 *              else: yes; fnam contains a valid filename now

 *      To handle CP/M style devices, like "CON:" and "LPT1:", which
 *      are not understood by the DOS kernel directly, these functions
 *      wrap the open() and fopen() functions and deal with it.
 *
 *    Also included are some low-level functions dealing with
 *    file descriptors, such as "isadev()".

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

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

#include <assert.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <string.h>

#include "../include/misc.h"
#include "../include/openf.h"

/*
 *      Just for the algorithm:
 *              One could just transform the "XXXX:" style into
 *              "\dev\XXXX". However, WinNT DOSbox does not support
 *              this as it seems ("copy \dev\con test" says path not found).
 *
 *              On the other hand, how many other DOS clones don't support
 *              the \DEV\ prefix, too?
 *
 *      So I open the device and check the handle if it's connected to
 *      a character device rather than a block device.
 */


int isDeviceName(char *const fnam)
{
  char *p;

  assert(fnam);

  if ((p = strpbrk(fnam, ":\\/")) != 0 /* has a colon or path component */
      && *p == ':'              /* has colon --> no path component */
      && p[1] == '\0'           /* at end of string */
      && p >= fnam + 2)
  {                             /* no simple drive letter */
    int fd;

    *p = '\0';                  /* strip off the colon to make it valid for DOS */

    /* open with read access only to prevent from creating
       disk fiels accidently */
    if ((fd = _open(fnam, O_RDONLY)) >= 0)
    {                           /* open OK */
      int rv;

      /* check for character device */
      rv = isadev(fd);
      close(fd);
      if (rv)
        return 1;               /* is device name */
    }
    *p = ':';                   /* restore the original filename */
  }

  return 0;                     /* no device name */
}
