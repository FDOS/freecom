/*
 * COMMAND.C - command-line interface.
 *
 * Comments:
 *
 * 17 Jun 1994 (Tim Norman)
 *   started.
 *
 * 08 Aug 1995 (Matt Rains)
 *   I have cleaned up the source code. changes now bring this source
 *   into guidelines for recommended programming practice.
 *
 *   A added the the standard FreeDOS GNU licence test to the
 *   initialize() function.
 *
 *   Started to replease puts() with printf(). this will help
 *   standardize output. please follow my lead.
 *
 *   I have added some constants to help making changes easier.
 *
 * 15 Dec 1995 (Tim Norman)
 *   major rewrite of the code to make it more efficient and add
 *   redirection support (finally!)
 *
 * 6 Jan 1996 (Tim Norman)
 *   finished adding redirection support!  Changed to use our own exec
 *   code (MUCH thanks to Svante Frey!!)
 *
 * 29 Jan 1996 (Tim Norman)
 *   added support for CHDIR, RMDIR, MKDIR, and ERASE, as per suggestion
 *   of Steffan Kaiser
 *
 *   changed "file not found" error message to "bad command or filename"
 *   thanks to Dustin Norman for noticing that confusing message!
 *
 *   changed the format to call internal commands (again) so that if they
 *   want to split their commands, they can do it themselves (none of the
 *   internal functions so far need that much power, anyway)
 *
 *
 * 27 Aug 1996 (Tim Norman)
 *   added in support for Oliver Mueller's ALIAS command
 *
 * 14 Jun 1997 (Steffan Kaiser)
 *   added ctrl-break handling and error level
 *
 * 16 Jun 1998 (Rob Lake)
 *   Runs command.com if /P is specified in command line.  Command.com
 *   also stays permanent.  If /C is in the command line, starts the
 *   program next in the line.
 *
 * 21 Jun 1998 (Rob Lake)
 *   Fixed up /C so that arguments for the program
 *
 * 08-Jul-1998 (John P. Price)
 *   - Now sets COMSPEC environment variable
 *   - misc clean up and optimization
 *   - added date and time commands
 *   - changed to using spawnl instead of exec.  exec does not copy the
 *     environment to the child process!
 *
 * 14 Jul 1998 (Hans B Pufal)
 *   Reorganised source to be more efficient and to more closely follow
 *   MS-DOS conventions. (eg %..% environment variable replacement
 *   works form command line as well as batch file.
 *
 *   New organisation also properly support nested batch files.
 *
 *   New command table structure is half way towards providing a
 *   system in which COMMAND will find out what internal commands
 *   are loaded
 *
 * 24 Jul 1998 (Hans B Pufal) [HBP_003]
 *   Fixed return value when called with /C option
 *
 * 27 Jul 1998  John P. Price
 * - added config.h include
 *
 * 28 Jul 1998  John P. Price
 * - added showcmds function to show commands and options available
 *
 * 07-Aug-1998 (John P Price <linux-guru@gcfl.net>)
 * - Fixed carrage return output to better match MSDOS with echo on or off.
 *   (marked with "JPP 19980708")
 *
 * 10-Aug-1998 ska
 * - fixed exit code of "/C" options
 * - readded ^Break catcher (^Break without catcher doesn't make much sense)
 *
 * 27-Oct-1998 ska
 * - changed: creation of name of temporary file of pipe (tmpnam()), no
 *  need for *tempdir, no need for malloc()'ed fname0&1
 * - bugfix: in pipe "cmd1 | cmd2 | cmd3" when switching to cmd3
 *  the tempfile of cmd2's input is removed, before it's closed.
 *  That's not recommended.
 * - bugfix: 'abort:' label sometimes missed a reset-option
 * - bugfix: moved the removal of fname1 behind the 'close()' of the
 *  opened file.
 *
 * 19-Nov-1998 (Rob Green <robg@sundial.net>)
 * - changed line in is_delim function to include backslash and period.
 *
 * 30-Nov-1998 (Rob Green <robg@sundial.net>)
 * - Removed previous change (Opps)
 *
 * 30-Nov-1998 (John P Price <linux-guru@gcfl.net>)
 * - Added code to remove white space from end of command in docommand
 *   function.  Also added nextcmd char pointer so we don't loose the other
 *   commands in the line.
 *   This fixes the bug with "type filename.c | more"
 *   (marked with "JPP 19981130")
 *
 * 1998/12/04 ska
 * - chg: moved tracemode interaction from  process_input() into
 *  paresecommandline(): This won't miss _any_ commands now and does
 *  now display the line after variable and alias expansion (if ECHO
 *  is ON, this adds an useful amount of information for the tracing
 *  user).
 * - chg: use vcgetcstr() to gather user input.
 * - add: tracemode interaction is now ^Break aware
 *
 * 1998/12/05 ska
 * - add: forceLow variable
 *
 * 1999/01/24 ska
 * add: support for CP/M style device names (openf.h)
 *
 * 09-Feb-1999 (John P Price <linux-guru@gcfl.net>)
 * - changed the spawnl call back to using the exec function.  Now that we
 *   are handling the environment correctly, this seems to work better.  It
 *   passes new environment variables (even ones that were created by this
 *   copy of command.com) to the child process.  This was suggested by ska.
 *
 * 24-Mar-1999 (John P Price <linux-guru@gcfl.net>)
 * - added support for swapable exec.
 * - changed the way we run autoexec.bat or a batch file on the command line.
 *
 * 14-Apr-1999 (John P Price <linux-guru@gcfl.net>)
 * - Changed docommand so it will not allow wildcards in the first argument
 *   (i.e. the command itself).
 *
 * 1999/04/23 ska
 * bugfix: command.c: process_input(): FOR hack: On command line one can
 *  enter "FOR %a IN () DO ...". The current implementation expanded
 *  the %A's.
 *
 * 1999/04/27 ska
 * chg/bugfix: docommand(): MS COMMAND supports that every internal
 *  command can be delimited by any non-filename character,
 *  e.g. "FOR,%a IN (*.*) DO ECHO %a" is a valid command.
 * add: is_delim(): DOS NLS
 *
 * 1999/07/06 ska
 * chg: FreeCom's arguments are parsed by the internal parser
 * add: if FreeCom is about to terminate, but this is forbidden
 *  the program goes into an infinite loop
 * add: session management
 *
 * 2000/01/14 ska
 * chg: made "oldinfd" and "oldoutfd" of parsecommandline() global
 *  variables in order to support the CTTY when used within pipes
 *  or redirections
 *
 * 2000/01/13 ska
 * add: if input stream is empty (EOF) or the NUL device and the shell
 *  is to become interactive; the shell is terminated
 * add: if the shell is about to terminate, but must not (/P switch),
 *  the output is redirected to CON after the infinite loop
 *  hanged ten times. see hangForever()
 *
 * 2000/07/05 Ron Cemer
 *	bugfix: renamed skipwd() -> skip_word() to prevent duplicate symbol
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dos.h>
#include <process.h>
#include <time.h>
#include <errno.h>
#include <dir.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>

#include "command.h"
#include "batch.h"
#include "cmdline.h"
#include "strings.h"
#ifdef FEATURE_NLS
#include "nls.h"
#endif
#include "openf.h"
#include "session.h"
#include "kswap.h"

extern struct CMD cmds[];       /* The internal command table */

int exitflag = 0;               /* indicates EXIT was typed */
int canexit = 1;                /* indicates if this shell is exitable */
int ctrlBreak = 0;              /* Ctrl-Break or Ctrl-C hit */
int errorlevel = 0;             /* Errorlevel of last launched external prog */
int forceLow = 0;               /* load resident copy into low memory */
int oldinfd = -1;       /* original file descriptor #0 (stdin) */
int oldoutfd = -1;        /* original file descriptor #1 (stdout) */
int autofail = 0;				/* Autofail <-> /F on command line */

	/* FALSE: no swap this time
		TRUE: swap this time
		ERROR: no swap avilable at all
	*/
int swapOnExec = FALSE;
	/* if != 0, pointer to static context
		NOT allowed to alter if swapOnExec == ERROR !!
	*/
kswap_p kswapContext = 0;


void fatal_error(char *s)
{
  /*
   *  fatal error handler.
   */

  printf("fatal_error() : %s\n", s);

  exit(100);
}

static int is_delim(int c)
{
  /*
   *  is character a delimeter when used on first word?
   */

#ifdef FEATURE_NLS
  refreshNLS();   /* refresh the NLS information */

#define uc unsigned char
  return c < (uc)nlsBuf->inclFirst || c > (uc)nlsBuf->inclLast
   || (c >= (uc)nlsBuf->exclFirst && c <= (uc)nlsBuf->exclLast)
   || _fmemchr(nlsBuf->illegalChars, c, nlsBuf->illegalLen);
#undef uc
#else
  return c <= ' ' || c == 0x7f || strchr(".\"/\\[]:|<>+=;,", c);
#endif
}

void perform_exec_result(int result)
{
	dprintf(("result of (do_)exec(): %d\n", result));
	if (result == -1)
		perror("executing spawnl function");
	else
		errorlevel = result;

	if(!restoreSession()) {
		error_restore_session();
		exit_all_batch();
	}
}


static void execute(char *first, char *rest)
{
  /*
   * This command (in first) was not found in the command table
   *
   *
   * first - first word on command line
   * rest  - rest of command line
   *
   */

  char *fullname;

  assert(first);
  assert(rest);

  if (strlen(first) + strlen(rest) + 1 > MAX_EXTERNAL_COMMAND_SIZE)
  {
    error_line_too_long();
    return;
  }

  /* check for a drive change */
  if ((strcmp(first + 1, ":") == 0) && isalpha(*first))
  {
  	changeDrive(*first);
    return;
  }

  if (strchr(first,'?') || strchr(first,'*'))
  {
    error_bad_command();
    return;
  }

  /* search through %PATH% for the binary */
  fullname = find_which(first);
  dprintf(("[find_which(%s) returned %s]\n", first, fullname));

  if (!fullname)
  {
    error_bad_command();
    return;
  }

  /* check if this is a .BAT file */
  assert(strrchr(fullname, '.'));

  if (stricmp(strrchr(fullname, '.'), ".bat") == 0)
  {
    dprintf(("[BATCH: %s %s]\n", fullname, rest));
    batch(fullname, first, rest);
  }
  else
    /* exec the program */
  {
    int result;

    dprintf(("[EXEC: %s %s]\n", fullname, rest));
    if(!saveSession()) {
      error_save_session();
      exit_all_batch();
      return;   /* Don't invoke the program in this case */
    }

#ifdef FEATURE_KERNEL_SWAP_SHELL
    if(swapOnExec == TRUE
	 && kswapMkStruc(fullname, rest)) {
	 	dprintf(("[EXEC: exiting to kernel swap support]\n"));
	 	exit(123);		/* Let the kernel swap support do the rest */
	}
#ifdef DEBUG
	if(swapOnExec == TRUE)
		dprintf(("KSWAP: failed to save context, proceed without swapping\n"));
#endif
#endif
    result = exec(fullname, rest, 0);

    perform_exec_result(result);
  }
}

static void docommand(char *line)
{
  /*
   * look through the internal commands and determine whether or not this
   * command is one of them.  If it is, call the command.  If not, call
   * execute to run it as an external program.
   *
   * line - the command line of the program to run
   */

  char *com;                    /* the first word in the command */
  char *cp;
  char *rest;            /* pointer to the rest of the command line */

  struct CMD *cmdptr;

  assert(line);

  /* delete leading & trailing whitespaces */
  line = rest = trim(line);

  if (*rest)                    /* Anything to do ? */
  {
    if ((cp = com = malloc(strlen(line) + 1)) == NULL)
    {
    error_out_of_memory();
    return;
    }

  /* Copy over 1st word as lower case */
  /* Internal commands are constructed out of alphabetic
  	characters; ? had been parsed already */
    while (isalpha(*rest))
      *cp++ = tolower(*rest++);

    if(*rest && (!is_delim(*rest) || strchr(QUOTE_STR, *rest)))
      /* If the first word is quoted, it is no internal command */
      cp = com;   /* invalidate it */
    *cp = '\0';                 /* Terminate first word */

  /* Scan internal command table */
  if(*com)
    for (cmdptr = cmds; cmdptr->name && strcmp(com, cmdptr->name) != 0
     ; cmdptr++);

    if(*com && cmdptr->name) {    /* internal command found */
    free(com);          /* free()'ed during call */
      switch(cmdptr->flags & (CMD_SPECIAL_ALL | CMD_SPECIAL_DIR)) {
      case CMD_SPECIAL_ALL: /* pass everything into command */
        break;
      case CMD_SPECIAL_DIR: /* pass '\\' too */
        if(*rest == '\\') break;
      default:        /* pass '/', ignore ',', ';' & '=' */
        if(*rest == '/') break;
        if(!*rest || isspace(*rest)) {  /* normal delimiter */
          rest = ltrim(rest);
          break;
        }
        if(strchr(",;=", *rest)) {
          rest = ltrim(rest + 1);
          break;
        }

        /* else syntax error */
        error_syntax(NULL);
        return;
      }

        /* JPP this will print help for any command */
        if (strstr(rest, "/?"))
        {
          displayString(cmdptr->help_id);
        }
        else
        {
          dprintf(("CMD '%s' : '%s'\n", com, rest));
          cmdptr->func(rest);
        }
      } else {
        /* no internal command --> spawn an external one */
        free(com);
        com = unquote(line, rest = skip_word(line));
        if(!com) {
          error_out_of_memory();
          return;
        }
    execute(com, ltrim(rest));
      free(com);
      }
  }
}

/*
 * process the command line and execute the appropriate functions
 * full input/output redirection and piping are supported
 *
 */
void parsecommandline(char *s)
{
  char *in = NULL;
  char *out = NULL;
  char *fname0 = NULL;
  char *fname1 = NULL;
  char *nextcmd;

  int of_attrib = O_CREAT | O_TRUNC | O_TEXT | O_WRONLY;
  int num;

  dprintf(("[parsecommandline (%s)]\n", s));

  /* first thing we do is alias expansion */
  assert(s);
  assert(oldinfd == -1);  /* if fails something is wrong; should NEVER */
  assert(oldoutfd == -1); /* happen! -- 2000/01/13 ska*/

#ifdef FEATURE_ALIASES
  aliasexpand(s, MAX_INTERNAL_COMMAND_SIZE);
  dprintf(("[alias expanded to (%s)]\n", s));
#endif

  if (tracemode)
  {                             /* Question after the variables expansion
                                   and make sure _all_ executed commands will
                                   honor the trace mode */
    printf("%s [Enter=Yes, ESC=No] ", s);
    /* If the user hits ^Break, it has the same effect as
       usually: If he is in a batch file, he is asked if
       to abort all the batchfiles or just the current one */
    if (!strchr("Y\r\n", vcgetcstr("\x1bYN\r\n")))
      /* Pressed either "No" or ^Break */
      return;
  }

  num = get_redirection(s, &in, &out, &of_attrib);
  if (num < 0)                  /* error */
    goto abort;

  /* Set up the initial conditions ... */

  if (in || (num > 1))          /* Need to preserve stdin */
    oldinfd = dup(0);

  if (in)                       /* redirect input from this file name */
  {
    close(0);
    if (0 != devopen(in, O_TEXT | O_RDONLY, S_IREAD))
    {
      displayString(TEXT_ERROR_REDIRECT_FROM_FILE, in);
      goto abort;
    }
  }

  if (out || (num > 1))         /* Need to preserve stdout */
    oldoutfd = dup(1);

  /* Now do all but the last pipe command */
  while (num-- > 1)
  {
    close(1);                   /* Close current output file */
    if ((fname0 = tmpfn()) == NULL)
      goto abort;
    open(fname0, O_CREAT | O_TRUNC | O_TEXT | O_WRONLY, S_IREAD | S_IWRITE);

    nextcmd = s + strlen(s) + 1;
    docommand(s);

    close(0);
    killtmpfn(fname1);          /* fname1 can by NULL */
    fname1 = fname0;
    fname0 = NULL;
    open(fname1, O_TEXT | O_RDONLY, S_IREAD);

    s = nextcmd;
  }

  /* Now set up the end conditions... */

  if (out)                      /* Final output to here */
  {
    close(1);
    if (1 != devopen(out, of_attrib, S_IREAD | S_IWRITE))
    {
      displayString(TEXT_ERROR_REDIRECT_TO_FILE, out);
      goto abort;
    }

    if (of_attrib & O_APPEND)
      lseek(1, 0, SEEK_END);

  }
  else if (oldoutfd != -1)      /* Restore original stdout */
  {
    close(1);
    dup2(oldoutfd, 1);
    close(oldoutfd);
    oldoutfd = -1;
  }

  docommand(s);                 /* process final command */

abort:
  if (oldinfd != -1)            /* Restore original STDIN */
  {
    close(0);
    dup2(oldinfd, 0);
    close(oldinfd);
    oldinfd = -1;
  }

  if (oldoutfd != -1)           /* Restore original STDOUT */
  {
    close(1);
    dup2(oldoutfd, 1);
    close(oldoutfd);
    oldoutfd = -1;
  }

  killtmpfn(fname1);
  killtmpfn(fname0);

  if (out)
    free(out);

  if (in)
    free(in);
}

/*
 * do the prompt/input/process loop
 *
 *  If xflg is true, the function will not go interactive, but returns.
 *  If commandline != NULL, this command is processed first.
 *
 *  Return: 0: on success
 */
int process_input(int xflag, char *commandline)
{
    /* Dimensionate parsedline that no sprintf() can overflow the
      buffer */
  char parsedline[MAX_INTERNAL_COMMAND_SIZE + sizeof(errorlevel) * 8]
    , *readline;
/* Return the maximum pointer into parsedline to add 'numbytes' bytes */
#define parsedMax(numbytes)   \
  (parsedline + MAX_INTERNAL_COMMAND_SIZE - 1 - (numbytes))
  char *evar;
  char *tp;
  char *ip;
  char *cp;
  char forvar;
  int echothisline;
  int tracethisline;

  do
  {
  	echothisline = tracethisline = 0;
    if(commandline) {
      ip = commandline;
      readline = commandline = NULL;
    } else {
    if ((readline = malloc(MAX_INTERNAL_COMMAND_SIZE + 1)) == NULL)
    {
      error_out_of_memory();
      return 1;
    }

      if (NULL == (ip = readbatchline(&echothisline, readline,
                      MAX_INTERNAL_COMMAND_SIZE)))
      { /* if no batch input then... */
      if (xflag   /* must not go interactive */
       || (fdattr(0) & 0x84) == 0x84  /* input is NUL device */
       || feof(stdin))    /* no further input */
      {
        free(readline);
        break;
      }

      /* Go Interactive */
      readcommand(ip = readline, MAX_INTERNAL_COMMAND_SIZE);
      tracemode = 0;          /* reset trace mode */
      }
    }

    /* 
     * The question mark '?' has a double meaning:
     *	C:\> ?
     *		==> Display short help
     *
     *	C:\> ? command arguments
     *		==> enable tracemode for just this line
     */
    if(*(ip = trim(ip)) == '?') {
    	 ip = trim(ip + 1);
    	 if(!*ip) {		/* is short help command */
#ifdef INCLUDE_CMD_QUESTION
    	 	showcmds(ip);
#endif
			free(readline);
			continue;
		}
		/* this-line-tracemode */
		echothisline = 0;
		tracethisline = 1;
	}

  /* The FOR hack
    If the line matches /^\s*for\s+\%[a-z]\s/, the FOR hack
    becomes active, because FOR requires the sequence "%<ch>"
    in its input.
    When the percent (%) expansion is made later on, any
    sequence "%<ch>" is retained.
  */
  cp = ip;
  if(matchtok(cp, "for") && *cp == '%' && isalpha(cp[1])
   && isspace(cp[2]))   /* activate FOR hack */
    forvar = toupper(cp[1]);
  else forvar = 0;

  cp = parsedline;
    while (*ip)
    {
      /* Assume that at least one character is added, place the
        test here to simplify the switch() statement */
      if(cp >= parsedMax(1)) {
        cp = NULL;    /* error condition */
        break;
      }
      if (*ip == '%')
      {
        switch (*++ip)
        {
          case '\0':    /* FOR hack forvar == 0 if no FOR is active */
            *cp++ = '%';
            break;

          case '%':
            *cp++ = *ip++;
            break;

          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            if (NULL != (tp = find_arg(*ip - '0')))
            {
              if(cp >= parsedMax(strlen(ip))) {
                cp = NULL;
                goto intBufOver;
              }
              cp = stpcpy(cp, tp);
              ip++;
            }
            else
              *cp++ = '%';

            break;

          case '?':
            /* overflow check: parsedline has that many character
              "on reserve" */
            cp += sprintf(cp, "%u", errorlevel);
            ip++;
            break;

          default:
            if(forvar == toupper(*ip)) {    /* FOR hack */
              *cp++ = '%';
        *cp++ = *ip++;
              break;
            }
            if ((tp = strchr(ip, '%')) != NULL)
            {
              *tp = '\0';

              if ((evar = getEnv(ip)) != NULL) {
                if(cp >= parsedMax(strlen(evar))) {
                  cp = NULL;
                  goto intBufOver;
                }
                cp = stpcpy(cp, evar);
               }

              ip = tp + 1;
            }
            break;
        }
        continue;
      }

      if (iscntrl(*ip))
        *ip = ' ';

      *cp++ = *ip++;
    }

intBufOver:
    free(readline);

  if(!cp) {     /* internal buffer overflow */
    error_line_too_long();
    continue;
  }

    *cp = '\0';   /* terminate copied string */

    if (echothisline)           /* Echo batch file line */
    {
      printprompt();
      puts(parsedline);
    }

    if (*parsedline)
    {
      if(tracethisline)
      	++tracemode;
      parsecommandline(parsedline);
      if(tracethisline)
      	--tracemode;
      if (echothisline || echo)
        putchar('\n');
    }
  }
  while (!canexit || !exitflag);

  return 0;
}

static void hangForever(void)
{
#ifdef FEATURE_AUTOREDIRECT_TO_CON
  int i;

  i = FEATURE_AUTOREDIRECT_TO_CON + 1;
#endif
    /* this might happen in certain "emergency"-level problems.
      The "cannot_exit" state must be honored, though.
      Also, it cannot be considered that the string area is
      available -- 1999/07/06 ska*/
    fcloseall();  /* prepare the external termination */
  for(;;) {
#ifdef FEATURE_AUTOREDIRECT_TO_CON
    if(--i == 0)
      cmd_ctty("CON");
#endif
    puts(   /* fcloseall() leaves the standard streams open */
     "\r\n\r\n"
     "The shell is about to be terminated, though, this is\r\n"
     "forbidden (usually by enabling the \"/P\" option).\r\n"
     "You must reboot the system or, if this shell runs in\r\n"
     "a multitasking environment, terminate this process/task manually.\r\n"
    );
    beep();
    delay(1000);  /* Keep the message on the screen for
              at least 1s, in case FreeCom has some problems
              with the keyboard */
  }
}


int main(void)
{
  /*
   * * main function
   */

  if(initialize() == E_None)
    process_input(0, NULL);

  if(!canexit)
    hangForever();

#ifdef FEATURE_KERNEL_SWAP_SHELL
	kswapDeRegister(kswapContext);
#endif

  return 0;
}
