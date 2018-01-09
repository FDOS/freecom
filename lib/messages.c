/*	$Id$

	msgSegment() - return the segment of memory the STRINGS have been
		loaded to; if they are not present in memory, they are loaded
		from the resources
	unloadMsgs() - deallocate the loaded STRINGS segment

	This file bases on MESSAGES.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.8  2006/09/11 00:07:22  blairdude
	Fixed compilation completely with Turbo C

	Revision 1.7  2004/06/29 21:57:20  skaus
	fix: /LOW option
	
	Revision 1.6  2004/06/29 14:14:56  skaus
	fix: help screen of internal commands causes "Unknown command error" {Bernd Blaauw}
	
	Revision 1.5  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.4  2003/12/09 21:29:24  skaus
	bugfix: Ask for FreeCOM location when STRINGS are missing [#687]
	
	Revision 1.3  2002/04/02 23:36:37  skaus
	add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
	
	Revision 1.2  2002/04/02 18:09:31  skaus
	add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
	
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include <suppl.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/resource.h"
#include "../include/res.h"
#include "../strings.h"
#include "../include/strings.typ"
#include "../include/cswap.h"

unsigned msgSegm = 0;                   /* strings segment if loaded */
string_count_t strCnt = 0;		/* number of strings */

/* Remove the string segment from memory */
void unloadMsgs(void) 
{
  if(msgSegm) {
    freeBlk(msgSegm);
    dprintf( ("[Message segment 0x%04x deallocated.]\n", msgSegm) );
  }
  msgSegm = 0;
}

/* Number of bytes of preceeding the index array:
	ID string
	+ trailer (\r\n\x1a\0)
	+ count of entries
	+ size of data portion
*/
#define STRINGS_HEADER_SIZE sizeof(STRINGS_ID) + STRINGS_ID_TRAILER	\
	+ sizeof(string_size_t) + sizeof(string_count_t) - 1

/* Callback function for enumResources() */
/* Called if the resource locator found a resource with a matching
	major ID --> we ignore all the rest of IDs and probe for the
	validation string at the beginning of the resource data */

static int loadStrings (res_majorid_t major,
		        res_minorid_t minor,
		        unsigned long length,
			int fd,
			void *const arg) {
	loadStatus *ls = arg;
	char fdid[sizeof(STRINGS_ID)];
	string_size_t len, firstStr;
	string_index_t far*idx;
	int i, error;

	(void)major, (void)minor;
	if((unsigned long)length >= 0x10000ul
	 || (unsigned)length < STRINGS_HEADER_SIZE) {
		*ls = STRINGS_SIZE_MISMATCH;
		return 0;
	}

	error = 0;
	if(dos_read(fd, fdid, sizeof(STRINGS_ID) - 1) != sizeof(STRINGS_ID) - 1)
		error = 1;

	if (memcmp(fdid, STRINGS_ID, sizeof(STRINGS_ID) - 1)) {
		*ls = STRINGS_ID_MISMATCH;
		return 0;			/* Continue searching */
	}
	/* immediately after the ID a trailer follows */
	if(lseek(fd, (long)STRINGS_ID_TRAILER, 1) < 0)
		error = 1;

		/* Read the strings dimensionating parameters */
	if(error
	 || dos_read(fd, &strCnt, sizeof(strCnt)) != sizeof(strCnt)
	 || dos_read(fd, &len, sizeof(len)) != sizeof(len)) { /* Read error */
	 	*ls = STRINGS_READ_ERROR;
	 	return 0;			/* Continue searching */
	}
	/* At this point f is positioned at the very first string index
	 the data area is NUMBER_OF_STRINGS * sizeof(index) +
	 SIZE_OF_STRINGS   */
	len += firstStr = strCnt * sizeof(string_index_t);
	if((unsigned)length - STRINGS_HEADER_SIZE < len) {
		*ls = STRINGS_SIZE_MISMATCH;
		return 0;
	}
		/* allocation mode: last fit, high first */
	if ((msgSegm = allocMemBlk(len, 0x82)) == 0) {
		*ls = STRINGS_OUT_OF_MEMORY;
		return 0;
	}

	if(farread(fd, MK_FP(msgSegm, 0), len) != len) {
		unloadMsgs();			/* Remove the message segment */
		*ls = STRINGS_READ_ERROR;
		return 0;
	}

	/* Now the offset of the index array are updated to point to the
		real offset instead of the displacement based on the first
		byte of the string data area */
	idx = MK_FP(msgSegm, 0);
	for(i = 0; i < strCnt; ++i)
		idx[i].index += firstStr;

	*ls = STRINGS_LOADED;
	return 1;		/* Stop searching */
}

/*
 *  If the messages are NOT loaded and if there is an error during
 *  the load, the automatic load process might hang here infinitely.
 */
unsigned msgSegment(void)              /* load messages into memory */
{	static int recurs = 1;
	loadStatus status;

	if(msgSegm)
		return msgSegm;

		/* prevent reentrance */
	if(--recurs == 0) {		/* OK */
		for(;;) {
		status = STRINGS_NOT_FOUND;
#ifdef FEATURE_XMS_SWAP
		msgSegm = XMSswapmessagesIn(&status);
		if(status == STRINGS_NOT_FOUND)
#endif
			enumResources(RES_ID_STRINGS, loadStrings, &status);
		switch(status) {
		case STRINGS_LOADED:
		  assert(msgSegm);
		  dprintf(("[Messages successfully loaded to segment 0x%04x]\n"
		   , msgSegm));
			break;
#ifdef DEBUG
		/* Don't use dprintf() to ensure the message is issued, even
			if fddebug is OFF */
		case STRINGS_NOT_FOUND:
			assert(msgSegm == 0);
			dbg_outsn("[No STRINGS resource found!]");
			break;
		case STRINGS_ID_MISMATCH:
			assert(msgSegm == 0);
			dbg_outsn("[STRINGS ID string mismatch.]");
			break;
		case STRINGS_READ_ERROR:
			assert(msgSegm == 0);
			dbg_outsn("[Read error while loading STRINGS.]");
			break;
		case STRINGS_SIZE_MISMATCH:
			assert(msgSegm == 0);
			dbg_outsn("[STRINGS resource has invalid size.]");
			break;
		case STRINGS_OUT_OF_MEMORY:
			assert(msgSegm == 0);
			dbg_outsn("[Out of memory loading STRINGS.]");
			break;
		default:
			assert(msgSegm == 0);
			dbg_outsn("[Unknown error loading STRINGS.]");
			break;
#else	/* Even in non-debugging, an error should
							be displayed */
		case STRINGS_OUT_OF_MEMORY:
			assert(msgSegm == 0);
			puts("[Out of memory loading STRINGS.]");
			break;
#endif
		}

			if(msgSegm)
				break;
			if(inInit > 1)	/* Don't fetch the name interactively too early */
				break;
#undef TEXT_ERROR_OUT_OF_MEMORY
#undef TEXT_ERROR_LOADING_STRINGS
			puts(TEXT_ERROR_LOADING_STRINGS);
			{	char *buf = malloc(128 + 1);

				if(!buf) {
					dos_write(2, TEXT_ERROR_OUT_OF_MEMORY,
					   strlen(TEXT_ERROR_OUT_OF_MEMORY));
					break;
				} else {
					int orgEcho = echo;

					echo = 0;
					readcommand(buf, 128);
					echo = orgEcho;
					if(!*buf) {
						free(buf);
						break;
					}
					if(0 == grabComFilename(1, (char far*)buf)) {
							/* Try to set the valid %COMSPEC% */
						if(!inInit && chgEnv("COMSPEC", ComPath))
							chgEnv("COMSPEC", 0);	/* Zap the old one */
					}
					free(buf);
				}
			}
		}

		++recurs;
	}

	return msgSegm;
}
