/*
 * MESSAGES.C - Read messages from strings database located in a file or
 *              at the end of command.com
 *
 *
 * Comments:
 *
 * 27 Jul 1998  John P. Price
 * - started.
 *
 * 03-Dec-1998  John P. Price
 * - Reduced the ammount of memory allocated by not reading in entire index,
 *   but only the index element needed.
 *
 * 1998/12/05 ska
 * - chg: split displayString() into two parts: 1) load string into
 *  local heap, 2) display string
 * - add: load messages into conventional far memory, high memory tried
 *  first
 *
 * 11-Feb-1999  John P. Price <linux-guru@gcfl.net>
 * - Changed openStrFile() function to try to read the strings from the end
 *   of command first, and then to try to read strings.dat in the same
 *   directory as command.com.  This helps when running from the debugger.
 *
 * 1999/07/07 ska
 * chg: the messages are always loaded into memory now, but are freed
 *  before an external program is executed. This helps to reduce the
 *  "bug" reports that if FreeCom is started from floppy the answer
 *  time in case of error is too long.
 * sub: feature LOAD MESSAGES
 * fix: if open(STRINGS.DAT) succeeds, but the ID check fails, the
 *  return value is not NULL
 * sub: error messages, why the COMMAND.COM/STRINGS.DAT is wrong
 *  this error messages are useful while debugging, but end-user should
 *  something like "%s does not point to FreeCom, please enter correct
 *  path to FreeCom or insert correct media and press ENTER".
 * add: Invalid message IDs or if the message file is absent, a default
 *  message is generated, "Error #<ID>\n". Will work for most of the
 *  current uses
 *
 * 2000/07/07 Ron Cemer
 * fix: add typecast to local variables for _fmemcpy()
 *
 * 2000/07/09 ska
 * add: Support for STRINGS.DAT without re-compiling FreeCOM
 */

//#undef DEBUG

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <alloc.h>
#include <dir.h>
#include <dos.h>
#include <io.h>

#include "config.h"
#include "strings.typ"
#include "command.h"
#include "strings.h"
#include "suppl.h"
#include "res.h"
#include "fmemory.h"

/* Default message settings:
  PATTERN: how the string is constructed for the ID (with \n)
  OUTOFMEMORY: string to issue on out-of-memory condition (no \n)
*/
#define DFL_MSG_PATTERN "Error #%u\n"
#define DFL_MSG_OUTOFMEMORY "Out of memory!"
#define ERR_ID_OUTOFMEMORY TEXT_ERROR_OUT_OF_MEMORY

typedef enum {
	  STRINGS_LOADED
	, STRINGS_ID_MISMATCH
	, STRINGS_SIZE_MISMATCH
	, STRINGS_NOT_FOUND
	, STRINGS_READ_ERROR
	, STRINGS_OUT_OF_MEMORY
} loadStatus;

static unsigned msgSegm = 0;    /* strings segment if loaded */
static string_count_t strCnt = 0;		/* number of strings */
//static string_size_t firstStr = 0;	/* offset of first string in segment */

/* Remove the string segment from memory */
void unloadMsgs(void) 
{
  if (msgSegm) {
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
#pragma argsused
int loadStrings(res_majorid_t major
	, res_minorid_t minor
	, long length
	, FILE* f
	, void * const arg)
{	loadStatus *ls = arg;
	char fdid[sizeof(STRINGS_ID)];
	string_size_t len, firstStr;
	string_index_t far*idx;
	int i;

	if((unsigned long)length >= 0x10000ul
	 || (unsigned)length < STRINGS_HEADER_SIZE) {
		*ls = STRINGS_SIZE_MISMATCH;
		return 0;
	}

	fread(fdid, sizeof(STRINGS_ID) - 1, 1, f);

	if (memcmp(fdid, STRINGS_ID, sizeof(STRINGS_ID) - 1)) {
		*ls = STRINGS_ID_MISMATCH;
		return 0;			/* Continue searching */
	}
	/* immediately after the ID a trailer follows */
	fseek(f, (long)STRINGS_ID_TRAILER, 1);

		/* Read the strings dimensionating parameters */
	if(ferror(f)
	 || fread(&strCnt, sizeof(strCnt), 1, f) != 1
	 || fread(&len, sizeof(len), 1, f) != 1) { /* Read error */
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
	if ((msgSegm = allocBlk(len, 0x82)) == 0) {
		*ls = STRINGS_OUT_OF_MEMORY;
		return 0;
	}

	if(farread(MK_FP(msgSegm, 0), len, f) != len) {
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
	loadStatus status = STRINGS_NOT_FOUND;
#ifndef DEBUG		/* To ensure the error is displayed only once */
	static int displayed = 0;
#endif

	if(!msgSegm) {     /* not loaded, yet */
		/* prevent reentrance */
		if(--recurs == 0) {		/* OK */
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
			default:	
				assert(msgSegm == 0);
				if(!displayed) {
					displayed = 1;
					puts("\n\a[Could not load STRINGS resource.]");
				}
				break;
#endif
			}
		}

		++recurs;
	}

	return msgSegm;
}

char *defaultMessage(unsigned id)
{
  char *h;

  /* Allocate 5 bytes for the numerical ID itself */
  if(id != ERR_ID_OUTOFMEMORY
   && (h = malloc(sizeof(DFL_MSG_PATTERN) + 5)) != 0) {
    /* Got enough memory to allocate the stuff */
    sprintf(h, DFL_MSG_PATTERN, id);
    return h;
  }

  puts(DFL_MSG_OUTOFMEMORY);
  return 0;
}

char *fetchString(unsigned id)
{
	char *thisstr;
	unsigned segm;
    string_index_t far *idx;

	  /* Issue the default message, if:
  		+ strings could not be loaded into memory,
  		+ requested string ID is not included with resource
  			(too high, or not defined).
	*/
	if((segm = msgSegment()) == 0
	 || id >= strCnt
	 || (idx = MK_FP(msgSegm, id * sizeof(*idx)))->size == 0)
		return defaultMessage(id);

	/* pointer to id's control data */
	if((thisstr = malloc(idx->size)) == 0)
		return defaultMessage(ERR_ID_OUTOFMEMORY);

	_fmemcpy((char far*)thisstr, MK_FP(segm, idx->index), idx->size);

	return thisstr;
}

/*
 * As fetchString(), but removes the last character.
 *
 *  This is necessary, because there is always a newline character
 *  at the end of the last line.
 */
char *getMessage(unsigned id)
{
  char *h;

  if ((h = fetchString(id)) != 0 && *h)
    strchr(h, '\0')[-1] = '\0';
  return h;
}

void displayString(unsigned id,...)
{
  char *thisstr;
  va_list argptr;

  if ((thisstr = fetchString(id)) == 0)
    return;

  va_start(argptr, id);
  vprintf(thisstr, argptr);
  va_end(argptr);
  free(thisstr);
}
