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

#include <dynstr.h>
#include <mcb.h>
#include <nls_f.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/cmdline.h"
#include "../include/misc.h"
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
int interactive = 0;	/* command directly entered by user */
int exitflag = 0;               /* indicates EXIT was typed */
int canexit = 0;                /* indicates if this shell is exitable
									enable within initialize() */
int ctrlBreak = 0;              /* Ctrl-Break or Ctrl-C hit */
int errorlevel = 0;             /* Errorlevel of last launched external prog */
int forceLow = 0;               /* load resident copy into low memory */
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
   * Execute an external command or X:.
   *
   * first - first word on command line
   * rest  - rest of command line
   */

  char *fullname;

  assert(first);
  assert(rest);

  /* check for a drive change */
  if((strcmp(first + 1, ":") == 0) && isalpha(*first)) {
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
		error_long_external_line();
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
		/* Install the dummy (always abort) ^Break handler */
	setvect(0x23, (void interrupt(*)()) kswapContext->cbreak_hdlr);
    result = exec(fullname, rest, 0);
	setvect(0x23, cbreak_handler);		/* Install local CBreak handler */

    perform_exec_result(result);
  }
}

static void docommand(char *line)
{
	/*
	 * check for forced internal commands, execute the MUX-AE chain, if not;
	 * look through the internal commands and determine whether or not this
	 * command is one of them. If it is, call the command. If not, call
	 * execute() to run it as an external program.
	 *
	 * line - the command line of the program to run
	 */

	FLAG forceInternalCommand;
	char *name, *rest, *p;
	struct CMD *cmdptr;

#ifdef FEATURE_INSTALLABLE_COMMANDS
	char *newargs;		/* runExtension returns a new command line
							here, if a MUX-AE client had changed it */
#endif

  assert(line);

	/* delete leading spaces, but keep trailing whitespaces */
	line = ltrimcl(line);
  	forceInternalCommand = 0;
	if(memcmp(line, "::=", 3) == 0) {
		forceInternalCommand = 1;
		line = ltrimcl(line + 3);
		dprintf(("[Force execution of internal command]\n"));
	}

	if(*(rest = line) == 0)
		return;		/* nothing to do */

	if((name = getCmdName(&(const char*)rest)) != 0) {
		if(*rest && strchr(QUOTE_STR, *rest)) {
			/* If the first word is quoted, it is no internal command */
			StrFree(name);
			rest = line;
		}
	}

	if(!name && !strchr(QUOTE_STR, *rest)) {
		/* maybe it's a special internal command */
		if((name = StrChar(*rest)) != 0)
			++rest;
	}
	StrFUpr(name);		/* Pass a mangled name to MUX-AE */

#ifdef FEATURE_INSTALLABLE_COMMANDS
	newargs = 0;
	if(!forceInternalCommand && name) {
		/* Check for installed COMMAND extension */
		if(runExtension(&name, rest, &newargs))
			return;		/* OK, executed! */
		dprintf( ("[Command on return of Installable Commands check: >%s<]\n"
		 , name) );
		if(newargs) {
			rest = newargs;
			dprintf(("[New arguments: %s]\n", rest));
		}
	}
#endif

	/* Scan internal command table */
	if((cmdptr = is_icmd(name)) != 0) {
		switch(cmdptr->flags & (CMD_SPECIAL_ALL | CMD_SPECIAL_DIR)) {
		case CMD_SPECIAL_ALL: /* pass everything into command */
			break;
		case CMD_SPECIAL_DIR: /* pass '\\' & '.' too */
			if(*rest == '\\' || *rest == '.' || *rest == ':')
				break;
			/**FALL THORUGH**/
		default:        /* pass '/', ignore ',', ';' & '=' */
			if(!*rest || *rest == '/')
				break;
			if(isargdelim(*rest)) {
				rest = ltrimcl(rest);
				break;
			}

			/* else syntax error */
			error_syntax(0);
			goto errRet;
		}

		if(memcmp(ltrimcl(rest), "/?", 2))
			displayString(cmdptr->help_id);
		else {
			dprintf(("[ICMD %s: %s]\n", name, rest));
			cmdptr->func(rest);
		}
	} else		/* external command */
		if(forceInternalCommand) {
			error_no_such_forced_internal_command(name);
	} else {
#ifdef FEATURE_INSTALLABLE_COMMANDS
		if(name) {	/* MUX-AE had been invoked */
			/* Installable Commands are allowed to change both:
				"com" and "args". Therefore, we may need to
				reconstruct the external command line */
			if((p = erealloc(name, strlen(name) + strlen(rest) + 1)) == 0)
				goto errRet;
			line = strcat(name, rest);
			StrFree(newargs);		/* conserve memory */
		}
#endif
        /* no internal command --> spawn an external one */
        if((p = unquote(line, rest = skip_word(line))) == 0) {
			error_out_of_memory();
			goto errRet;
        }
		execute(p, rest);
		free(p);
	}

errRet:
	free(name);
#ifdef FEATURE_INSTALLABLE_COMMANDS
	free(newargs);
#endif
}

static int redirectJFTentry(int i
	, char ** const jftUsed
	, char * const fnam
	, int openmode
	, int createmode
	, void (*err_fct)(const char * const))
{	int j;
	byte far *jft;		/* pointer to FreeCOM's JFT */

	jft = getJFTp();

	if(ecMkFD(i, jft[i]))
		return 0;

	jft[i] = 255;		/* Mark the file descriptor as unused */
	if((j = devopen(fnam, openmode, createmode)) < 0) {
		err_fct(fnam);
		return 0;
	}
	if(j != i) {
		if(dup2(j, i) != 0) {
			err_fct(fnam);
			return 0;
		}
		close(j);
	}

	/* Apply the other entries of this redirection */
	for(j = i; j--;) if(jftUsed[i] == jftUsed[j]) {
		if(ecMkFD(j, jft[j]))
			return 0;
		/* is the same --> duplicate the fd here, too */
		if(dup2(i, j) != 0) {
			err_fct(fnam);
			return 0;
		}
	}
	return 1;
}

static char *makePipeDelTemp(void)
{	char *ivar;

	if((ivar = ecMkIVar()) == 0) {	
		error_out_of_memory();
		return 0;
	}
	if(!ecMkHC("IVAR ", ivar)					/* remove the IVar */
	 || !ecMkHC("DEL %@IVAR(", ivar, ")")) {	/* remove tempfile */
	 	ecFreeIVar(ivar);
	 	return 0;
	}

	return ivar;
}

static int makePipeContext(char *** const Xout
	, char * const * const pipe
	, const int num)
{	int i, rv;
	char *ivarIn, *ivarOut;
	char *p, **out, *q;

	assert(Xout);
	assert(*Xout);
	assert(pipe);
	assert(num >= 2);

	out = *Xout;
	rv = 0;				/* error */

/* The commands must be pushed from the last to the top one */
	p = ivarOut = 0;
	if((ivarIn = ecMkIVar()) == 0)
		goto errRet;
	assert(pipe[num - 1]);
	if(out) {
		int len;

		/* The last command gets the optional output redirection */
		len = 0;
		/* Add all the output redirections */
		for(i = 0; out[i]; ++i) {
			assert(out[i][0] == '>');
			assert(out[i][1] != 0);

			if((q = erealloc(p, len + strlen(&out[i][2]) + 10)) == 0)
				goto errRet;
			p = q;
			q += len;
			*q++ = '>';
			if(out[i][1] & 1)			/* append */
				*q++ = '>';
			if(out[i][1] & 4)			/* redirect stderr */
				*q++ = '&';
			if((out[i][1] & 2) == 0)	/* redirect stderr only */
				*q++ = '>';
			*q++ = '"';					/* have the filename quoted */
			q = stpcpy(q, &out[i][2]);
			*q++ = '"';
			len = q - p;
		}
		p[len] = 0;
		if(!ecMkV1C(pipe[num - 1], p, "<%@IVAR(", ivarIn, ")"))
			goto errRet;
		StrFree(p);
		freep(out);
		*Xout = 0;
	} else
		if(!ecMkV1C(pipe[num - 1], "<%@IVAR(", ivarIn, ")"))
			goto errRet;

	/* Process the middle commands */
	for(i = num - 1; --i; ) {
		assert(pipe[i]);
		assert(*pipe[i]);
		if((q = strchr(pipe[i], 0))[-1] == '|') {
			*q = 0;
			q = 0;			/* q == 0 <-> redirect stderr, too */
		}
		free(ivarOut);
		ivarOut = ivarIn;
		if((ivarIn = ecMkIVar()) == 0
		 || !ecMkV1C(pipe[i], ">", q? "": "&", "%@IVAR(", ivarOut, ")"
		     , "<%@IVAR(", ivarIn, ")")
		 || !ecMkHC("IVAR ", ivarOut, "=%@TEMPFILE"))
			goto errRet;
	}

/* The first command of the pipe may be executed now */
	/* perform the ::=IVAR <ivarIn>=%@TEMPFILE  command */
	assert(!p);
	if((p = fct_tempfile("")) == 0		/* make the tempfile */
	 || (q = erealloc(p, strlen(p) + 3)) == 0	/* needed below */
	 || !ivarSet(ivarIn, p = q))		/* in 'p', if Set() fails */
		goto errRet;

	/* Setup the *Xout array to contain the previous ivarIn */
	StrFree(ivarIn);		/* the name is no longer needed */
	if((out = ecalloc(2, sizeof(char*))) == 0)
		goto errRet;
	out[0] = p;
	/* out[1] = 0;		done by calloc() */
	memmove(&p[2], p, strlen(p));
	p[0] = '>';		/* output redirection ID */
	p[1] = 2;			/* overwrite, stdout */
	assert(pipe[0]);
	assert(*pipe[0]);
	if((q = strchr(pipe[0], 0))[-1] == '|') {
		*q = 0;
		p[1] = 6;			/* overwrite, stdout & stderr */
	}
	*Xout = out;
	p = 0;

	rv = 0;						/* OK */

errRet:
	free(ivarIn);
	free(ivarOut);
	free(p);

	return rv;
}

void parseExpandedCommand(char *s)
{
	char ** in, ** out, ** pipe;	/* list of all input, output redirections
  										in sequence of appearence and the
  										pipe components.
  									*/
	int num;
	char **jftUsed;		/* which entries are redir'ed to which file */
	int i;

	num = get_redirection(s, &in, &out, &pipe);

	if(num) {
		if(num > 1) {			/* pipe */
			/* Break up the pipe into individual commands as explained
				in DOCS\ICMDS.TXT */
			if(!makePipeContext(&out, pipe, num))
				goto errRet1;
		}

	/* Execute a single command with possible redirections */
		/* Setup the redirection contexts */
		if((jftUsed = ecalloc(getJFTlen(), sizeof(char*))) == 0)
			goto errRet;

		if(in) {
			for(i = 0; in[i]; ++i) {
				/* There is currently only one kind of input redirection:
					the one redirecting stdin */
				assert(in[i][0] == '<');
				jftUsed[0] = in[i];
			}
		}
		if(out) {
			int error = 0;
			for(i = 0; out[i]; ++i) {
				/* Supported output redirections:
					cmdline	out[]	meaning
					>		>\2	overwrite, stdout
					>>		>\3	append, stdout
					>&		>\6	overwrite, stdout & stderr -- 4dos compatibly
					>>&		>\7	append, stdout & stderr -- 4dos compatibly
					>&>		>\4	overwrite, stderr -- 4dos compatibly
					>>&>	>\5	append, stderr -- 4dos compatibly
				*/
				assert(out[i][0] == '>');
				assert(out[i][1] >= 2 && out[i][1] <= 5);
				if(out[i][1] & 2) {	/* redirect fd#1 == stdout */
					if(jftUsed[1] && jftUsed[1][0] != '>') {
						error_intermixed_redirection(1);
						error = 1;
					}
					jftUsed[1] = out[i];
				}
				if(out[i][1] & 4) {	/* redirect fd#2 == stderr */
					if(jftUsed[2] && jftUsed[2][0] != '>') {
						error_intermixed_redirection(2);
						error = 1;
					}
					jftUsed[2] = out[i];
				}
			}
			if(error)
				goto errRet;
		}

		/* apply the redirections */
		for(i = getJFTlen(); --i;) if(jftUsed[i]) {
			switch(jftUsed[i][0]) {
			case '<':
				jftUsed[i][0] = 'I';	/* prevent the same action
											with secondary redirs */
				if(!redirectJFTentry(i, jftUsed, &jftUsed[i][1]
				 , O_TEXT | O_RDONLY, S_IREAD | S_IWRITE
				 , error_redirect_from_file))
				 	goto errRet;
				break;

			case '>':		/* output redirection */
				jftUsed[i][0] = 'O';	/* prevent the same action
											with secondary redirs */
				if(!redirectJFTentry(i, jftUsed, &jftUsed[i][2]
				 , (jftUsed[i][1] & 1)
				  ? O_TEXT | O_WRONLY | O_CREAT | O_TRUNC	/* overwrite */
				  : O_TEXT | O_WRONLY | O_CREAT | O_APPEND	/* append */
				 , S_IREAD | S_IWRITE
				 , error_redirect_to_file))
				 	goto errRet;
				break;

#ifdef DEBUG
			case 'I':
			case 'O':	/* already processed entries */
				break;
			default:
				dprintf(("Unkown redirection code: %u]\n", jftUsed[i][0]));
#endif
			}
		}

		docommand(pipe[0]);		/* execute the command */
errRet:
		free(jftUsed);
	}
	/* else an empty command line is happily ignored */

errRet1:
	freep(in);
	freep(out);
	free(pipe);		/* pointers into s[] */
}

/*
 * process the command line and execute the appropriate functions
 * full input/output redirection and piping are supported
 */
void parsecommandline(char *Xs)
{	char *s;

	s = Xs;
	assert(s);

	dprintf(("[parsecommandline (%s)]\n", s));

#ifdef FEATURE_ALIASES
	s = aliasexpand(s);
	dprintf(("[alias expanded to (%s)]\n", s));
#endif

	if(trace) {		/* Question after the variables expansion
                                   and make sure _all_ executed commands will
                                   honor the mode */
		fputs(s, stdout);
		/* If the user hits ^Break, it has the same effect as
			usually: If he is in a batch file, he is asked if
			to abort all the batchfiles or just the current one */
		if(userprompt(PROMPT_YES_NO) != 1)
			/* Pressed either "No" or ^Break */
			goto errRet;
	}

	parseExpandedCommand(s);

errRet:
	if(s != Xs)
		free(s);
}

/*
 *	Fetches the next context; executes it to retreive the line
 *	and parses and executes the line.
 *	It returns if the very last context finishes
 */
void run_exec_context(void)
{	ctxtEC_t far *ec;

	assert(ctxtMain);
	ec = ctxtExecContext;
	assert(ec);

	while(ec->ctxt_type != EC_TAG_TERMINATE) {
		if(FP_SEG(ec) + (FP_OFF(ec) + ec->ctxt_length + sizeof(*ec) - 1) / 16
		  >= nxtMCB(FP_SEG(ctxtMCB))) {
		  	error_context_corrupted();
		  	return;
		}

		if(ec->ctxt_type >= EC_TAG_TERMINATE) {
			dprintf(("[EXEC: Skipping unknown exec context: %u]\n"
			 , ec->ctxt_type));
			ecPop();			/* remove this context */
		} else {
			char *cmdline;

			assert(ecFunction[ec->ctxt_type]);

			/* as we are about to aquire a new command line, some
				options are resetted to their defaults */
			echo = ctxtFlags.f_echo;
			trace = ctxtFlags.f_trace;
			swap = ctxtFlags.f_swap;
			called = ctxtFlags.f_call;
			interactive = 0;

			if((cmdline = (ecFunction[ec->ctxt_type])(ec)) != 0) {
				/* process this command line */
				char *p, *q;

				/* 
				 * The question mark '?' has a double meaning:
				 *	C:\> ?
				 *		==> Display short help
				 *
				 *	C:\> ? command arguments
				 *		==> enable tracemode for just this line
				 */
				if(*(p = ltrimcl(cmdline)) == '?' && *(q = ltrimcl(p + 1))) {
					/* something follows --> tracemode */
					trace = 1;
					p = q;			/* skip the prefix */
				}

#ifdef CMD_INCLUDE_FOR
				/* Placed here FOR even preceeds any checks for 
					redirections, pipes etc. */
				if(cmd_for_hackery(p)) {
					free(cmdline);
					continue;
				}
#endif
				p = expEnvVars(p);
				free(cmdline);
				if(p != 0) {
					parsecommandline(p);
					free(p);
				}
			}
			/* else ignore this call */
		}
	}
}

#if 0
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
    if((readline = emalloc(MAX_INTERNAL_COMMAND_SIZE + 1)) == 0)
      return 1;

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

      if (iscntrl(*ip))
        *cp = ' ';
      else 
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
#endif

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
  if(setjmp(jmp_beginning) == 0 && initialize() == E_None)
    run_exec_context();

  if(!canexit)
    hangForever();

#ifdef FEATURE_KERNEL_SWAP_SHELL
	kswapDeRegister(kswapContext);
#endif

  return 0;
}
