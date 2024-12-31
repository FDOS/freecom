/*	$id$
	$Locker$	$Name$	$State$

 *  RES_R.C - Read resource
 *
 *	Contains all functions to encapsule to "read" resources.
 *
 *	int enumFileResources(fnam, res_majorid_t, int (*fct)(), void * arg)
 *		Searches for all resources of the specified major ID in the
 *		specified file.
 *		The callback function is called like this:
 *			int fct(res_majorid_t, res_minorid_t, length, FILE*, arg)
 *		The "length" parameter specifies the length of the data
 *		block (unsigned long), the file is the resource file opened
 *		in "rb" modus, "arg" is the pointer passed to enumREsources().
 *		The callback function may reposition the file, though, the file
 *		itself must not be closed.
 *
 *		If the callback function returns 0 (zero), the loop continues,
 *		otherwise enumResource() is terminated and this value is returned.
 *
 *	Return:
 *		0: Nothing found or callback function always returned "0" itself
 *		else: return value of callback function
 *		-1: file open error
 *

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.8  2006/09/11 00:07:22  blairdude
	Fixed compilation completely with Turbo C

	Revision 1.7  2006/09/04 20:03:14  blairdude
	Support compiler oddity (Arkady)
	
	Revision 1.6  2004/08/18 17:18:10  skaus
	chg: res_r.c: don#('t use relative seek; fails under NTFSdos [Tom Ehlert]
	
	Revision 1.4  2002/11/05 19:34:38  skaus
	chg: enumResources(): handle fnam == 0 (file open error)
	
	Revision 1.3  2001/07/27 22:37:35  skaus
	bugfix: pre-compiled package BINARY.ZIP without command-line editing
	chg: FreeCOM archive (== executable) is opened read-only.
	
	Revision 1.2  2001/06/10 15:20:39  skaus
	add: error message if debugging when to open the resource file failed
	
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
	
	TE 14 may 2004: there was some bug wrt NTFS4DOS, fixed.
	                unwound this beautiful C puzzle to do this
 */

#include "../config.h"

#include <assert.h>
#include <string.h>	/* memcmp */
#include <fcntl.h>
#if !defined(__GNUC__) && !defined(__WATCOMC__) || !defined(PTCHSIZE) || defined(__MINGW32__)
#include <io.h>		/* filelength */
#endif

#include "../include/resource.h"
#ifdef PTCHSIZE
#define dos_open open
#define dos_read read
#define dos_close close
#if defined(__GNUC__) && !defined(__MINGW32__)
#ifndef O_BINARY
#define O_BINARY 0
#endif
#include <sys/types.h>
#include <unistd.h>
static long int filelength(int fd)
{
  long int cur = lseek(fd, 0, SEEK_CUR);
  long int fsize = lseek(fd, 0, SEEK_END);
  lseek(fd, cur, SEEK_SET);
  return fsize;
}
#elif defined(__WATCOMC__)
#include <sys/types.h>
#include <unistd.h>
#endif
#else
#include "../include/misc.h"
#endif

int enumFileResources(const char *const fnam
	, res_majorid_t id
	, res_callbackp_t fct
	, void * const arg)
{	resource_t res;
	unsigned long pos;
	int rc;
	int fd;

	assert(fct);
	if(!fnam)
		return -1;

	rc = 0;
#ifdef PTCHSIZE
	#define OMODE O_RDONLY | O_BINARY
#else
	#define OMODE O_RDONLY
#endif
	if((fd = dos_open(fnam, OMODE)) < 0) {
	 	rc = -1;
	 	dprintf(("[RES: Failed to open file: %s]\n", fnam));
#ifdef DEBUG
		perror("Open");
#endif
		/* resource file could be opened
			The first resource item is located at the end of the file,
			the var #("pos#(" is used to indicate the absolute position
			within the file as relative positioning is failing under
			NTFSdos {Tom Ehlert} */
	} else if((pos = filelength(fd)) != (unsigned long)-1L)
   	  while (1) {
		if(pos <= sizeof(res)				/* file corruption */
		 		/* read and verify the resource ID block */
		 || lseek(fd, pos -= sizeof(res), SEEK_SET) < 0		/* seek error */
	     || dos_read(fd, &res, sizeof(res)) != sizeof(res)		/* read error */
		 					/* file corruption as magic string is missing */
		 || memcmp(res.res_cookie, RES_COOKIE, sizeof(res.res_cookie)) != 0
		 		/* file corruption: not that many bytes left in file */
		 || pos < res.res_length
		 		/* failed to position to data block in the file */
		 || lseek(fd, pos -= res.res_length, SEEK_SET) < 0
		)
		 	break;
		 
		 if (res.res_majorID == id || id == RES_ID_ANY)
		 	if ((rc = fct(res.res_majorID
				 , res.res_minorID
				 , res.res_length
				 , fd
				 , arg)) != 0)
		    break;
	}  

	dos_close(fd);
	return rc;
}
