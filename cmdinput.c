/*
 *  CMDINPUT.C - handles command input (tab completion, history, etc.)
 *
 *
 *
 *  Comments:
 *
 *  01/14/95 (Tim Norman)
 *    started.
 *
 *  08/08/95 (Matt Rains)
 *    i have cleaned up the source code. changes now bring this source into
 *    guidelines for recommended programming practice.
 *
 *    i have added some constants to help making changes easier.
 *
 *  12/12/95 (Tim Norman)
 *    added findxy() function to get max x/y coordinates to display
 *    correctly on larger screens
 *
 *  12/14/95 (Tim Norman)
 *    fixed the Tab completion code that Matt Rains broke by moving local
 *    variables to a more global scope and forgetting to initialize them
 *    when needed
 *
 *  8/1/96 (Tim Norman)
 *    fixed a bug in tab completion that caused filenames at the beginning
 *    of the command-line to have their first letter truncated
 *
 *  9/1/96 (Tim Norman)
 *    fixed a silly bug using printf instead of fputs, where typing "%i"
 *    confused printf :)
 *
 *  6/14/97 (Steffan Kaiser)
 *    ctrl-break checking
 *
 *  6/7/97 (Marc Desrochers)
 *    recoded everything! now properly adjusts when text font is changed.
 *    removed findxy(), reposition(), and reprint(), as these functions
 *    were inefficient. added goxy() function as gotoxy() was buggy when the
 *    screen font was changed. the printf() problem with %i on the command
 *    line was fixed by doing printf("%s",str) instead of printf(str).
 *    don't ask how I find em just be glad I do :)
 *
 *  7/12/97 (Tim Norman)
 *    Note: above changes pre-empted Steffan's ctrl-break checking.
 *
 *  7/7/97 (Marc Desrochers)
 *    rewrote a new findxy() because the new dir() used it.  This findxy()
 *    simply returns the values of *maxx *maxy.  In the future, please use
 *    the pointers, they will always be correct since they point to BIOS
 *    values.
 *
 *  7/8/97 (Marc Desrochers)
 *    once again removed findxy(), moved the *maxx, *maxy pointers global
 *    and included them as externs in command.h.  Also added insert/overstrike
 *    capability
 *
 *  7/13/97 (Tim Norman)
 *    added different cursor appearance for insert/overstrike mode
 *
 *  7/13/97 (Tim Norman)
 *    changed my code to use _setcursortype until I can figure out why
 *    my code is crashing on some machines.  It doesn't crash on mine :)
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 28-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - put ifdef's around filename completion code.
 *
 * 30-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - moved filename completion code to filecomp.c
 * - made second TAB display list of filename matches
 *
 * 31-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - Fixed bug where if you typed something, then hit HOME, then tried to
 *   type something else in insert mode, it crashed.
 *
 * 07-Aug-1998 (John P Price <linux-guru@gcfl.net>)
 * - Fixed carrage return output to better match MSDOS with echo on or off.
 *
 * 1999/04/23 ska
 * add: handle ^C on command line --> as ESC, also sets ECHO state ON
 *
 * 2000/09/26 ska
 *	add: FEATURE_ENHANCED_INPUT (actually the non-defined variant of it)
 *
 * 2000/11/28 (Owen Rudge <orudge@freeuk.com>)
 * - Added support for F1, F3 and F5, as in MS COMMAND.COM
 *   F1 = Insert next character from last buffer
 *   F3 = Complete string (NOTE: in this version, replaces string)
 *   F5 = Places current string in buffer, moves to next line
 *
 * 2000/12/10 ska
 *	chg: F3 == complete string
 *	add: cur-right at end of string == F1
 *	fix: F5 removes cursor sometimes
 *
 * 2001/02/16 ska
 *	bugfix: If ENHANCED_INPUT && !HISTORY, Cur-Right falls through CurLeft
 */

#include "config.h"

#include <assert.h>
#include <string.h>
#include <dos.h>

#include "command.h"
#include "batch.h"

#ifndef FEATURE_ENHANCED_INPUT

/* Using traditional input via DOS-21-0A */

#if MAX_INTERNAL_COMMAND_SIZE > 253
#define CMD_SIZE 253
#else
#define CMD_SIZE MAX_INTERNAL_COMMAND_SIZE
#endif

unsigned char iobuf[CMD_SIZE + 2] = { CMD_SIZE, '\0'};

void readcommand(char *str, int maxlen)
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
	putchar('\n');
}

#else

#include <conio.h>
#include <stdio.h>

#define D_BS      8
#define D_DELETE  256+83
#define D_INSERT  256+82
#define D_TAB     9
#define D_HOME    256+71
#define D_END     256+79
#define D_UP      256+72
#define D_DOWN    256+80
#define D_LEFT    256+75
#define D_RIGHT   256+77
#define D_ENTER   13
#define D_ESC     27
#define D_CTL_C   3
#define D_CTL_R		0x12
#define D_CTL_L		0x0c

/* OCR - MS-DOS compatible history keystrokes */
#define D_F1      256+59
#define D_F3      256+61
#define D_F5      256+63

#ifdef FEATURE_HISTORY
void history(int, char *);      /* prototype for the command-line history */
#endif

#ifdef FEATURE_FILENAME_COMPLETION
void complete_filename(char *str, unsigned charcount);
int show_completion_matches(char *str, unsigned charcount);
#endif

unsigned orgx, orgy;		/* start of current line */

/*
 * goxy
 *
 * move the cursor on the screen.  Needs to be in ASM because of bugs with
 * gotoxy()
 */
void goxy(unsigned char x, unsigned char y)
{
  union REGS regs;

  assert(x > 0);
  assert(y > 0);

  regs.h.ah = 2;                /* set cursor position */
  regs.h.dh = y - 1;
  regs.h.dl = x - 1;
  regs.h.bh = 0;                /* video page 0 */
  int86(0x10, &regs, &regs);
}

/*
 * clrcmdline
 *
 *
 */
void clrcmdline(char *str, int maxlen, unsigned orgx, unsigned orgy)
{
  int count;

  assert(str);

  goxy(orgx, orgy);
  for (count = 0; count < strlen(str); count++)
    putchar(' ');
  memset(str, 0, maxlen);
  goxy(orgx, orgy);
}

/* Print a character to current cursor position
	Updates cursor postion
 */
void outc(char c)
{
	putchar(c);
}
/* Print a blank to current cursor postion
	Updates cursor position
 */
void outblank(void)
{	outc(' ');
}

/* Print string to current cursor position
	Updates cursor position
 */
void outs(const char * const s)
{	assert(s);
	fputs(s, stdout);
}
/* Print string to cursor position and append one blank
	Updates cursor postion
 */
void outsblank(const char * const s)
{	outs(s);
	outblank();
}

/* read in a command line */
void readcommand(char *str, int maxlen)
{
  unsigned char insert = 1;
  unsigned ch;
#ifdef FEATURE_FILENAME_COMPLETION
  unsigned lastch = 0;
#endif
  unsigned curx;
  unsigned cury;
  int count;
  unsigned current = 0;
  unsigned charcount = 0;

  char tmpstr[MAX_INTERNAL_COMMAND_SIZE];

  /* if echo off, don't print prompt */
  if (echo)
    printprompt();

  assert(str);

  orgx = wherex();
  orgy = wherey();
  memset(str, 0, maxlen);

  _setcursortype(_NORMALCURSOR);

  do
  {
    ch = getch();
    if (ch == 0)
      ch = 256 + getch();       // special key

    if(cbreak)
      ch = D_CTL_C;

    switch (ch)
    {
      case D_BS:               // delete character to left of cursor

        if (current > 0 && charcount > 0)
        {
          if (current == charcount)     //if at end of line

          {
            str[current - 1] = 0;
            if (wherex() != 1)
              outs("\b \b");
            else
            {
              goxy(MAX_X, wherey() - 1);
              outblank();
              goxy(MAX_X, wherey() - 1);
            }
          }
          else
          {
            for (count = current - 1; count < charcount; count++)
              str[count] = str[count + 1];
            if (wherex() != 1)
              goxy(wherex() - 1, wherey());
            else
              goxy(MAX_X, wherey() - 1);
            curx = wherex();
            cury = wherey();
            outsblank(&str[current - 1]);
            goxy(curx, cury);
          }
          charcount--;
          current--;
        }
        break;

      case D_INSERT:           /* toggle insert/overstrike mode */
        insert ^= 1;
        if (insert)
          _setcursortype(_NORMALCURSOR);
        else
          _setcursortype(_SOLIDCURSOR);
        break;

      case D_DELETE:           // delete character under cursor

        if (current != charcount && charcount > 0)
        {
          for (count = current; count < charcount; count++)
            str[count] = str[count + 1];
          charcount--;
          curx = wherex();
          cury = wherey();
          outsblank(&str[current]);
          goxy(curx, cury);
        }
        break;

      case D_HOME:             // goto beginning of string

        if (current != 0)
        {
          goxy(orgx, orgy);
          current = 0;
        }
        break;

      case D_END:              // goto end of string

        if (current != charcount)
        {
          goxy(orgx, orgy);
          outs(str);
          current = charcount;
        }
        break;

#ifdef FEATURE_FILENAME_COMPLETION
      case D_TAB:
        // expand current file name
        if (current == charcount)       // only works at end of line

        {
          if (lastch != D_TAB)  // if first TAB, complete filename

          {
            complete_filename(str, charcount);
            charcount = strlen(str);
            current = charcount;

            goxy(orgx, orgy);
            outs(str);
            if ((strlen(str) > (MAX_X - orgx)) && (orgy == MAX_Y + 1))
              orgy--;
          }
          else                  //if second TAB, list matches

          {
            if (show_completion_matches(str, charcount))
            {
              printprompt();
              orgx = wherex();
              orgy = wherey();
              outs(str);
            }
          }
        }
        else
          beep();
        break;
#endif

      case D_ENTER:            // end input, return to main

#ifdef FEATURE_HISTORY
        if (str[0])
          history(0, str);      // add to the history
#endif

        outc('\n');
        break;

      case D_CTL_C:       // ^C
      case D_ESC:              // clear str  Make this callable!

        clrcmdline(str, maxlen, orgx, orgy);
        current = charcount = 0;

        if(ch == D_CTL_C && !echo) {
          /* enable echo to let user know that's this
            is the command line */
          echo = 1;
          printprompt();
        }
        break;

      case D_RIGHT:            // move cursor right

        if (current != charcount)
        {
          current++;
          if (wherex() == MAX_X)
            goxy(1, wherey() + 1);
          else
            goxy(wherex() + 1, wherey());
			break;
        }
        /* cursor-right at end of string grabs the next character
        	from the previous line */
        /* FALL THROUGH */

#ifndef FEATURE_HISTORY
		break;
#else
      case D_F1:               // OCR: get character from last command buffer
			history(-1, tmpstr);
          if (current < strlen(tmpstr)) {
			 outc(str[current] = tmpstr[current]);
			 charcount = ++current;
          }
          break;
          
      case D_F3:               // OCR: get previous command from buffer
		history(-1, tmpstr);
		if(charcount < strlen(tmpstr)) {
			outs(strcpy(&str[charcount], &tmpstr[charcount]));
		   current = charcount = strlen(str);
	   }
	   break;

      case D_UP:               // get previous command from buffer

        clrcmdline(str, maxlen, orgx, orgy);
        history(-1, str);
        current = charcount = strlen(str);
        outs(str);
        break;

      case D_DOWN:             // get next command from buffer

        clrcmdline(str, maxlen, orgx, orgy);
        history(1, str);
        current = charcount = strlen(str);
        outs(str);
        break;
#endif

      case D_LEFT:             // move cursor left

        if (current > 0)
        {
          current--;
          if (wherex() == 1)
            goxy(MAX_X, wherey() - 1);
          else
            goxy(wherex() - 1, wherey());
        }
#if 0
        else
               beep();
#endif
        break;

	case D_F5:              // OCR: keep cmdline in F3/UP buffer and move to next line

		history(0, str);
        clrcmdline(str, maxlen, orgx, orgy);
        outc('@');
		if(orgy >= MAX_Y) {
			outc('\n');			/* Force scroll */
			orgy = MAX_Y;
		} else {
			++orgy;
		}
		goxy(orgx, orgy);
        current = charcount = 0;

        break;

      default:                 // insert character into string...

        if ((ch >= 32 && ch <= 255) && (charcount != (maxlen - 2)))
        {
          if (insert && current != charcount)
          {
            for (count = charcount; count > current; count--)
              str[count] = str[count - 1];
            str[current++] = ch;
            curx = wherex() + 1;
            cury = wherey();
            outs(&str[current - 1]);
            if ((strlen(str) > (MAX_X - orgx)) && (orgy == MAX_Y + 1))
              cury--;
            goxy(curx, cury);
            charcount++;
          }
          else
          {
            if (current == charcount)
              charcount++;
            str[current++] = ch;
            outc(ch);
          }
          if ((strlen(str) > (MAX_X - orgx)) && (orgy == MAX_Y + 1))
            orgy--;
        }
        else
          beep();
        break;
    }
#ifdef FEATURE_FILENAME_COMPLETION
    lastch = ch;
#endif
  }
  while (ch != D_ENTER);

  _setcursortype(_NORMALCURSOR);
}
#endif
