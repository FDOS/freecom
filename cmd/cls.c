/* $Id$
 *  CLS.C - clear screen internal command
 *
 *  Comments:
 *
 *	Issues ^L (Form Feed), then:
 *	if file descriptor #1 (aka stdout) is connected to a device _and_
 *	is not the NUL: device, uses the BIOS to clear the screen
 *	and home the cursor to the upper left.
 *
 *  07/27/1998 (John P. Price)
 *    started.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 30-Nov-1998 (John P Price <linux-guru@gcfl.net>)
 * - CLS now sets the text colors to lightgray on black before clearing the
 *   screen.
 *
 * $Log$
 * Revision 1.6  2004/08/19 19:10:49  skaus
 * fix: CLS: cut debug message
 *
 * Revision 1.5  2004/06/29 14:14:54  skaus
 * fix: help screen of internal commands causes "Unknown command error" {Bernd Blaauw}
 *
 * Revision 1.4  2004/06/15 19:27:43  skaus
 * bugfix: CLS: use BIOS, unless fd1 is file or NUL {Eric Auer}
 *
 */

#include "../config.h"

#include <dos.h>
#include <stdio.h>

#include "../include/command.h"
#include "../include/openf.h"
#include "../include/misc.h"

#pragma argsused
int cmd_cls(char *param)
{	struct REGPACK r;

	putchar('\xc');		/*  ^L Form feed */
	fflush(stdout);
	fflush(stderr);

	/* Output stream is neither a file nor NUL nor CLOCK$ */
	if(((fdattr(1) ^ 0x80) & (0x80 | 0x08 | 0x04)) == 0) {
		/* Now roll the screen */
		r.r_ax = 0x0600;	/* Scroll window up // entire window */
		r.r_bx = 0x0700;	/* Attribute to write */
		r.r_cx = 0x0000;	/* Upper left */
		r.r_dx = ((SCREEN_ROWS - 1) << 8) | (SCREEN_COLS - 1); /* Lower right */
		intr(0x10, &r);
		goxy(1, 1);			/* home the cursor */
	}

	return 0;
}
