/*	$Id$

 *	Read a block of data from a FILE* into far memory
 *	Return 0 on failure, otherwise the number of read bytes

	This file bases on MISC.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.3  2006/06/11 02:47:05  blairdude
	Optimized FreeCOM for size, fixed LFN bugs, and started an int 2e handler (which safely fails at the moment)

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
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#include "../include/misc.h"

#if defined(__TURBOC__) || defined(__GNUC__)
static unsigned DOSreadwrite(int fd, void far *buffer, unsigned size,
                             unsigned short func )
{
	IREGS r;

	r.r_ax = func;
	r.r_bx = fd;
	r.r_cx = size;
	r.r_dx = FP_OFF(buffer);
    r.r_ds = FP_SEG(buffer);
	intrpt(0x21, &r);
    return( ( r.r_flags & 1 ) ? 0xFFFF : r.r_ax );
}


size_t farread(int fd, void far*buf, size_t length)
{
	/* Use DOS API in order to read the strings directly to the
		far address */
    return( DOSreadwrite( fd, buf, length, 0x3F00 ) );
}

size_t farwrite(int fd, void far*buf, size_t length)
{
    return( DOSreadwrite( fd, buf, length, 0x3F00 ) );
}
#else
size_t farread(int fd, void far*buf, size_t length)
{
	/* Use DOS API in order to read the strings directly to the
		far address */
	size_t bytes;
	if( _dos_read(fd, buf, length, &bytes) )
		return 0xffff;
	return bytes;
}

size_t farwrite(int fd, void far*buf, size_t length)
{
	size_t bytes;
	if( _dos_write(fd, buf, length, &bytes) )
		return 0xffff;
	return bytes;
}

int sfn_open(const char *pathname, int flags)
{
	int handle;
	int result = _dos_open(pathname, flags,	&handle);
	return (result == 0 ? handle : -1);
}

int sfn_creat(const char *pathname, int attr)
{
	int handle;
	int result = _dos_creat(pathname, attr, &handle);
	return (result == 0 ? handle : -1);
}

int dos_read(int fd, void *buf, unsigned int len)
{
	return farread(fd, buf, len);
}

int dos_write(int fd, const void *buf, unsigned int len)
{
	return farwrite(fd, buf, len);
}
#endif

int dos_creatnew(const char *pathname, int attr)
{
#ifdef __WATCOMC__
	int handle;
	int result = _dos_creatnew(pathname, attr, &handle);
	return (result == 0 ? handle : -1);
#else
	IREGS r;

	r.r_ds = FP_SEG( pathname );
	r.r_dx = FP_OFF( pathname );
	r.r_cx = attr;
	r.r_ax = 0x5B00;

	intrpt( 0x21, &r );

	if( ( r.r_flags & 1 ) ) r.r_ax = 0xFFFF;

	return( r.r_ax );
#endif
}
