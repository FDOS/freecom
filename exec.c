
/*
 *  EXEC.C - external program execution
 *
 *
 *
 *  Comments:
 *
 *  ?????? (Steffen Kaiser)
 *    started.
 *
 *  12/??/95 (Svante Frey)
 *    reorganized code
 *
 *  1/6/96 (Tim Norman)
 *    added this history
 *
 *  06/14/97 (Steffen Kaiser)
 *    added errorlevel
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 1999/11/02 ska
 * bugfix: exec() now also returns the errorlevel to match the
 *  implementation of swpa_exec()
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>

#include "command.h"

#define CBREAK_ERRORLEVEL 3

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

int decode_exec_result(int rc)
{	struct REGPACK rp;
	if (!rc) {
		rp.r_ax = 0x4d00;           /* get return code */
		intr(0x21, &rp);
		rc = rp.r_ax & 0xFF;
		if ((rp.r_ax & 0xFF00) == 0x100) {
			ctrlBreak = 1;
			if (!rc)
				rc = CBREAK_ERRORLEVEL;
		}
	}

	return rc;
}

int exec(const char *cmd, char *cmdLine, const unsigned segOfEnv)
{
  unsigned char buf[128];
  struct fcb fcb1,
    fcb2;
  struct ExecBlock execBlock;

  assert(cmd);
  assert(cmdLine);

  /* generate Pascal string from the command line */
  memcpy(&buf[1], cmdLine, buf[0] = strlen(cmdLine));
  memcpy(&buf[1] + buf[0], "\xd", 2);

  /* fill execute structure */
  execBlock.segOfEnv = segOfEnv;
  execBlock.cmdLine = (char far *)buf;
  execBlock.fcb1 = (struct fcb far *)&fcb1;
  execBlock.fcb2 = (struct fcb far *)&fcb2;

  /* fill FCBs */
  if ((cmdLine = parsfnm(cmdLine, &fcb1, 1)) != NULL)
    parsfnm(cmdLine, &fcb2, 1);

	return decode_exec_result(
	 lowLevelExec((char far *)cmd, (struct ExecBlock far *)&execBlock));
}
