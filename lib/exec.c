/*	$id$
	$Locker$	$Name$	$State$

	Execute an external program using DOS-4B-00

	This file bases on EXEC.C of FreeCOM v0.81 beta 1.

	$Log$
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

/* align one byte */
#pragma option -a-
struct ExecBlock
{
  word segOfEnv;
  char far *cmdLine;
  struct fcb far *fcb1,
    far * fcb2;
};
/* default alignment */
#pragma option -a.

int lowLevelExec(char far * cmd, struct ExecBlock far * bl);

int exec(const char *cmd, char *cmdLine, const unsigned segOfEnv)
{
#ifdef FEATURE_XMS_SWAP
#	define buf dosCMDTAIL
#	define memcpy _fmemcpy
#else
  unsigned char buf[128];
#endif
  struct fcb fcb1,
    fcb2;
  struct ExecBlock execBlock;

  assert(cmd);
  assert(cmdLine);
  assert(strlen(cmdLine) <= 125);

  /* generate Pascal string from the command line */
  memcpy(&buf[1], cmdLine, buf[0] = strlen(cmdLine));
  memcpy(&buf[1] + buf[0], "\xd", 2);

  /* fill FCBs */
  if ((cmdLine = parsfnm(cmdLine, &fcb1, 1)) != 0)
    parsfnm(cmdLine, &fcb2, 1);

#ifdef FEATURE_XMS_SWAP
	if(XMSisactive() && swapOnExec == TRUE) {
		/* Copy the prepared values into the buffers in CSWAP.ASM module */
		_fmemcpy(dosFCB1, &fcb1, sizeof(fcb1));
		_fmemcpy(dosFCB2, &fcb2, sizeof(fcb1));
		assert(strlen(cmd) < 128);
		_fstrcpy(dosCMDNAME, cmd);
		dosParamDosExec.envSeg = segOfEnv;
		return decode_exec_result(XMSexec());
	}
#endif
  /* fill execute structure */
  execBlock.segOfEnv = segOfEnv;
  execBlock.cmdLine = (char far *)buf;
  execBlock.fcb1 = (struct fcb far *)&fcb1;
  execBlock.fcb2 = (struct fcb far *)&fcb2;

	return decode_exec_result(
	 lowLevelExec((char far *)cmd, (struct ExecBlock far *)&execBlock));
}
