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
 * Revision 1.9  2006/09/05 01:44:32  blairdude
 * Massive patches from Arkady that I'm way too lazy to sort through.  If anything happens, Arkady can answer for it.
 *
 * Revision 1.8  2006/06/12 04:55:42  blairdude
 * All putchar's now use outc which first flushes stdout and then uses write to write the character to the console.  Some potential bugs have been fixed ( Special thanks to Arkady for noticing them :-) ).  All CONIO dependencies have now been removed and replaced with size-optimized functions (for example, mycprintf, simply opens "CON" and directly writes to the console that way, and mywherex and mywherey use MK_FP to access memory and find the cursor position).  FreeCOM is now
 * significantly smaller.
 *
 * Revision 1.7  2006/06/11 02:06:11  blairdude
 *
 *
 * All of FreeCOM now uses write instead of putchar and intr instead of int86[x] or intdos[x]
 *
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

int cmd_cls (char * param) {
    (void)param;
    outc( '\xc' ); /* ^L Form feed */

	/* Output stream is neither a file nor NUL nor CLOCK$ */
	if(((fdattr(1) ^ 0x80) & (0x80 | 0x08 | 0x04)) == 0) {
		unsigned attr = 0x0700;
		int mode;
		IREGS r;
		
		/* Get the current video mode */
		
		r.r_ax = 0x0f00;	/* Scroll window up // entire window */
		intrpt(0x10, &r);
		mode = r.r_ax & 0x7f;
		
		switch (mode)
		{
		case 0x04: /* CGA 320x200 */
		case 0x05: /* CGA 320x200, grayscale */
		case 0x09: /* PCjr 320x200 */
		case 0x0a: /* PCjr 640x200 */
		case 0x0b: /* Tandy 1000 SL/TL */
		case 0x0d: /* EGA/VGA 320x200, 16 colors */
		case 0x0e: /* EGA/VGA 640x200, 16 colors */
		case 0x0f: /* EGA/VGA 640x350, mono */
		case 0x10: /* EGA/VGA 640x350, 4 or 16 colors */
		case 0x11: /* VGA 640x480, mono */
		case 0x12: /* VGA 640x480, 16 colors */
		case 0x13: /* VGA 320x200, 256 colors */
		case 0x59: /* SVGA 800x600, 16 colors */
			attr = 0;
			break;
		default:
			;
		}
		
		/* Now roll the screen */
		r.r_ax = 0x0600;	/* Scroll window up // entire window */
		r.r_bx = attr;		/* Attribute to write */
		r.r_cx = 0x0000;	/* Upper left */
		r.r_dx = ((SCREEN_ROWS - 1) << 8) | (SCREEN_COLS - 1); /* Lower right */
		intrpt(0x10, &r);
		goxy(1, 1);			/* home the cursor */
	}

	return 0;
}
