/*
 *  CMDINPUT.C - handles command input (tab completion, history, etc.)
 */

#include "../config.h"

#include <assert.h>
#include <conio.h>
#include <dir.h>
//#include <dos.h>
#include <stdlib.h>
//#include <stdio.h>
#include <string.h>

#include <dynstr.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/keys.h"
#include "../include/misc.h"
#include "../err_fcts.h"

static unsigned orgx, orgy;		/* start of current line */

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

/* read in a command line */
#pragma argsused
char *readcommandEnhanced(ctxtEC_t far * const ctxt)
{
	unsigned char insert = 1;
	unsigned ch;
#ifdef FEATURE_FILENAME_COMPLETION
	unsigned lastch = 0;
#endif
#ifdef FEATURE_HISTORY
	int histLevel = 0;
	char *prvLine = 0;
#endif
	unsigned curx;
	unsigned cury;
	unsigned current = 0;
	unsigned charcount = 0;
	char *str = 0;

	/* if echo off, don't print prompt */
	if(dispPrompt)
		printprompt();

	orgx = wherex();
	orgy = wherey();

	_setcursortype(_NORMALCURSOR);

#ifdef FEATURE_HISTORY
	histGet(histLevel - 1, &prvLine);
#endif

	do {
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
				curx = wherex();
				cury = wherey();
				outsblank(&str[current]);
				goxy(curx, cury);
			}
			break;

		case KEY_HOME:             /* goto beginning of string */

			if(current != 0) {
				goxy(orgx, orgy);
				current = 0;
			}
			break;

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
				for(p = &str[current]
				 ; --p >= str && (is_fnchar(*p) || is_pathdelim(*p))
				 ; );
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
				charcount = strlen(str);

				goxy(orgx, orgy);
				outs(str);
				if((strlen(str) > (MAX_X - orgx)) && (orgy == MAX_Y + 1))
				  orgy--;
			}
			break;
#endif

		case KEY_ENTER:            /* end input, return to main */

#ifdef FEATURE_HISTORY
			if(charcount)
			  histSet(0, str);      /* add to the history */
#endif

			outc('\n');
			break;

		case KEY_CTL_C:       		/* ^C */
		case KEY_ESC:              /* clear str  Make this callable! */

			clrcmdline(str, orgx, orgy);
			current = charcount = 0;

			if(ch == KEY_CTL_C && !dispPrompt) {
			  /* enable echo to let user know that's this
				is the command line */
			  dispPrompt = F(dispPrompt) = 1;
			  printprompt();
			}
			break;

		case KEY_RIGHT:            /* move cursor right */

			if(current != charcount) {
				current++;
				if(wherex() == MAX_X)
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
			if(current < Strlen(prvLine)) {
				if(current != charcount) {
					assert(str);
					outc(str[current] = prvLine[current]);
					++current;
				} else {		/* append the character */
					if(!StrAppChr(str, prvLine[current])) {
						error_out_of_memory();
					} else
						charcount = ++current;
				}
			}
			break;
			  
		case KEY_F3:               /* get previous command from buffer */
			if(current < Strlen(prvLine)) {
				if(charcount)
					str[current] = 0;
				if(!StrCat(str, prvLine))
					error_out_of_memory();
				else {
					outs(&str[current]);
					current = charcount = strlen(str);
				}
			}
			break;

		case KEY_UP:               /* get previous command from buffer */
			if(!histGet(--histLevel, &prvLine))
				++histLevel;		/* failed -> keep current command line */
			else {
				clrcmdline(str, orgx, orgy);
				StrRepl(str, prvLine);
				prvLine = 0;
				current = charcount = Strlen(str);
				outs(str);
				histGet(histLevel - 1, &prvLine);
			}
			break;

		case KEY_DOWN:             /* get next command from buffer */
			if(histLevel) {
				clrcmdline(str, orgx, orgy);
				StrRepl(prvLine, str);
				str = 0;
				histGet(++histLevel, &str);
				current = charcount = Strlen(str);
				outs(str);
			}
			break;

		case KEY_F5: /* keep cmdline in F3/UP buffer and move to next line */
			clrcmdline(str, orgx, orgy);
			StrRepl(prvLine, str);
			str = 0;
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
			  if(wherex() == 1)
				goxy(MAX_X, wherey() - 1);
			  else
				goxy(wherex() - 1, wherey());
			}
#if 0
			else
				   beep();
#endif
			break;

		default:                 /* insert character into string... */

			if(ch >= 32 && ch <= 255) {
				if(!StrAppChr(str, ch))
					error_out_of_memory();
				else {
					if(insert && current != charcount) {
						memmove(&str[current + 1]
						 , &str[current], charcount - current);
						str[current] = ch;
					}
					curx = wherex() + 1;
					cury = wherey();
					outs(&str[current++]);
					if((strlen(str) > (MAX_X - orgx)) && (orgy == MAX_Y + 1))
						cury--;
					goxy(curx, cury);
					charcount++;
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

#ifdef FEATURE_HISTORY
	free(prvLine);
#endif
	return StrTrim(str);
}
