/*
 * MISC.C -- Misc. Functions
 *
 * 07/12/98 (Rob Lake)
 *  started
 *
 * 07/13/98 (Rob Lake)
 *  moved functions in here
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 09-Aug-1998 (Rob Lake <rlake@cs.mun.ca>)
 * - changed split function
 * - added freep function
 *
 * 10-Aug-1998 ska
 * - fix someone removed the ^Break catcher --> readded, this time using
 *   ANSI-like (DOS emulation) of signal handling functions
 *
 * 09-Sep-1998 ska
 * - fix/chg: ^Break handler now handles external program properly
 *
 * 1998/12/04 ska
 * - chg: vcgetchar() moved here
 * - add: vcgetcstr() gets visual user input and validates it
 *
 * 1999/01/24 ska
 * bugfix: split(): the loop skipping over leading whitespaces crashed
 *  at end of string ('\0') (reported by Eric Kohl <ekohl@abo.rhein-zeitung.de>)
 *
 * 1999/04/23 ska
 * add: match(), matchtok(), match_() test if a word
 *  begins a line, if so, skip over it and following spaces
 * add: comFile(): returns the absolute filename of COMMAND.COM
 * add: comPathFile(): returns the name of a file located in the
 *  directory as COMMAND.COM
 * add: ltrim() / rtrim()
 * chg: ltrim() never returns NULL
 *
 * 1999/04/27 ska
 * chg: moved match_(), ltrim(), rtrim(), trim(), split(), freep()
 *  into CMDLINE.C
 *
 * 1999/07/02 ska
 * chg: removed exist(), replaced by dfnstat() [reduces size of image]
 *
 *	2000/06/22 ska
 *	add: cwd(int drive); changeDrive(int drive), drvNum(int drive)
 *	add: onoffStr()
 */

#include "config.h"

#include <assert.h>
#include <conio.h>
#include <ctype.h>
#include <dir.h>
#include <direct.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "command.h"
#include "batch.h"
#include <dfn.h>

#include "strings.h"

/*
 * get a character out-of-band and honor Ctrl-Break characters
 */
int cgetchar(void)
{
  int c;

  if ((c = getch()) == 0)
    c = getch() << 8;

  if (c == CTL_C)
    ctrlBreak = 1;

  return c;
}

/* Visible cgetchar() that does not advance the cursor */
int vcgetchar(void)
{
  int ch;

  ch = cgetchar();
  putchar(isprint(ch) ? ch : ' ');
  putchar('\b');
  fflush(stdout);

  return ch;
}

/*
 *  get a visual character that must match one of the supplied
 *  ones
 */
int vcgetcstr(const char *const legalCh)
{
  int ch;

  assert(legalCh);

  fflush(stdout);               /* Make sure the pending output arrives the
                                   screen as we bypass the stdio interface */
  while ((ch = toupper(vcgetchar())) == 0
         || !strchr(legalCh, ch))
  {
    if (cbreak)
    {
      ch = CTL_C;                /* This is ^C for ^Break */
      break;
    }
    beep();                     /* hit erroreous character */
  }
  putchar('\n');                /* advance to next line */
  return ch;
}

/*
 * Check if Ctrl-Break was pressed during the last calls
 */
int chkCBreak(int mode)
{
  static int leaveAll = 0;      /* leave all batch files */
  int c;

  switch (mode)
  {
    case BREAK_ENDOFBATCHFILES:
      leaveAll = 0;
      return 0;

    case 0:
      if (!bc)
        goto justCheck;

    case BREAK_BATCHFILE:
      if (leaveAll)
        return 1;
      if (!ctrlBreak)
        return 0;

      /* we need to be sure the string arrives on the screen! */
      do
        cprintf("\r\nCtrl-Break pressed.\r\nCancel batch file '%s'? (Yes/No/All) ", bc && bc->bfnam ? bc->bfnam : "");
      while (!strchr("YNA\3", c = toupper(cgetchar())) || !c);

      cputs("\r\n");

      if (c == 'N')
        return ctrlBreak = 0;   /* ignore */

      leaveAll = c == 'A' || c == CTL_C; /* leave all batch files */

      break;

    justCheck:
    case BREAK_FORCMD:         /* FOR commands are part of batch processing */
      if (leaveAll)
        return 1;
      /* fall through */

    case BREAK_INPUT:
      if (!ctrlBreak)
        return 0;
      break;
  }

  ctrlBreak = 0;                /* state processed */
  return 1;
}

/*
 * Internally BEEP (getting the user's attention)
 * Note: One should consider implementing a _visual_ beep
 *   rather an audible beep and an option to switch between them.
 */
void beep(void)
{
  sound(900);
  delay(400);
  nosound();
  delay(100);
}

void beep_low(void)
{
  sound(900);
  delay(400);
  nosound();
  delay(100);
}


/*
 *  Return the absolute filename of the current COMMAND shell
 */
char *comFile(void)
{ char *fnam;

  if((fnam = getEnv("COMSPEC")) == NULL)
    return ComPath;
  return fnam;
}

/*
 * Construct the path of a file to be located in the same directory
 *  as COMMAND.
 *  Dynamically allocated.
 */
char *comPathFile(const char * fnam)
{ char *com, *h;
  int pathLen;

  assert(fnam);

  if((com = comFile()) == NULL)
    return strdup(fnam);

  h = malloc((pathLen = dfnfilename(com) - com) + strlen(fnam) + 1);
  if(!h)
    return NULL;

  memcpy(h, com, pathLen);
  strcpy(h + pathLen, fnam);

  return h;
}

/*
 * Display a count
 */
void dispCount(int cnt, const char * const zero, const char * const one
 , const char * const multiple)
{ switch(cnt) {
  case 0: fputs(zero, stdout); break;
  case 1: fputs(one, stdout); break;
  default: printf(multiple, cnt); break;
  }
}

int drvNum(int drive)
{
	if(drive == 0)		/* change to current drive */
		return getdisk();
	if(drive <= 32)
		return drive - 1;
	return toupper(drive) - 'A';
}

/*
 *	Retreive the current working directory including drive letter
 *	Returns in a dynamically allocated buffer (free'ed by the caller)
 *	on error: Displays "out of memory"
 */
char *cwd(int drive)
{	char *h;

	if((h = dfnpath(drive)) != NULL)
		return h;

	if(drive)
		error_no_cwd(drive);
	else error_out_of_memory();

	return NULL;
}

int changeDrive(int drive)
{	drive = drvNum(drive);

    setdisk(drive);

    if (getdisk() == drive)
    	return 0;

  displayString(TEXT_ERROR_INVALID_DRIVE, drive + 'A');

  return 1;
}

/*
 *	Tests if a string is ON or OFF
 */
enum OnOff onoffStr(char *line)
{
	if(!line)
		return OO_Null;
	if(!*line)
		return OO_Empty;
  if (stricmp(line, D_OFF) == 0)
  	return OO_Off;
  if (stricmp(line, D_ON) == 0)
    return OO_On;

return OO_Other;
}

