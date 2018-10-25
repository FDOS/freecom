/* $Id$
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
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>

#include <suppl.h>
#include <dfn.h>
#include "../include/lfnfuncs.h"

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
#include "../include/cswap.h"

#include <environ.h>

#ifdef FEATURE_INSTALLABLE_COMMANDS
#include "../include/mux_ae.h"
#endif
#include "../include/crossjmp.h"

#ifdef I_AM_TOM
#ifndef DEBUG
void suppl_testMemChain() {}
#endif
#endif

  /* Shall the message block remain in memory when an external
    program is executed */
int persistentMSGs = 0;
int interactive_command = 0;	/* command directly entered by user */
int exitflag = 0;               /* indicates EXIT was typed */
#ifndef FEATURE_XMS_SWAP
	/* If XMS-Swap, this variable is located in resident portion */
int canexit = 0;                /* indicates if this shell is exitable
									enable within initialize() */
#endif
/*int ctrlBreak = 0;*/              /* Ctrl-Break or Ctrl-C hit */
int errorlevel = 0;             /* Errorlevel of last launched external prog */
int forceLow = 0;               /* load resident copy into low memory */
int oldinfd = -1;       /* original file descriptor #0 (stdin) */
int oldoutfd = -1;        /* original file descriptor #1 (stdout) */
int autofail = 0;				/* Autofail <-> /F on command line */
int inInit = 2;
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
int swapContext = TRUE;					/* may destroy external context */

#ifdef FEATURE_LONG_FILENAMES
unsigned char __supportlfns = 1;
#endif

/*
	stack checking:

	initialize memory at the bottom of stack to some
	known state (actually its own address)

	later verify that at least some of this memory is still
	left unchanged.
*/
static void * stack_bottom, * stack_unused;
void stack_check_init()
{
  char current_stack_location;
  void **barrier;

  /* place a barrier at the bottom of the stack
     code assumes 4 K right now */
  stack_bottom = &current_stack_location - 4*1024 + 50;
  stack_unused    = (char*)(&current_stack_location);

  for (barrier = stack_bottom; barrier < stack_unused ; barrier++)
    *barrier = barrier;
}
int stack_check(const char *commandline)
{
  void **barrier;

  for (barrier = stack_bottom; barrier < stack_unused; barrier++)
    if (*barrier != barrier)
	break;

  if (barrier < stack_unused) {
    unsigned stack_left = (char*)barrier - (char*)stack_bottom;

    if (stack_left < 0x300) {
      fprintf(stderr, "stack left %u after <%.60s>\n",stack_left,commandline);
      if (stack_left < 0x100) {
	fprintf(stderr, "hit any key to continue");
	cgetchar();
      }
    }

    stack_unused = barrier;
  }
  return 0;
}

#ifdef __GNUC__
int dup(int fd)
{
  int ret;
  asm volatile("int $0x21" : "=a"(ret): "Rah"((char)0x45), "b"(fd));
  return ret;
}

int dup2(int oldfd, int newfd)
{
  int scratch;
  asm volatile("int $0x21" : "=a" (scratch) :
			     "Rah"((char)0x46), "b"(oldfd), "c"(newfd));
  return 0;
}
#endif

#ifdef FEATURE_SWITCHAR
char switchar(void)
{
    USEREGS
    _AX = 0x3700;

    geninterrupt(0x21);

    return(_AL == 0x00 ? _DL : '/');
}
#endif

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
  char *extension;

  assert(first);
  assert(rest);

  /* check for a drive change */
  if ((strcmp(first + 1, ":") == 0) && isalpha(*first))
  {
  	changeDrive(*first);
    return;
  }

  if(strchr(first,'?') || strchr(first,'*')) {
    error_bad_command(first);
    return;
  }

  /* search through %PATH% for the binary */
  errno = 0;
  fullname = find_which(first);
  dprintf(("[find_which(%s) returned %s]\n", first, fullname));

  if(!fullname) {
    error_bad_command(first);
    return;
  }

  /* check if this is a .BAT file */
  extension = strrchr(dfnfilename(fullname), '.');
  assert(extension);

  if(stricmp(extension, ".bat") == 0) {
    dprintf(("[BATCH: %s %s]\n", fullname, rest));
    batch(fullname, first, rest);
  } else if(stricmp(extension, ".exe") == 0
   || stricmp(extension, ".com") == 0) {
    /* exec the program */
    int result;

    dprintf(("[EXEC: %s %s]\n", fullname, rest));

	if(strlen(rest) > MAX_EXTERNAL_COMMAND_SIZE) {
        char *fullcommandline = malloc( strlen( first ) + strlen( rest ) + 2 );
        error_line_too_long();
        if( fullcommandline == NULL ) return;
        sprintf( fullcommandline, "%s%s", first, rest );
        if( chgEnv( "CMDLINE", fullcommandline ) != 0 ) {
            free( fullcommandline );
            return;
        }
        free( fullcommandline );
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
#ifdef FEATURE_XMS_SWAP
    set_isrfct(0x23,
            MK_FP(FP_SEG(lowlevel_cbreak_handler)-0x10,
            FP_OFF(lowlevel_cbreak_handler)+0x100));
    /*
     * some tools expect this interrupt to  have the same segment as the
     * command.com PSP, but FreeCOM is an exe...
     */
#else
	set_isrfct(0x23, kswapContext->cbreak_hdlr);
#endif
#ifdef FEATURE_XMS_SWAP
    {
    isr v;
    get_isr(0x2e, v);
    if( *(unsigned char far *)v == 0xCF && !canexit) /* IRET? */
        set_isrfct( 0x2E,
                 MK_FP(FP_SEG(lowlevel_int_2e_handler)-0x10,
                 FP_OFF(lowlevel_int_2e_handler)+0x100));
    }
#endif
    result = exec(fullname, rest, 0);
	set_isrfct(0x23, cbreak_handler);		/* Install local CBreak handler */
	/* The external command might has killed the string area. */
	env_nullStrings(0);

    setErrorLevel(result);
  } else
    error_bad_command(first);
  chgEnv( "CMDLINE", NULL );
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
  char *cp;
  char *rest;            /* pointer to the rest of the command line */
  struct CMD *cmdptr = 0;

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
	char *buf = malloc(2+2*BUFFER_SIZE_MUX_AE+2+1);
#define args  (buf + 2)
#define ARGS_BUFFER_SIZE (2 + BUFFER_SIZE_MUX_AE + 3)
#define com (buf + ARGS_BUFFER_SIZE)
#define BUFFER_SIZE BUFFER_SIZE_MUX_AE
#else
	char *com = malloc(MAX_INTERNAL_COMMAND_SIZE);
#define args line
#define buf com
#define BUFFER_SIZE MAX_INTERNAL_COMMAND_SIZE
#endif

  assert(line);

	if(!buf) {
	  error_out_of_memory();
	  return;
	}

  /* delete leading spaces, but keep trailing whitespaces */
  line = ltrimcl(line);

#ifdef FEATURE_INSTALLABLE_COMMANDS
#if BUFFER_SIZE < MAX_INTERNAL_COMMAND_SIZE
	if(strlen(line) > BUFFER_SIZE) {
		error_line_too_long();
		goto errRet;
	}
#endif
	strcpy(args, line);
#endif

  if (*(rest = args))                    /* Anything to do ? */
  {
    cp = com;

  /* Copy over 1st word as upper case */
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
		int tryMUXAE;
		for(tryMUXAE = MUX_AE_MAX_REPEAT_CALL; tryMUXAE > 0; --tryMUXAE) {
			/* Check for installed COMMAND extension */
			switch(runExtension(com, args)) {
				case 1:		/* OK, done */
					goto errRet;
				case 0:		/* no extension */
					tryMUXAE = 0;
			}
			/* reset the argument pointer */
			rest = &args[(unsigned char)com[-1]];

			dprintf( ("[Command on return of Installable Commands check: >%s]\n", com) );
#ifndef NDEBUG
			dprintf( ("[Command line: >") );
			for(cp = args; cp < rest; ++cp)
				dprintf( ("%c", *cp) );
			dprintf( ("|%s]\n", rest) );
#endif	/* !defined(NDEBUG) */

#endif

		/* Scan internal command table */
		for (cmdptr = internalCommands
		 ; cmdptr->name && strcmp(com, cmdptr->name) != 0
		 ; cmdptr++);

    if(cmdptr && cmdptr->name) {    /* internal command found */

#ifdef FEATURE_INSTALLABLE_COMMANDS
	cp = malloc(ARGS_BUFFER_SIZE);
	strcpy(cp, rest);
	rest = cp;
	free(buf);
	buf = rest;
#else
	free(buf);  buf = 0;	/* no further useage of this buffer */
#endif
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
        goto errRet;
      }

        currCmdHelpScreen = cmdptr->help_id;
        /* JPP this will print help for any command */
        if(memcmp(ltrimcl(rest), "/?", 2) == 0)  {
          displayString(currCmdHelpScreen);
        } else {
          dprintf(("CMD '%s' : '%s'\n", cmdptr->name, rest));
          cmdptr->func(rest);
        }
        goto errRet;
	}
#ifdef FEATURE_INSTALLABLE_COMMANDS
	  }
#endif
      }

        free(buf); buf = 0;		/* no longer used */
        /* no internal command --> spawn an external one */
        cp = unquote(line, rest = skip_word(line));
        if(!cp) {
          error_out_of_memory();
          goto errRet;
        }
		execute(cp, rest);
		free(cp);
      }

#undef com
#undef args
#undef BUFFER_SIZE
#undef ARGS_BUFFER_SIZE

errRet:
	  free(buf);
}

/*
 * process the command line and execute the appropriate functions
 * full input/output redirection and piping are supported
 */
void parsecommandline(char *s, int redirect)
{
  char *in = 0;
  char *out = 0;
  char *fname0 = 0;
  char *fname1 = 0;
  char *nextcmd;

  int of_attrib = O_CREAT | O_TRUNC | O_WRONLY;
  int num;

  assert(s);

  dprintf(("[parsecommandline (%s)]\n", s));

#ifdef FEATURE_ALIASES
  aliasexpand(s, MAX_INTERNAL_COMMAND_SIZE);
  dprintf(("[alias expanded to (%s)]\n", s));
#endif

  if (tracemode)
  {                             /* Question after the variables expansion
                                   and make sure _all_ executed commands will
                                   honor the trace mode */
    /* 
     * Commands may be nested ("if errorlevel 1 echo done>test"). To
     * prevent redirecting FreeCOM prompts to user file, temporarily
     * revert redirection.
     */
    int redir_fdin = -1, redir_fdout = -1, answer;

    if (oldinfd != -1) {
        redir_fdin = dup (0);
        dup2 (oldinfd,  0);
    }
    if (oldoutfd != -1) {
        redir_fdout = dup (1);
        dup2 (oldoutfd, 1);
    }

    printprompt();
    outs(s);
    /* If the user hits ^Break, it has the same effect as
       usually: If he is in a batch file, he is asked if
       to abort all the batchfiles or just the current one */
    answer = userprompt(PROMPT_YES_NO);

    if (oldinfd  != -1) {
        dup2 (redir_fdin,  0);
        dos_close (redir_fdin);
    }
    if (oldoutfd != -1) {
        dup2 (redir_fdout, 1);
        dos_close (redir_fdout);
    }

    if (answer != 1) return;              /* "No" or ^Break   */
  }

  if(!redirect) {
  	docommand(s);
  	return;
  }

  assert(oldinfd == -1);  /* if fails something is wrong; should NEVER */
  assert(oldoutfd == -1); /* happen! -- 2000/01/13 ska*/

  num = get_redirection(s, &in, &out, &of_attrib);
  if (num < 0)                  /* error */
    goto abort;

  /* Set up the initial conditions ... */

  if (in || (num > 1))          /* Need to preserve stdin */
    oldinfd = dup(0);

  if (in)                       /* redirect input from this file name */
  {
    dos_close(0);
    if (0 != devopen(in, O_RDONLY))
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
    dos_close(1);               /* Close current output file */
    if ((fname0 = tmpfn()) == 0)
      goto abort;
    dos_creat(fname0, 0);

    nextcmd = s + strlen(s) + 1;
    docommand(s);

    dos_close(1);
    dup2(oldoutfd, 1);

    dos_close(0);
    killtmpfn(fname1);          /* fname1 can by NULL */
    fname1 = fname0;
    fname0 = 0;
    dos_open(fname1, O_RDONLY);

    s = nextcmd;
  }

  /* Now set up the end conditions... */

  if (out)                      /* Final output to here */
  {
    dos_close(1);
    if (1 != devopen(out, of_attrib))
    {
		error_redirect_to_file(out);
      goto abort;
    }
  }
  else if (oldoutfd != -1)      /* Restore original stdout */
  {
    dos_close(1);
    dup2(oldoutfd, 1);
    dos_close(oldoutfd);
    oldoutfd = -1;
  }

  docommand(s);                 /* process final command */

abort:
  if (oldinfd != -1)            /* Restore original STDIN */
  {
    dos_close(0);
    dup2(oldinfd, 0);
    dos_close(oldinfd);
    oldinfd = -1;
  }

  if (oldoutfd != -1)           /* Restore original STDOUT */
  {
    dos_close(1);
    dup2(oldoutfd, 1);
    dos_close(oldoutfd);
    oldoutfd = -1;
  }

  killtmpfn(fname1);
  killtmpfn(fname0);

  if (out)
    free(out);

  if (in)
    free(in);
}

/* line -- min size: MAX_INTERNAL_COMMAND_SIZE + sizeof(errorlevel) * 8
 */
int expandEnvVars(char *ip, char * const line)
{	char *cp, *tp;

	assert(ip);
	assert(line);

/* Return the maximum pointer into parsedline to add 'numbytes' bytes */
#define parsedMax(numbytes)   \
  (line + MAX_INTERNAL_COMMAND_SIZE - 1 - (numbytes))

	cp = line;

	while(*ip) {
	  /* Assume that at least one character is added, place the
		test here to simplify the switch() statement */
	  if(cp >= parsedMax(1))
		return 0;

	  if(*ip == '%') {
		switch(*++ip) {
		  case '\0':
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
			if(0 != (tp = find_arg(*ip - '0'))) {
			  if(cp >= parsedMax(strlen(tp)))
				return 0;
			  cp = stpcpy(cp, tp);
			  ip++;
			}
			else
			  *cp++ = '%';
			  /* Let the digit be copied in the cycle */

			break;

#if 0
		  	/* Caused conflicts with some batches,
		  		see %ERRORLEVEL% */
		  case '?':
			/* overflow check: parsedline has that many character
			  "on reserve" */
			cp += sprintf(cp, "%u", errorlevel);
			ip++;
			break;
#endif

		  default:
#if 0
			if(forvar == toupper(*ip)) {    /* FOR hack */
			  *cp++ = '%';			/* let the var be copied in next cycle */
			  break;
			}
#endif
			if((tp = strchr(ip, '%')) != 0) {
				char *evar;
			  *tp = '\0';

			  if((evar = getEnv(ip)) != 0) {
				if(cp >= parsedMax(strlen(evar)))
				  return 0;
				cp = stpcpy(cp, evar);
				free(evar);
			  } else if(matchtok(ip, "ERRORLEVEL")) {
				/* overflow check: parsedline has that many character
				  "on reserve" */
				cp += sprintf(cp, "%u", errorlevel);
			  } else if(matchtok(ip, "_CWD") || matchtok(ip, "CD")) {
			  	if(0 == (evar = cwd(0))) {
				    return 0;
			  	} else {
					if(cp >= parsedMax(strlen(evar)))
					  return 0;
					cp = stpcpy(cp, evar);
					free(evar);
			  	}
			  }

			  ip = tp + 1;
			} else
			  *cp++ = '%';
			break;
		}
		continue;
	  }

#if 0
	  if(iscntrl(*ip)) {
		*cp++ = ' ';
		++ip;
	  } else 
#endif
		*cp++ = *ip++;
	}

	assert(cp);
	assert(cp < line + MAX_INTERNAL_COMMAND_SIZE);

	*cp = 0;
	return 1;
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
#if 0
/* Return the maximum pointer into parsedline to add 'numbytes' bytes */
#define parsedMax(numbytes)   \
  (parsedline + MAX_INTERNAL_COMMAND_SIZE - 1 - (numbytes))
  char *evar;
  char *tp;
  char *cp;
#endif
  char *ip;
#if 0
  char forvar;
#endif
  int echothisline;
  int tracethisline;

  do
  {
#ifdef FEATURE_LONG_FILENAMES
    char *lfn = getEnv("LFN");
    if( lfn && toupper( *lfn ) == 'N' )
         __supportlfns = 0;
    else __supportlfns = 1;
    free(lfn);
#endif
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
      int attr;
      if (xflag   /* must not go interactive */
       || ((attr = fdattr(0)) & 0x84) == 0x84  /* input is NUL device */
       || (attr & 0xc0) == 0x80) /* no further input */
      {
        free(readline);
        break;
      }

      /* Go Interactive */
		interactive_command = 1;		/* directly entered by user */
		/* Ensure the prompt starts at column #0 */
		if(echo && (mywherex()>1))
			outc('\n');
      readcommand(ip = readline, MAX_INTERNAL_COMMAND_SIZE);
      tracemode = 0;          /* reset trace mode */
      }
    }

    	/* Make sure there is no left-over from last run */
    currCmdHelpScreen = 0;

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

#if 0
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

#else
	if(cmd_for_hackery(ip)) {
		free(readline);
		continue;
	}
#endif

	{	int rc = expandEnvVars(ip, parsedline);
		free(readline);
		if(!rc) {
			error_line_too_long();
			continue;
		}
	}
  
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
      parsecommandline(parsedline, TRUE);
      if(tracethisline)
      	--tracemode;
    }

    stack_check(parsedline);
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
  for(;;) {
#ifdef FEATURE_AUTOREDIRECT_TO_CON
    if(--i == 0)
      cmd_ctty("CON");
#endif
#if 0
    puts(
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
    delay(9000);  /* Keep the message on the screen for
              at least 1s, in case FreeCom has some problems
              with the keyboard */
  }
}

int cdecl my2e_parsecommandline( char *s )
{
    s[ (unsigned char)s[ 0 ] ] = '\0';
/*    printf("_my2e_parsecommandline( %s )\n", s );*/
    parsecommandline( &s[ 1 ], 1 );
    return( errorlevel );
}

int main(void)
{
  /*
   * * main function
   */

  stack_check_init();

  if(setjmp(jmp_beginning) == 0 && initialize() == E_None)
    process_input(0, 0);

  if(!canexit)
    hangForever();

#ifdef FEATURE_KERNEL_SWAP_SHELL
	kswapDeRegister(kswapContext);
#endif

  return 0;
}
