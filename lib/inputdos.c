/*	$id$
	$Locker$	$Name$	$State$

	Input a line via DOS-0A

	Keep the buffer static to allow F3.

	This file bases on CMDINPUT.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.4  2001/07/25 20:01:18  skaus
	Update #10

	Revision 1.1.4.3  2001/07/16 20:28:38  skaus
	Update #9
	
	Revision 1.1.4.2  2001/07/01 22:04:31  skaus
	Update #3
	
	Revision 1.1.4.1  2001/06/25 20:06:36  skaus
	Update #3
	
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
#include "../include/context.h"

#if MAX_INTERNAL_COMMAND_SIZE > 253
#define CMD_SIZE 253
#else
#define CMD_SIZE MAX_INTERNAL_COMMAND_SIZE
#endif

static unsigned char iobuf[CMD_SIZE + 3] = { CMD_SIZE, '\0'};

#pragma argsused
char *readcommandDOS(void)
{	struct REGPACK r;

	iobuf[0] = CMD_SIZE;
	if(iobuf[1] > iobuf[0])
		iobuf[1] = iobuf[0];

	dprintf(("[CMDINPUT characters max:%u in:%u]\n", iobuf[0], iobuf[1]));
	if(lflag_echo)
		printprompt();

	r.r_ax = 0xa00;
	r.r_ds = FP_SEG(iobuf);
	r.r_dx = FP_OFF(iobuf);
	intr(0x21, &r);

	dprintf(("[CMDINPUT characters max:%u out:%u]\n", iobuf[0], iobuf[1]));
	iobuf[2 + iobuf[1]] = 0;	/* make sure it's terminated */
	putchar('\n');
	return estrdup((char*)&iobuf[2]);
}
