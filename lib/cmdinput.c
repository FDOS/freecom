/*
 *  CMDINPUT.C - handles command input (tab completion, history, etc.)
 */

#include "../config.h"

#include <assert.h>
#include <conio.h>
#include <dir.h>
#include <stdlib.h>
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
static void recalcOrgXY(unsigned charcount)
{	int lines;

	assert(orgx > 0);
	lines = (charcount + orgx - 1) / MAX_X;
	while(orgy > 1 && orgy + lines - 1 > MAX_Y)
		--orgy;
}
static void gochar(unsigned char current)
{	div_t x;

	x = div(current + orgx - 1, MAX_X);
	goxy(x.rem + 1, orgy + x.quot);
}

/* read in a command line */
#pragma argsused
char *readcommandEnhanced(void)
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

	if(feof(stdin))
		return 0;

	do {
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
			if(!StrCat(str, "echo q >q")) {
				error_out_of_memory();
				break;
			}
			charcount = strlen(str);
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
				charcount = strlen(str);
				current = end - str;

				goxy(orgx, orgy);
				outs(str);
				recalcOrgXY(charcount);
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

		case KEY_UP:               /* get previous command from buffer */
			if(!histGet(--histLevel, &prvLine))
				++histLevel;		/* failed -> keep current command line */
			else {
				clrcmdline(str, orgx, orgy);
				chkPtr(str);
				StrRepl(str, prvLine);
				prvLine = 0;
				current = charcount = Strlen(str);
				outs(str);
				recalcOrgXY(charcount);
				histGet(histLevel - 1, &prvLine);
			}
			break;

		case KEY_DOWN:             /* get next command from buffer */
			if(histLevel) {
				clrcmdline(str, orgx, orgy);
				chkPtr(prvLine);
				StrRepl(prvLine, str);
				str = 0;
				histGet(++histLevel, &str);
				current = charcount = Strlen(str);
				outs(str);
				recalcOrgXY(charcount);
			}
			break;

		case KEY_F5: /* keep cmdline in F3/UP buffer and move to next line */
			clrcmdline(str, orgx, orgy);
			chkPtr(prvLine);
			StrRepl(prvLine, str);
			str = 0;
			current = charcount = 0;
			outc('@');
			if(orgy >= MAX_Y) {
				outc('\n');			/* Force scroll */
				orgy = MAX_Y;
			} else {
				++orgy;
			}

			break;

#endif

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

#ifdef FEATURE_HISTORY
	myfree(prvLine);
#endif
	return str? StrTrim(str): estrdup("");
}
