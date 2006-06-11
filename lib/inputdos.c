/*	$Id$

	Input a line via DOS-0A

	This file bases on CMDINPUT.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.4  2006/06/11 02:47:05  blairdude
	Optimized FreeCOM for size, fixed LFN bugs, and started an int 2e handler (which safely fails at the moment)

	Revision 1.3  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.2  2003/12/09 21:29:24  skaus
	bugfix: Ask for FreeCOM location when STRINGS are missing [#687]
	
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
#include <string.h>

#include "../include/command.h"

#if MAX_INTERNAL_COMMAND_SIZE > 253
#define CMD_SIZE 253
#else
#define CMD_SIZE MAX_INTERNAL_COMMAND_SIZE
#endif

static unsigned char iobuf[CMD_SIZE + 2] = { CMD_SIZE, '\0'};

void readcommandDOS(char * const str, int maxlen)
{	struct REGPACK r;

	assert(str);
	assert(maxlen);

	iobuf[0] = (maxlen < CMD_SIZE)? maxlen: CMD_SIZE;
	if(iobuf[1] > iobuf[0])
		iobuf[1] = iobuf[0];

	dprintf(("[CMDINPUT characters max:%u in:%u]\n", iobuf[0], iobuf[1]));
	if(echo)
		printprompt();

	r.r_ax = 0xa00;
	r.r_ds = FP_SEG(iobuf);
	r.r_dx = FP_OFF(iobuf);
	intr(0x21, &r);

	dprintf(("[CMDINPUT characters max:%u out:%u]\n", iobuf[0], iobuf[1]));
	if(iobuf[1]) 		/* could bug if == 0 */
		memcpy(str, &iobuf[2], iobuf[1]);
	str[iobuf[1]] = 0;
//	putchar('\n');
    write( 1, "\n", 1 );
}
