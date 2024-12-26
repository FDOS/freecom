/*	$Id$

	Execute an external program using DOS-4B-00

	This file bases on EXEC.C of FreeCOM v0.81 beta 1.

	Returns: DOS error code of DOS-4B

	$Log$
	Revision 1.7  2006/09/05 01:44:33  blairdude
	Massive patches from Arkady that I'm way too lazy to sort through.  If anything happens, Arkady can answer for it.

	Revision 1.6  2004/10/25 19:37:34  skaus
	fix: LH: Errorlevel of program effects LH's error reporting {Eric Auer}
	
	Revision 1.5  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.4  2003/03/05 17:43:52  skaus
	bugfix: cached NLS data not flushed
	
	Revision 1.3  2002/11/12 18:31:57  skaus
	add: save/restore session (swap context) {Tom Ehlert}
	
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
#include <stdio.h>
#include <dos.h>
#include <string.h>

#include "../include/command.h"
#include "../include/cswap.h"
#include "../include/nls.h"

#include "algnbyte.h"

#if defined(__WATCOMC__) || defined(__GNUC__)
struct fcb     {
	char bytes[0x25];
};

#ifdef __WATCOMC__
char *parsfnm(const char *cmdline, struct fcb far *fcbptr, int option);
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#pragma aux parsfnm = \
	"mov ah, 29h" \
	"int 21h" \
	"inc al" \
	"jnz ok" \
	"xor si, si" \
	"ok:" \
	__parm [__si] [__es __di] [__ax] __value [__si] __modify [__ax __es]
#else
#pragma aux parsfnm = \
	"mov ah, 29h" \
	"int 21h" \
	"inc al" \
	"jnz ok" \
	"xor si, si" \
	"ok:" \
	__parm [__ds __si] [__es __di] [__ax] __value [__ds __si] __modify [__ax __es]
#endif
#else /* __GNUC__ */
static char *parsfnm(const char *cmdline, struct fcb far *fcbptr, int option)
{
	char *ret;
	unsigned char opt = option;
	asm volatile("int $0x21" :
		"=S"(ret), "+Ral"(opt) :
		"Rah"((unsigned char)0x29), "e"(FP_SEG(fcbptr)), "D"(FP_OFF(fcbptr)),
		"Rds"(FP_SEG(cmdline)), "0"(cmdline) :
		"cc", "memory");
	return opt == 0xff ? NULL : ret;
}
#endif
#endif

struct ExecBlock
{
	word segOfEnv;
	char far *cmdLine;
	struct fcb far *fcb1;
	struct fcb far *fcb2;
};

#include "algndflt.h"

int cdecl lowLevelExec(char far * cmd, struct ExecBlock far * bl);

int exec(const char *cmd, char *cmdLine, const unsigned segOfEnv)
{
#ifdef FEATURE_XMS_SWAP
#	define buf dosCMDTAIL
#	define memcpy _fmemcpy
#else
	unsigned char buf[MAX_EXTERNAL_COMMAND_SIZE+2]; /* 128 bytes is max size in PSP, 2 bytes for size and terminator */
#endif
	struct fcb fcb1, fcb2;
	struct ExecBlock execBlock;
	int retval;
	int cmdLen;

	assert(cmd);
	assert(cmdLine);

	invalidateNLSbuf();

	/* generate Pascal string from the command line */
	/* we assume passed in c string (ASCIIZ) */
	cmdLen = strlen(cmdLine);
	if (cmdLen > MAX_EXTERNAL_COMMAND_SIZE) {
		/* we assume CMDLINE environment variable already set with full cmdLine */
		/* so we simply truncate passed command line to max size, terminated by \r, no \0 for callee */
		/* for maximum compatibility set size to 127 (0x7f) */
		buf[0] = (unsigned char)(MAX_EXTERNAL_COMMAND_SIZE+1);
		memcpy(&buf[1], cmdLine, MAX_EXTERNAL_COMMAND_SIZE);
		/* terminate with just carriage return \r */
		buf[MAX_EXTERNAL_COMMAND_SIZE+1] = '\x0d';
	} else {
		/* set size of actual command line and copy to buffer */
		buf[0] = (unsigned char)cmdLen;
		memcpy(&buf[1], cmdLine, cmdLen);
		/* ensure terminated with \r\0, if less then 126 characters
		   or \r, if exactly 126 characters */
		memcpy(&buf[1] + cmdLen, "\x0d", (cmdLen < MAX_EXTERNAL_COMMAND_SIZE) ? 2 : 1);
	}

	/* fill FCBs */
	if ((cmdLine = parsfnm(cmdLine, &fcb1, 1)) != 0) {
		parsfnm(cmdLine, &fcb2, 1);
	}

	saveSession();

#ifdef FEATURE_XMS_SWAP
	if(XMSisactive() && swapOnExec == TRUE) {
		/* Copy the prepared values into the buffers in CSWAP.ASM module */
		_fmemcpy(dosFCB1, &fcb1, sizeof(fcb1));
		_fmemcpy(dosFCB2, &fcb2, sizeof(fcb1));
		assert(strlen(cmd) < 128);
		_fstrcpy((char far *)dosCMDNAME, cmd);
		dosParamDosExec.envSeg = segOfEnv; 

		retval = XMSexec();
		} else
#endif
	{
		/* fill execute structure */
		execBlock.segOfEnv = segOfEnv;
		execBlock.cmdLine = (char far *)buf;
		execBlock.fcb1 = (struct fcb far *)&fcb1;
		execBlock.fcb2 = (struct fcb far *)&fcb2;

		retval = lowLevelExec((char far*)cmd, (struct ExecBlock far*)&execBlock);
	}

	restoreSession();
				
	return retval;
}
