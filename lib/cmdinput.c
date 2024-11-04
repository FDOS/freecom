/* $Id$
 *  CMDINPUT.C - handles command input (tab completion, history, etc.)
 */

#include "../config.h"

#include <assert.h>
#include <dos.h>
#include <stdio.h>
#include <io.h>
#include <string.h>

#include "../include/command.h"
#include "../include/keys.h"
#include "../include/misc.h"
#if defined(DBCS)
# include "../include/openf.h"
# include "../suppl/nls_c.h"
#endif

static unsigned orgx, orgy;		/* start of current command input */

#if defined(NEC98)
unsigned mywherex (void) {
    return 1U + *(unsigned char far *)MK_FP(0x60, 0x11c);
}
unsigned mywherey (void) {
    return 1U + *(unsigned char far *)MK_FP(0x60, 0x110);
}
#elif defined(IBMPC)
#define MK_PTR(type,seg,ofs) ((type FAR*) MK_FP (seg, ofs))
/* safer edition of MK_FP (Arkady) */
typedef struct { unsigned char col, row; } SCRPOS;
#define _scr_page      (* MK_PTR (volatile const unsigned char, 0, 0x462))
#define _scr_pos_array    MK_PTR (volatile const SCRPOS, 0, 0x450)
unsigned mywherex (void) {
    return _scr_pos_array [_scr_page].col + 1;
}

unsigned mywherey (void) {
    return _scr_pos_array [_scr_page].row + 1;
}
#endif

#if defined(NEC98)
char use_int29 = 0;
static void outc_dos(char c)
{
    IREGS r;
    if (use_int29)
    {
        r.r_ax = c;
        intrpt(0x29, &r);
    }
    else
    {
        dos_write(1, &c, 1);
    }
}
void outc(char c)
{
    if (c == '\n') outc_dos('\r');
    outc_dos(c);
    if (mywherex() > MAX_X) {
        outc_dos('\b');
        outc_dos('\r');
        outc_dos('\n');
    }
}
void outs(const char * const s)
{
	const char *s1 = (const char *)s;
	assert(s);
	while(*s1) outc(*s1++);
}
#else
/* Print string to current cursor position
	Updates cursor position
 */
void outs(const char * const s)
{	assert(s);
	printf("%s", s);
}

/* Print a character to current cursor position
	Updates cursor postion
 */
void outc(char c)
{
	if (c == '\n') dos_write(1, "\r\n", 2); else dos_write(1, &c, 1);
}
#endif

#ifdef FEATURE_ENHANCED_INPUT

/* set cursor state for insert/overwrite mode */
#if defined(NEC98)

static void setcursorstate_nec98(int insert)
{
	IREGS r;

	/* set cursor blink state (and hide cursor) */
	r.r_ax = insert ? 0x1001 : 0x1000;
	intrpt(0x18, &r);
	if (*(unsigned char far *)MK_FP(0x60, 0x11b))
	{
		/* (re-)show cursor */
		r.r_ax = 0x1100;
		intrpt(0x18, &r);
	}
#if 0
	extern void nec98_setcursorblink(int blink_rate);
	nec98_setcursorblink(insert ? 0xff : 0x0d);
	goxy(mywherex(), mywherey());
#endif
}

# define setcursorstate setcursorstate_nec98

#elif defined(IBMPC)

static void setcursorstate_ibmpc(int insert)
{
	IREGS r;
	unsigned char cur_mode = *(unsigned char far *)MK_FP(0x40, 0x49);
	
	/* for IBMPC: insert -> _NORMALCURSOR, not insert -> _SOLIDCURSOR */
	r.r_ax = 0x0100;
	if (cur_mode == 7)
		r.r_cx = insert ? 0x0b0c : 0x010c;
	else
		r.r_cx = insert ? 0x0607 : 0x0107;
	intrpt(0x10, &r);
}

# define setcursorstate setcursorstate_ibmpc

#else

static void setcursorstate_ibmpc(int insert)
{
# error need platform-specific setcursorstate()
}

#endif

#define wherex mywherex
#define wherey mywherey
#define _setcursortype my_setcursortype

static int isworddelimiter(unsigned c)
{
	return c == ' ' || c == '\t';
}

#if !defined(DBCS)
/* Print a blank to current cursor postion
	Updates cursor position
 */
static void outblank(void)
{	outc(' ');
}

/* Print string to cursor position and append one blank
	Updates cursor postion
 */
static void outsblank(const char * const s)
{	outs(s);
	outblank();
}
#endif

#if defined(NEC98)
struct skeystate {
	unsigned keynum;
	unsigned char newkey[6];
	unsigned char oldkey[6];
};
static struct skeystate skeys[] = {
	{ 0x19, { 0xff, 0x3a, 0 }, { 0 } },	/* up */
	{ 0x1a, { 0xff, 0x3b, 0 }, { 0 } },	/* left */
	{ 0x1b, { 0xff, 0x3c, 0 }, { 0 } },	/* right */
	{ 0x1c, { 0xff, 0x3d, 0 }, { 0 } },	/* down */
	{ 0x1d, { 0xff, 0x3e, 0 }, { 0 } },	/* HOME CLR */
	{ 0x1e, { 0xff, 0x3f, 0 }, { 0 } },	/* HELP */
};

void prepareSpecialKeys(void)
{
	IREGS r;
	unsigned n;
	for(n=0; n<sizeof(skeys)/sizeof(skeys[0]); ++n) {
		r.r_ax = skeys[n].keynum;
		if (r.r_ax >= 0x15 && r.r_ax <= 0x1f) {
			r.r_cx = 0x0c;
			r.r_ds = FP_SEG(&(skeys[n].oldkey[0]));
			r.r_dx = FP_OFF(&(skeys[n].oldkey[0]));
			intrpt(0xdc, &r);
			r.r_cx = 0x0d;
			r.r_ax = skeys[n].keynum;
			r.r_ds = FP_SEG(&(skeys[n].newkey[0]));
			r.r_dx = FP_OFF(&(skeys[n].newkey[0]));
			intrpt(0xdc, &r);
		}
	}
}

void restoreSpecialKeys(void)
{
	IREGS r;
	unsigned n;
	for(n=0; n<sizeof(skeys)/sizeof(skeys[0]); ++n) {
		r.r_ax = skeys[n].keynum;
		if (r.r_ax >= 0x15 && r.r_ax <= 0x1f) {
			r.r_cx = 0x0d;
			r.r_ds = FP_SEG(&(skeys[n].oldkey[0]));
			r.r_dx = FP_OFF(&(skeys[n].oldkey[0]));
			intrpt(0xdc, &r);
		}
	}
}
#endif

#if defined(DBCS)
# include "cmdinp_d.c" /* DBCS version of readcommandEnhanced */
#else

static void clrcmdline(char * const str, const unsigned pos, const int maxlen)
{
	size_t len = strlen(str);
	int x = wherex();
	int y = wherey();

	if(len > 0) {
		unsigned step;

		for(step = pos; step > 0; step--) {
			x--;
			if(x<0) {
				y--;
				x=SCREEN_COLS-1;
			}
		}

		assert(str);
		goxy(x, y);
		memset(str, ' ', len);
		dos_write(1, str, len);
		goxy(x, y);
		memset(str, 0, maxlen);
	}
}

/* calculates the number of lines to scroll if cursor is initially at position
   curx,cury and cursor is advanced numchars characters */
static unsigned numLinesToScroll(const unsigned curx, unsigned cury, const unsigned numchars)
{
	cury += (curx + numchars - 1) / SCREEN_COLS;
	return (cury <= SCREEN_ROWS) ? 0 : cury - SCREEN_ROWS;
}

/* read in a command line */
void readcommandEnhanced(char * const str, const int maxlen)
{
	unsigned char insert = 1;
	unsigned ch;
#ifdef FEATURE_FILENAME_COMPLETION
	unsigned lastch = 0;
#endif
#ifdef FEATURE_HISTORY
	int histLevel = 0;
	char prvLine[MAX_INTERNAL_COMMAND_SIZE] = { '\0' };
#endif
	unsigned curx;
	unsigned cury;
	unsigned lines;
	int count;
	unsigned current = 0;
	unsigned charcount = 0;

	assert(str);
	assert(maxlen <= MAX_INTERNAL_COMMAND_SIZE);

	/* if echo off, don't print prompt */
	if(echo)
		printprompt();

	orgx = wherex();
	orgy = wherey();

	memset(str, 0, maxlen);

	setcursorstate(insert);

#ifdef FEATURE_HISTORY
	histGet(histLevel - 1, prvLine, sizeof(prvLine));
#endif
#if defined(NEC98)
	prepareSpecialKeys();
#endif

	do {
		ch = cgetchar();

		if(cbreak)
			ch = KEY_CTL_C;

		switch(ch) {
		case KEY_BS:               /* delete character to left of cursor */

			if(current > 0 && charcount > 0) {
			  if(current == charcount) {     /* if at end of line */
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

		case KEY_INSERT:           /* toggle insert/overstrike mode */
			insert ^= 1;
			setcursorstate(insert);
			break;

		case KEY_DELETE:           /* delete character under cursor */

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

		case KEY_HOME:             /* goto beginning of string */

			if (current != 0)
			{
			  goxy(orgx, orgy);
			  current = 0;
			}
			break;

		case KEY_END:              /* goto end of string */

			if (current != charcount)
			{
			  goxy(orgx, orgy);
			  outs(str);
			  current = charcount;
			}
			break;

#ifdef FEATURE_FILENAME_COMPLETION
		case KEY_TAB:		 /* expand current file name */
			if(current == charcount) {      /* only works at end of line */
			  if(lastch != KEY_TAB) { /* if first TAB, complete filename */
				complete_filename(str, charcount);
				charcount = strlen(str);
				current = charcount;
				goxy(orgx, orgy);
				outs(str);
				orgy -= numLinesToScroll(orgx, orgy, charcount);
			  } else {                 /* if second TAB, list matches */
				if (show_completion_matches(str, charcount))
				{
				  printprompt();
				  orgx = wherex();
				  orgy = wherey();
				  outs(str);
				  orgy -= numLinesToScroll(orgx, orgy, charcount);
				}
			  }
			}
			else
			  beep();
			break;
#endif

		case KEY_ENTER:            /* end input, return to main */
			if (current != charcount) {
				/* make sure we locate cursor to end of command before 
				   spawning process */
				goxy(orgx, orgy);
				outs(str);
			}

#ifdef FEATURE_HISTORY
			if(str[0])
			  histSet(0, str);      /* add to the history */
#endif

			outc('\n');
			break;

		case KEY_CTL_C:       		/* ^C */
		case KEY_ESC:              /* clear str  Make this callable! */

			clrcmdline(str, current, maxlen);
			current = charcount = 0;

			if(ch == KEY_CTL_C && !echo) {
			  /* enable echo to let user know that's this
				is the command line */
			  echo = 1;
			  printprompt();
			}
			break;

		case KEY_RIGHT:            /* move cursor right */

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
		case KEY_F1:       /* get character from last command buffer */
			  if (current < strlen(prvLine)) {
				 outc(str[current] = prvLine[current]);
				 charcount = ++current;
			  }
			  break;
			  
		case KEY_F3:               /* get previous command from buffer */
			if(charcount < strlen(prvLine)) {
				outs(strcpy(&str[charcount], &prvLine[charcount]));
			   current = charcount = strlen(str);
				orgy -= numLinesToScroll(orgx, orgy, charcount);
		   }
		   break;

		case KEY_UP:               /* get previous command from buffer */
			if(!histGet(--histLevel, prvLine, sizeof(prvLine)))
				++histLevel;		/* failed -> keep current command line */
			else {
				clrcmdline(str, current, maxlen);
				strcpy(str, prvLine);
				current = charcount = strlen(str);
				orgy -= numLinesToScroll(orgx, orgy, charcount);
				outs(str);
				histGet(histLevel - 1, prvLine, sizeof(prvLine));
			}
			break;

		case KEY_DOWN:             /* get next command from buffer */
			if(histLevel) {
				clrcmdline(str, current, maxlen);
				strcpy(prvLine, str);
				histGet(++histLevel, str, maxlen);
				current = charcount = strlen(str);
				orgy -= numLinesToScroll(orgx, orgy, charcount);
				outs(str);
			}
			break;

		case KEY_F5: /* keep cmdline in F3/UP buffer and move to next line */
			strcpy(prvLine, str);
			clrcmdline(str, current, maxlen);
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

#endif

		case KEY_LEFT:             /* move cursor left */
			if(current > 0) {
			  current--;
			  if (wherex() == 1)
				goxy(MAX_X, wherey() - 1);
			  else
				goxy(wherex() - 1, wherey());
			}
			break;

#if defined(KEY_CTRL_LEFT)
		case KEY_CTRL_LEFT:	/* move cursor left to begin of word */
			while(current > 0) {
			  current--;
			  if (wherex() == 1)
				goxy(MAX_X, wherey() - 1);
			  else
				goxy(wherex() - 1, wherey());

			  if(isworddelimiter(str[current-1])	/* ignore current == 0 */
			   && !isworddelimiter(str[current]))
			     break;
			}
			break;
#endif

#if defined(KEY_CTRL_RIGHT)
		case KEY_CTRL_RIGHT:	/* move cursor right to begin of word */
			while(current < charcount) {
			  current++;
			  if (wherex() == MAX_X)
				goxy(1, wherey() + 1);
			  else
				goxy(wherex() + 1, wherey());

			  if(isworddelimiter(str[current-1])
			   && !isworddelimiter(str[current]))
			     break;
			}
			break;
#endif

		default:                 /* insert character into string... */

			if ((ch >= 32 && ch <= 255) && (charcount != (maxlen - 2)))
			{
			  if (insert && current != charcount)
			  {
				for (count = charcount; count > current; count--)
				  str[count] = str[count - 1];
				str[current++] = ch;
				charcount++;
				curx = wherex() + 1;
				cury = wherey();
				if (curx > MAX_X) { curx = 1; cury++; };
				outs(&str[current - 1]);
				lines = numLinesToScroll(orgx, orgy, charcount);
				cury -= lines;
				orgy -= lines;
				goxy(curx, cury);
			  }
			  else
			  {
				if (current == charcount)
				  charcount++;
				str[current++] = ch;
				curx = wherex();
				cury = wherey();
				outc(ch);
				lines = numLinesToScroll(orgx, orgy, charcount);
				orgy -= lines;
			  }
			}
			else
			  beep();
			break;
		}
#ifdef FEATURE_FILENAME_COMPLETION
		lastch = ch;
#endif
	} while(ch != KEY_ENTER);

#if defined(NEC98)
	restoreSpecialKeys();
#endif
	setcursorstate(insert);
}
#endif /* !DBCS */

#endif /* defined FEATURE_ENHANCED_INPUT */

