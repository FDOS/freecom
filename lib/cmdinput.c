/*
 *  CMDINPUT.C - handles command input (tab completion, history, etc.)
 */

#include "../config.h"

#include <assert.h>
#include <conio.h>
#include <dir.h>
#include <string.h>

#include <dynstr.h>
#include <suppl.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/keys.h"
#include "../include/misc.h"
#include "../err_fcts.h"

static unsigned orgx, orgy;		/* start of current line */
#ifndef FEATURE_HISTORY
static char *prvLine = 0;
#endif

/* Print a character to current cursor position
	Updates cursor postion
 */
static void outc(char c)
{
	putchar(c);
}
/* Print a blank to current cursor postion
	Updates cursor position
 */
static void outblank(void)
{	outc(' ');
}

/* Print string to current cursor position
	Updates cursor position
 */
static void outs(const char * const s)
{
	if(s)	fputs(s, stdout);
}
/* Print string to cursor position and append one blank
	Updates cursor postion
 */
static void outsblank(const char * const s)
{	outs(s);
	outblank();
}
static void recalcOrgXY(unsigned charcount)
{	int lines;

	assert(orgx > 0);
	lines = (charcount + orgx - 1) / MAX_X;
	while(orgy > 1 && orgy + lines - 1 > MAX_Y)
		--orgy;
}
static void gochar(unsigned current)
{	div_t x;

	x = div(current + orgx - 1, MAX_X);
	goxy(x.rem + 1, orgy + x.quot);
}

#ifdef FEATURE_HISTORY
static void switchHistlevel(
	 char ** const Xstr
	, char ** const prvLine
	, unsigned * const current
	, unsigned * const charcount
	, unsigned * const histLevel
	, int direction)
{	unsigned newHistLevel;
	char *str;

	assert(histLevel);
	assert(charcount);
	assert(current);
	assert(prvLine);
	assert(Xstr);

	newHistLevel = *histLevel;
	if(!addu(&newHistLevel, direction)	/* underflow --> ignore */
	 && CTXT_INFO(CTXT_TAG_HISTORY, nummin) < newHistLevel) {
			/* save the current line to the ctxt  */
		histSet(*histLevel, str = *Xstr);
		*prvLine = ctxtAddress(CTXT_TAG_HISTORY, *histLevel);
			/* remove the string from the screen & heap */
		clrcmdline(str, orgx, orgy);
		chkPtr(str);
		StrFree(str);
			/* Duplicate the previous cmd line.
				On error: At this location a line should be
					existing <-> ignore errors
					e.g. if out-of-mem: proceed with empty line and replece
						the long line with the newly added one */
		if(direction)
			ctxtGet(2, CTXT_TAG_HISTORY, *histLevel = newHistLevel, &str);
		else {		/* F5 pressed */
			outc('@');
			if(orgy > MAX_Y) {
				outc('\n');			/* Force scroll */
				orgy = MAX_Y + 1;	/* MAX_Y is zero based, orgy not */
			} else {
				++orgy;
			}
		}
		*current = *charcount = Strlen(str);
		outs(str);
		recalcOrgXY(*charcount);
		*Xstr = str;
	}
}
#endif

/* read in a command line */
#pragma argsused
char *readcommandEnhanced(void)
{
	FLAG insert = 1;
	unsigned ch;
#ifdef FEATURE_FILENAME_COMPLETION
	unsigned lastch = 0;
#endif
	unsigned current = 0;
	unsigned charcount = 0;
	char *str = 0;				/* current line */

#ifdef FEATURE_HISTORY
	char *prvLine = 0;			/* previous line */
	unsigned histLevel;
#endif

#ifdef FEATURE_HISTORY
	if((histLevel = CTXT_INFO(CTXT_TAG_HISTORY, nummax)) != 0)
		/* fetch last entry in stack */
		prvLine = ctxtAddress(CTXT_TAG_HISTORY, histLevel);
	++histLevel;			/* start with a new entry */
#endif

redo:
#ifdef DEBUG
	/* In the first line, the status information is displayed */
	if(wherey() == 1)
		putchar('\n');
#endif
	/* if echo off, don't print prompt */
	if(lflag_echo)
		printprompt();

	orgx = wherex();
	orgy = wherey();

	_setcursortype(_NORMALCURSOR);

	do {
#ifdef DEBUG
		{	int x,y;
		gochar(current);
		x = wherex();
		y = wherey();
		goxy(1,1);
		cprintf(">> cur: %4u count: %4u org: %2u/%2u cur: %2u/%2u max: %2u/%2u echo: %c  <<"
		 , current, charcount, orgx, orgy, x, y, MAX_X, MAX_Y, lflag_echo? 'Y': 'N');
		}
#endif
		gochar(current);
		ch = cgetchar();
		assert(current <= charcount);
		assert(charcount == 0 || str != 0);

		if(cbreak)
			ch = KEY_CTL_C;

		switch(ch) {
		case KEY_INSERT:           /* toggle insert/overstrike mode */
			insert ^= 1;
			if(insert)
				_setcursortype(_NORMALCURSOR);
			else
				_setcursortype(_SOLIDCURSOR);
			break;

		case KEY_BS:               /* delete character to left of cursor */
			if(!current)
				break;
			outc('\b');
			--current;				/* character to be deleted */
			/**FALL THROUGH**/

		case KEY_DELETE:           /* delete character under cursor */
			if(current != charcount && charcount > 0) {
					/* Because current != charcount && current <= charcount
						==> charcount - current >! 0 */
				memmove(&str[current], &str[current + 1]
				 , charcount - current + 1);
				charcount--;
				outsblank(&str[current]);
				gochar(current);
			}
			break;

		case KEY_HOME:             /* goto beginning of string */

			if(current != 0) {
				current = 0;
			}
			break;

#ifdef DEBUG
		case KEY_CTL_T:
			chkPtr(str);
			//if(!StrCat(str, "€‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷ùúûüýþÿ")) {
			if(!StrCat(str, "echo 1 |& echo 2 |& echo 3")) {
				error_out_of_memory();
				break;
			}
			charcount = strlen(str);
			recalcOrgXY(charcount);
			/** FALL THROUGH **/
#endif
		case KEY_END:              /* goto end of string */

			if(current != charcount) {
				goxy(orgx, orgy);
				outs(str);
				current = charcount;
			}
			break;

#ifdef FEATURE_FILENAME_COMPLETION
		case KEY_TAB:		 /* expand current file name */
			{	char *p, *end;
				unsigned len;	/* size of area behind end */

				if((p = erealloc(str, charcount + MAXPATH + 3)) == 0)
					break;
				if(!str || !charcount)
					p[current = charcount = 0] = 0;
				str = p;
				str[charcount] = 0;
				/* limit the filename */
				for(p = &str[current]; --p >= str && is_pathchar(*p); );
				++p;				/* start of string */
				end = skippath(p);
				len = &str[charcount] - end;
				if(len) {
					if(len >= MAXPATH) {
						error_long_internal_line();
						*str = 0;
						break;
					}
					memmove(&p[MAXPATH], end, len);
				}
				*end = 0;		/* make the name a C-string */

				if(lastch != KEY_TAB) { /* if first TAB, complete filename */
					complete_filename(p, MAXPATH);
				} else if(show_completion_matches(p, MAXPATH)) {
					printprompt();
					orgx = wherex();
					orgy = wherey();
				}
				end = strchr(p, '\0');
				if(end > &p[MAXPATH]) {
					error_corrupt_command_line();
					*str = 0;
					str = StrTrim(str);
					break;
				}
				if(len) {
					memmove(end, &p[MAXPATH], len);
					end[len] = 0;
				}
				current = end - str;
				charcount = strlen(str);

				goxy(orgx, orgy);
				outs(str);
				recalcOrgXY(charcount);
			}
			break;
#endif

		case KEY_ENTER:            /* end input, return to main */

#ifdef FEATURE_HISTORY
			histSet(histLevel, str);      /* add to the history */
#endif

			outc('\n');
			break;

		case KEY_CTL_C:       		/* ^C */
		case KEY_ESC:              /* clear str  Make this callable! */

			clrcmdline(str, orgx, orgy);
			current = charcount = 0;

			if(ch == KEY_CTL_C && !lflag_echo) {
			  /* enable echo to let user know that's this
				is the command line */
			  gflag_echo = lflag_echo = gflag_dispPrompt = 1;
			  goto redo;
			}
			break;

		case KEY_RIGHT:            /* move cursor right */

			if(current != charcount) {
				current++;
				break;
			}
			/* cursor-right at end of string grabs the next character
				from the previous line */
			/* FALL THROUGH */

		case KEY_F1:       /* get character from last command buffer */
			if(current < Strlen(prvLine)) {
				if(current != charcount) {
					assert(str);
					outc(str[current] = prvLine[current]);
					++current;
				} else {		/* append the character */
					if(!StrAppChr(str, prvLine[current])) {
						error_out_of_memory();
					} else {
						assert(str);
						outc(str[current]);
						charcount = ++current;
					}
				}
			}
			break;
			  
		case KEY_F3:               /* get previous command from buffer */
			if(current < Strlen(prvLine)) {
				if(charcount) {
					assert(str);
					str[current] = 0;
				}
				chkPtr(str);
				if(!StrCat(str, prvLine))
					error_out_of_memory();
				else {
					outs(&str[current]);
					current = charcount = strlen(str);
					recalcOrgXY(charcount);
				}
			}
			break;

#ifdef FEATURE_HISTORY
		case KEY_UP:               /* get previous command from buffer */
			switchHistlevel(&str, &prvLine
			 , &current, &charcount, &histLevel, -1);
			break;

		case KEY_DOWN:             /* get next command from buffer */
			switchHistlevel(&str, &prvLine
			 , &current, &charcount, &histLevel, 1);
			break;
#endif

		case KEY_F5: /* keep cmdline in F3/UP buffer and move to next line */
#ifdef FEATURE_HISTORY
			switchHistlevel(&str, &prvLine
			 , &current, &charcount, &histLevel, 0);
#else
			if(str) {
				/* Clear the string on the screen */
				goxy(orgx, orgy);
				fputmc(' ', charcount + 1, stdout);
			}
			myfree(prvLine);
			prvLine = str;
			str = 0;
			current = charcount = 0;
			goxy(orgx, orgy);
			outc('@');
			if(orgy > MAX_Y) {
				outc('\n');			/* Force scroll */
				orgy = MAX_Y + 1;	/* MAX_Y is zero based, orgy not */
			} else
				++orgy;
#endif
			break;

		case KEY_LEFT:             /* move cursor left */

			if(current > 0) {
			  current--;
			}
#if 0
			else
				   beep();
#endif
			break;

		default:

			if(ch >= 32 && ch <= 255) {
				if(insert || current == charcount) {
					/* insert or append character into string... */
					if(!StrAppChr(str, ch))
						error_out_of_memory();
					else {
						if(insert && current != charcount) {
							memmove(&str[current + 1]
							 , &str[current], charcount - current);
							str[current] = ch;
						}
						outs(&str[current]);
						recalcOrgXY(++charcount);
						++current;
					}
				} else {
					/* Overwrite character */
					outc(str[current++] = ch);
				}
			} else
				beep();
			break;
		}
#ifdef FEATURE_FILENAME_COMPLETION
		lastch = ch;
#endif
	} while(ch != KEY_ENTER);

	_setcursortype(_NORMALCURSOR);

#ifndef FEATURE_HISTORY
	if(str) {
		myfree(prvLine);
		prvLine = strdup(str);
	}
#endif

	return str? StrTrim(str): estrdup("");
}
