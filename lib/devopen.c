/*	$Id$

 	like open() supporting device names

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2006/06/19 04:12:46  blairdude
	File redirection now supports long filenames, bug fixed in __creat_or_truncate

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
#include <io.h>
#include <fcntl.h>
#include <dos.h>

#include "../include/openf.h"
#include "../include/misc.h"
#include "../include/lfnfuncs.h"

int devopen(char *const fnam, int mode)
{
  int fd;

  isDeviceName(fnam);           /* modify fnam if device */
  fd = -1;
  if((mode & (O_CREAT|O_TRUNC)) != (O_CREAT|O_TRUNC))
    fd = dos_open(fnam, mode & ~(O_APPEND|O_CREAT|O_TRUNC));
  if(fd == -1 && (mode & O_CREAT))
    return dos_creat(fnam, 0);
  if (mode & O_APPEND)
    lseek(fd, 0, SEEK_END);
  return fd;
}
