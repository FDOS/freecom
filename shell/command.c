/*
 * COMMAND.C - command-line interface.
 *
 */

#include "../config.h"

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

#include "../include/command.h"
#include "../include/batch.h"
#include "../include/cmdline.h"
#include "../err_fcts.h"
#include "../strings.h"
#ifdef FEATURE_NLS
#include "../include/nls.h"
#endif
#include "../include/openf.h"
#include "../include/kswap.h"

#ifdef FEATURE_INSTALLABLE_COMMANDS
#include "../include/mux_ae.h"
#endif
#include "../include/crossjmp.h"


  /* Shall the message block remain in memory when an external
    program is executed */
int persistentMSGs = 0;
int interactive_command = 0;	/* command directly entered by user */
int exitflag = 0;               /* indicates EXIT was typed */
int canexit = 0;                /* indicates if this shell is exitable
									enable within initialize() */
int ctrlBreak = 0;              /* Ctrl-Break or Ctrl-C hit */
int errorlevel = 0;             /* Errorlevel of last launched external prog */
int forceLow = 0;               /* load resident copy into low memory */
int oldinfd = -1;       /* original file descriptor #0 (stdin) */
int oldoutfd = -1;        /* original file descriptor #1 (stdout) */
int autofail = 0;				/* Autofail <-> /F on command line */
int inInit = 1;
int isSwapFile = 0;
jmp_buf jmp_beginning;

	/* FALSE: no swap this time
		TRUE: swap this time
		ERROR: no swap avilable at all
	*/
int swapOnExec = FALSE;
int defaultToSwap = FALSE;
	/* if != 0, pointer to static context
		NOT allowed to alter if swapOnExec == ERROR !!
	*/
kswap_p kswapContext = 0;


void perform_exec_result(int result)
{
	dprintf(("result of (do_)exec(): %d\n", result));
	if (result == -1)
		perror("executing spawnl function");
	else
		errorlevel = result;

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
  errno = 0;
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

	if (strlen(rest) > MAX_EXTERNAL_COMMAND_SIZE)
	{
		error_line_too_long();
		return;
	}

/* Prepare to call an external program */

	/* Unload the message block if not loaded persistently */
	if(!persistentMSGs)
		unloadMsgs();

/* Execute the external program */
#ifdef FEATURE_KERNEL_SWAP_SHELL
    if(swapOnExec == TRUE
	 && kswapMkStruc(fullname, rest)) {
	 	/* The Criter and ^Break handlers has been installed within
	 		the PSP in kswapRegister() --> nothing to do here */
	 	dprintf(("[EXEC: exiting to kernel swap support]\n"));
	 	exit(123);		/* Let the kernel swap support do the rest */
	}
#ifdef DEBUG
	if(swapOnExec == TRUE)
		dprintf(("KSWAP: failed to save context, proceed without swapping\n"));
#endif
#endif
		/* Install the dummy (always abort) handler */
	setvect(0x23, (void interrupt(*)()) kswapContext->cbreak_hdlr);
    result = exec(fullname, rest, 0);
	setvect(0x23, cbreak_handler);		/* Install local CBreak handler */

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

#ifdef FEATURE_INSTALLABLE_COMMANDS
	/* Duplicate the command line into such buffer in order to
		allow Installable Commands to alter the command line.
		*line cannot be modified as pipes would be destroyed. */
	/* Place both buffers immediately following each other in
		order to make sure the contents of args can be appended
		to com without any buffer overflow checks.
		*2 -> one buffer for com and one for args
		+2 -> max length byte of com + cur length of com
		+3 -> max length byte of args + cur length of args + additional '\0'
	*/
	char buf[2+2*BUFFER_SIZE_MUX_AE+2+1];
#define com  (buf + 2)
#define args (buf + 2 + BUFFER_SIZE_MUX_AE + 2)
#define BUFFER_SIZE BUFFER_SIZE_MUX_AE
#else
	char com[MAX_INTERNAL_COMMAND_SIZE];
#define BUFFER_SIZE MAX_INTERNAL_COMMAND_SIZE
#endif
  char *cp;
  char *rest;            /* pointer to the rest of the command line */

  struct CMD *cmdptr;

  assert(line);

  /* delete leading spaces, but keep trailing whitespaces */
  line = ltrimcl(line);

#ifdef FEATURE_INSTALLABLE_COMMANDS
#if BUFFER_SIZE < MAX_INTERNAL_COMMAND_SIZE
	if(strlen(line) > BUFFER_SIZE) {
		error_line_too_long();
		return;
	}
#endif
	line = strcpy(args, line);
#endif

  if (*(rest = line))                    /* Anything to do ? */
  {
    cp = com;

  /* Copy over 1st word as lower case */
  /* Internal commands are constructed out of non-delimiter
  	characters; ? had been parsed already */
    while(*rest && is_fnchar(*rest) && !strchr(QUOTE_STR, *rest))
      *cp++ = toupper(*rest++);

    if(*rest && strchr(QUOTE_STR, *rest))
      /* If the first word is quoted, it is no internal command */
      cp = com;   /* invalidate it */
    *cp = '\0';                 /* Terminate first word */

	if(*com) {
#ifdef FEATURE_INSTALLABLE_COMMANDS
		/* Check for installed COMMAND extension */
		if(runExtension(com, args))
			return;		/* OK, executed! */

		dprintf( ("[Command on return of Installable Commands check: >%s<]\n", com) );
#endif

		/* Scan internal command table */
		for (cmdptr = internalCommands
		 ; cmdptr->name && strcmp(com, cmdptr->name) != 0
		 ; cmdptr++);

	}

    if(*com && cmdptr->name) {    /* internal command found */
      switch(cmdptr->flags & (CMD_SPECIAL_ALL | CMD_SPECIAL_DIR)) {
      case CMD_SPECIAL_ALL: /* pass everything into command */
        break;
      case CMD_SPECIAL_DIR: /* pass '\\' & '.' too */
        if(*rest == '\\' || *rest == '.' || *rest == ':') break;
      default:        /* pass '/', ignore ',', ';' & '=' */
        if(!*rest || *rest == '/') break;
        if(isargdelim(*rest)) {
			rest = ltrimcl(rest);
			break;
		}

        /* else syntax error */
        error_syntax(0);
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
#ifdef FEATURE_INSTALLABLE_COMMANDS
		if(*com) {		/* external command */
			/* Installable Commands are allowed to change both:
				"com" and "args". Therefore, we may need to
				reconstruct the external command line */
			/* Because com and *rest are located within the very same
				buffer and rest is definitely terminated with '\0',
				the followinf memmove() operation is fully robust
				against buffer overflows */
			memmove(com + strlen(com), rest, strlen(rest) + 1);
			/* Unsave, but probably more efficient operation:
				strcat(com, rest);
					-- 2000/12/10 ska*/
			line = com;
		}
#endif
        /* no internal command --> spawn an external one */
        cp = unquote(line, rest = skip_word(line));
        if(!cp) {
          error_out_of_memory();
          return;
        }
		execute(cp, ltrimsp(rest));
		free(cp);
      }
  }
#undef line
#undef com
#undef args
#undef BUFFER_SIZE
}

/*
 * process the command line and execute the appropriate functions
 * full input/output redirection and piping are supported
 */
void parsecommandline(char *s)
{
  char *in = 0;
  char *out = 0;
  char *fname0 = 0;
  char *fname1 = 0;
  char *nextcmd;

  int of_attrib = O_CREAT | O_TRUNC | O_TEXT | O_WRONLY;
  int num;

  assert(s);
  assert(oldinfd == -1);  /* if fails something is wrong; should NEVER */
  assert(oldoutfd == -1); /* happen! -- 2000/01/13 ska*/

  dprintf(("[parsecommandline (%s)]\n", s));

#ifdef FEATURE_ALIASES
  aliasexpand(s, MAX_INTERNAL_COMMAND_SIZE);
  dprintf(("[alias expanded to (%s)]\n", s));
#endif

  if (tracemode)
  {                             /* Question after the variables expansion
                                   and make sure _all_ executed commands will
                                   honor the trace mode */
    fputs(s, stdout);
    /* If the user hits ^Break, it has the same effect as
       usually: If he is in a batch file, he is asked if
       to abort all the batchfiles or just the current one */
	if(userprompt(PROMPT_YES_NO) != 1)
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
		error_redirect_from_file(in);
      goto abort;
    }
  }

  if (out || (num > 1))         /* Need to preserve stdout */
    oldoutfd = dup(1);

  /* Now do all but the last pipe command */
  while (num-- > 1)
  {
    close(1);                   /* Close current output file */
    if ((fname0 = tmpfn()) == 0)
      goto abort;
    open(fname0, O_CREAT | O_TRUNC | O_TEXT | O_WRONLY, S_IREAD | S_IWRITE);

    nextcmd = s + strlen(s) + 1;
    docommand(s);

    close(1);
    dup2(oldoutfd, 1);

    close(0);
    killtmpfn(fname1);          /* fname1 can by NULL */
    fname1 = fname0;
    fname0 = 0;
    open(fname1, O_TEXT | O_RDONLY, S_IREAD);

    s = nextcmd;
  }

  /* Now set up the end conditions... */

  if (out)                      /* Final output to here */
  {
    close(1);
    if (1 != devopen(out, of_attrib, S_IREAD | S_IWRITE))
    {
		error_redirect_to_file(out);
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

void readcommand(char * const str, int maxlen)
{
#ifdef FEATURE_ENHANCED_INPUT
	/* If redirected from file or so, should use normal one */
	readcommandEnhanced(str, maxlen);
#else
	readcommandDOS(str, maxlen);
#endif
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
    /* Dimensionate parsedline that no sprintf() can overflow the buffer
     */
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
  	interactive_command = 0;		/* not directly entered by user */
  	echothisline = tracethisline = 0;
    if(commandline) {
      ip = commandline;
      readline = commandline = 0;
    } else {
    if ((readline = malloc(MAX_INTERNAL_COMMAND_SIZE + 1)) == 0)
    {
      error_out_of_memory();
      return 1;
    }

      if (0 == (ip = readbatchline(&echothisline, readline,
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
		interactive_command = 1;		/* directly entered by user */
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
    if(*(ip = ltrimcl(ip)) == '?') {
    	 ip = ltrimcl(ip + 1);
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
   && isargdelim(cp[2]))   /* activate FOR hack */
    forvar = toupper(cp[1]);
  else forvar = 0;

  cp = parsedline;
    while (*ip)
    {
      /* Assume that at least one character is added, place the
        test here to simplify the switch() statement */
      if(cp >= parsedMax(1)) {
        cp = 0;    /* error condition */
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
            if (0 != (tp = find_arg(*ip - '0')))
            {
              if(cp >= parsedMax(strlen(tp))) {
                cp = 0;
                goto intBufOver;
              }
              cp = stpcpy(cp, tp);
              ip++;
            }
            else
              *cp++ = '%';
              /* Let the digit be copied in the cycle */

            break;

          case '?':
            /* overflow check: parsedline has that many character
              "on reserve" */
            cp += sprintf(cp, "%u", errorlevel);
            ip++;
            break;

          default:
            if(forvar == toupper(*ip)) {    /* FOR hack */
              *cp++ = '%';			/* let the var be copied in next cycle */
              break;
            }
            if ((tp = strchr(ip, '%')) != 0)
            {
              *tp = '\0';

              if ((evar = getEnv(ip)) != 0) {
                if(cp >= parsedMax(strlen(evar))) {
                  cp = 0;
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

      if (iscntrl(*ip)) {
        *cp++ = ' ';
        ++ip;
      } else 
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
      if(swapOnExec != ERROR)
      	swapOnExec = defaultToSwap;
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
#if 0
    puts(   /* fcloseall() leaves the standard streams open */
     "\r\n\r\n"
     "The shell is about to be terminated, though, this is\r\n"
     "forbidden (usually by enabling the \"/P\" option).\r\n"
     "You must reboot the system or, if this shell runs in\r\n"
     "a multitasking environment, terminate this process/task manually.\r\n"
    );
#else
#undef 	TEXT_MSG_REBOOT_NOW
	puts(TEXT_MSG_REBOOT_NOW);
#endif
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

  if(setjmp(jmp_beginning) == 0 && initialize() == E_None)
    process_input(0, 0);

  if(!canexit)
    hangForever();

#ifdef FEATURE_KERNEL_SWAP_SHELL
	kswapDeRegister(kswapContext);
#endif

  return 0;
}
