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
#include <environ.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/cmdline.h"
#include "../include/misc.h"
#include "../include/nls.h"
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



int ctrlBreak = 0;              /* Ctrl-Break or Ctrl-C hit */
FLAG forceLow = 0;               /* load resident copy into low memory */
FLAG autofail = 0;				/* Autofail <-> /F on command line */
int inInit = 1;
FLAG isSwapFile = 0;
jmp_buf jmp_beginning;
	/* if != 0, pointer to static context
		NOT allowed to alter if swapOnExec == ERROR !!
	*/
kswap_p kswapContext = 0;

FLAG lflag_rewindBatchFile = FALSE;
FLAG lflag_doExit = FALSE, lflag_doCancel = FALSE, lflag_doQuit = FALSE;
char *lflag_gotoLabel = 0;

void perform_exec_result(int result)
{
	dprintf(("result of (do_)exec(): %d\n", result));
	if (result == -1)
		perror("executing spawnl function");
	else
		gflag_errorlevel = result;
}


/*
 * Execute an external command or X:.
 *
 * first - first word on command line
 * rest  - rest of command line
 */
static void execute(char *first, char *rest)
{
	char *name;

	assert(first);
	assert(rest);

	/* check for a drive change */
	if((strcmp(first + 1, ":") == 0) && isalpha(*first)) {
		changeDrive(*first);
		return;
	}

	if(strchr(first,'?') || strchr(first,'*')) {
		error_bad_command();
		return;
	}

	chkHeap
	/* search through %PATH% for the binary */
	errno = 0;
	name = find_which(first);
	chkHeap
	dprintf(("[find_which(%s) returned %s]\n", first, name));

	if(!name) {
		error_bad_command();
		return;
	}

	/* check if this is a .BAT file */
	assert(strrchr(name, '.'));

	if(stricmp(strrchr(name, '.'), ".bat") == 0) {
		dprintf(("[BATCH: %s %s]\n", name, rest));
		chkHeap
		batch(name, first, rest);
		chkHeap
	} else {
		/* exec the program */
		int result;

		dprintf(("[EXEC: %s %s]\n", name, rest));

		if(strlen(rest) > MAX_EXTERNAL_COMMAND_SIZE) {
			error_long_external_line();
			return;
		}

		/* Prepare to call an external program */

		purgeMemory();

		chkHeap
		/* Execute the external program */
#ifdef FEATURE_KERNEL_SWAP_SHELL
		if(lflag_swap == TRUE && gflag_swap != ERROR
		 && kswapMkStruc(name, rest)) {
			/* The Criter and ^Break handlers has been installed within
				the PSP in kswapRegister() --> nothing to do here */
			dprintf(("[EXEC: exiting to kernel swap support]\n"));
			exit(123);		/* Let the kernel swap support do the rest */
		}
#ifdef DEBUG
		if(lflag_swap == TRUE && gflag_swap != ERROR)
			dprintf(("KSWAP: failed to save context, proceed without swapping\n"));
#endif
#endif
			/* Install the dummy (always abort) ^Break handler */
		setvect(0x23, (void interrupt(*)()) kswapContext->cbreak_hdlr);
		result = exec(name, rest, 0);
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
	if(memcmp(line, FORCE_INTERNAL_COMMAND_STRING
	 , (sizeof(FORCE_INTERNAL_COMMAND_STRING)-1)) == 0) {
		forceInternalCommand = 1;
		line = ltrimcl(line + (sizeof(FORCE_INTERNAL_COMMAND_STRING)-1));
		dprintf(("[Force execution of internal command]\n"));
	}

	chkHeap
	if(*(rest = line) == 0)
		return;		/* nothing to do */

#ifdef DEBUG
	if(dbg_trace) {
		char *p;

		p = StrConcat(3, dbg_trace, " prexec: ", rest);
		if(p) {
			cmd_dispStatus(p);
			myfree(p);
		} else	dprintf(("[DBG: Out of memory running preexec status]\n"));
	}
#endif

	if((name = getCmdName(&(const char*)rest)) != 0) {
		if(*rest && strchr(QUOTE_STR, *rest)) {
			/* If the first word is quoted, it is no internal command */
			chkPtr(name);
			StrFree(name);
			rest = line;
		}
	}

	chkHeap
	if(!name && !strchr(QUOTE_STR, *rest)) {
		/* maybe it's a special internal command */
		if((name = StrChar(*rest)) != 0)
			++rest;
	}
	StrFUpr(name);		/* Pass a mangled name to MUX-AE */

	chkHeap
#ifdef FEATURE_INSTALLABLE_COMMANDS
	newargs = 0;
	if(!forceInternalCommand && name) {
		/* Check for installed COMMAND extension */
		if(runExtension(&name, rest, &newargs))
			goto okRet;		/* OK, executed! */
		dprintf( ("[Command on return of Installable Commands check: >%s<]\n"
		 , name) );
		if(newargs) {
			rest = newargs;
			dprintf(("[New arguments: %s]\n", rest));
		}
	}
#endif

	chkHeap
	/* Scan internal command table */
	if((cmdptr = is_icmd(name)) != 0
	 && (forceInternalCommand || (cmdptr->flags & CMD_HIDDEN) == 0)) {
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

		if(memcmp(ltrimcl(rest), "/?", 2) == 0)
			displayString(cmdptr->help_id);
		else {
			dprintf(("[ICMD %s: %s]\n", name, rest));
			cmdptr->func(rest);
		}
	} else		/* external command */
		if(forceInternalCommand) {
			error_no_such_forced_internal_command(name);
	} else {
		chkHeap
#ifdef FEATURE_INSTALLABLE_COMMANDS
		if(name) {	/* MUX-AE had been invoked */
			/* Installable Commands are allowed to change both:
				"com" and "args". Therefore, we may need to
				reconstruct the external command line */
			if((line = StrCat(name, rest)) == 0) {
				error_out_of_memory();
				goto errRet;
			}
			chkPtr(newargs);
			StrFree(newargs);		/* conserve memory */
//			name = 0;
		}
		chkHeap
#endif
        /* no internal command --> spawn an external one */
        if((p = unquote(line, rest = skip_word(line))) == 0) {
			error_out_of_memory();
			goto errRet;
        }
		chkHeap
		execute(p, rest);
		chkHeap
		myfree(p);
		chkHeap
	}

okRet:
#ifdef DEBUG
	if(dbg_trace) {
		char *p;

		p = StrConcat(3, dbg_trace, " postexec: ", line);
		if(p) {
			cmd_dispStatus(p);
			myfree(p);
		} else	dprintf(("[DBG: Out of memory running preexec status]\n"));
	}
#endif

errRet:
	chkHeap
	myfree(name);
	chkHeap
#ifdef FEATURE_INSTALLABLE_COMMANDS
	myfree(newargs);
	chkHeap
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
#ifdef DEBUG
	unsigned old_fd;
#endif

	flushall();			/* messing around with file descriptors */

	jft = getJFTp();

	if(ecMkFD(i, jft[i]) != E_None)
		return 0;

#ifdef DEBUG
	old_fd = jft[i];
#endif
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
	dprintf(("[Redirected fd#%u (SFT %u --> %u) to: %s]\n"
	 , i, old_fd, jft[i], fnam));

	/* Apply the other entries of this redirection */
	for(j = i; j--;) if(jftUsed[i] == jftUsed[j]) {
#ifdef DEBUG
		old_fd = jft[j];
#endif
		if(ecMkFD(j, jft[j]) != E_None)
			return 0;
		/* is the same --> duplicate the fd here, too */
		if(dup2(i, j) != 0) {
			err_fct(fnam);
			return 0;
		}
		dprintf(("[Redirected fd#%u (SFT %u --> %u) to duplicated fd#%u]\n"
		 , j, old_fd, jft[j], i));
	}
	return 1;
}

static char *makePipeDelTemp(void)
{	char *ivar;

	if((ivar = ecMkIVar()) == 0)
		return 0;

	if(ecMkc("IVAR ", ivar, (char*)0) != E_None		/* remove the IVar */
	 		/* remove tempfile */
	 || ecMkc("DEL %@IVAR(", ivar, ")", (char*)0) != E_None) {
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
	assert(pipe);
	assert(num >= 2);

	out = *Xout;
	rv = 0;				/* error */

/* The commands must be pushed from the last to the top one */
	p = ivarOut = 0;
	if((ivarIn = makePipeDelTemp()) == 0)
		goto errRet;
	assert(pipe[num - 1]);
	assert(pipe[0]);
	if(!*ltrimcl(pipe[num - 1])) {
		error_empty_command();
		goto errRet;
	}
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
		if(ecMkV1C(pipe[num - 1], p, "<%@IVAR(", ivarIn, ")", (char*)0)
		 != E_None)
			goto errRet;
		chkPtr(p);
		StrFree(p);
		freep(out);
		*Xout = 0;
	} else
		if(ecMkV1C(pipe[num - 1], "<%@IVAR(", ivarIn, ")", (char*)0) != E_None)
			goto errRet;

	/* Process the middle commands */
	for(i = num - 1; --i; ) {
		assert(pipe[i]);
		if(!*ltrimcl(pipe[i])) {
			error_empty_command();
			goto errRet;
		}
		if((q = strchr(pipe[i], 0))[-1] == '|') {
			q[-1] = 0;
			q = 0;			/* q == 0 <-> redirect stderr, too */
		}
		myfree(ivarOut);
		ivarOut = ivarIn;
		if((ivarIn = makePipeDelTemp()) == 0
		 || ecMkV1C(pipe[i], ">", q? "": "&", "%@IVAR(", ivarOut, ")"
		     , "<%@IVAR(", ivarIn, ")", (char*)0) != E_None
		 || ecMkHC("IVAR ", ivarOut, "=%@TEMPFILE", (char*)0) != E_None)
			goto errRet;
	}

/* The first command of the pipe may be executed now */
	/* perform the ::=IVAR <ivarIn>=%@TEMPFILE  command */
	assert(!p);
	assert(pipe[0]);
	if(!*ltrimcl(pipe[0])) {
		error_empty_command();
		goto errRet;
	}
	if((p = tmpfn()) == 0			/* make the tempfile */
	 || (q = erealloc(p, strlen(p) + 3)) == 0	/* needed below */
	 || ivarSet(ivarIn, p = q) != E_None)		/* in 'p', if Set() fails */
		goto errRet;

	/* Setup the *Xout array to contain the previous ivarIn */
	chkPtr(ivarIn);
	StrFree(ivarIn);		/* the name is no longer needed */
	if((*Xout = out = ecalloc(2, sizeof(char*))) == 0)
		goto errRet;
	out[0] = p;
	/* out[1] = 0;		done by calloc() */
	memmove(&p[2], p, strlen(p) + 1);
	p[0] = '>';		/* output redirection ID */
	p[1] = 2;			/* overwrite, stdout */
	if((q = strchr(pipe[0], 0))[-1] == '|') {
		q[-1] = 0;
		p[1] = 6;			/* overwrite, stdout & stderr */
	}
	p = 0;

	rv = 1;					/* OK */

errRet:
	myfree(ivarIn);
	myfree(ivarOut);
	myfree(p);

	return rv;
}

void parseExpandedCommand(char *line)
{
	char ** in, ** out, ** pipe;	/* list of all input, output redirections
  										in sequence of appearence and the
  										pipe components.
  									*/
	int num;
	char **jftUsed;		/* which entries are redir'ed to which file */
	int i;

	num = get_redirection(line, &in, &out, &pipe);
	chkHeap

	if(num) {
		if(num > 1) {			/* pipe */
			/* Break up the pipe into individual commands as explained
				in DOCS\ICMDS.TXT */
			if(!makePipeContext(&out, pipe, num))
				goto errRet1;
		}
		chkHeap

	/* Execute a single command with possible redirections */
		/* Setup the redirection contexts */
		if((jftUsed = ecalloc(getJFTlen(), sizeof(char*))) == 0)
			goto errRet;

		chkHeap
		if(in) {
			for(i = 0; in[i]; ++i) {
				/* There is currently only one kind of input redirection:
					the one redirecting stdin */
				assert(in[i][0] == '<');
				jftUsed[0] = in[i];
			}
		}
		chkHeap
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
				assert(out[i][1] >= 2 && out[i][1] <= 7);
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

		chkHeap
		/* apply the redirections */
		for(i = getJFTlen(); i--;) if(jftUsed[i]) {
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
				  ? O_TEXT | O_WRONLY | O_CREAT | O_APPEND	/* append */
				  : O_TEXT | O_WRONLY | O_CREAT | O_TRUNC	/* overwrite */
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

		chkHeap
		docommand(pipe[0]);		/* execute the command */
errRet:
		chkHeap
		myfree(jftUsed);
	}
	/* else an empty command line is happily ignored */

errRet1:
	chkHeap
	freep(in);
	chkHeap
	freep(out);
	chkHeap
	myfree(pipe);		/* pointers into line[] */
	chkHeap
}

/*
 * process the command line and execute the appropriate functions
 * full input/output redirection and piping are supported
 */
void parsecommandline(char *Xline)
{	char *line;

	line = Xline;
	assert(line);

	dprintf(("[parsecommandline (%s)]\n", line));

#ifdef FEATURE_ALIASES
	line = aliasexpand(line);
	chkHeap
	dprintf(("[alias expanded to (%s)]\n", line));
#endif

	if(lflag_trace) {		/* Question after the variables expansion
                                   and make sure _all_ executed commands will
                                   honor the mode */
		fputs(line, stdout);
		/* If the user hits ^Break, it has the same effect as
			usually: If he is in a batch file, he is asked if
			to abort all the batchfiles or just the current one */
		if(userprompt(PROMPT_YES_NO) != 1)
			/* Pressed either "No" or ^Break */
			goto errRet;
	}

	parseExpandedCommand(line);

errRet:
	if(line != Xline)
		myfree(line);
	chkHeap
}

/*
 *	Fetches the next context; executes it to retreive the line
 *	and parses and executes the line.
 *	It returns if the very last context finishes
 */
void run_exec_context(void)
{	char *ctxt;
	unsigned id;
	ecTag_t tag;
	char *line;

	while(0 != (id = CTXT_INFO(CTXT_TAG_EXEC, nummax))
	 && id >= CTXT_INFO(CTXT_TAG_EXEC, nummin)) {
		assert(ctxtSegm);
		if((ctxt = ctxtAddress(CTXT_TAG_EXEC, id)) == 0) {
			/* hole / topmost entry lost */
			--CTXT_INFO(CTXT_TAG_EXEC, nummax);
			continue;
		}
		chkHeap
		tag = *ctxt;
		 	/* or an end context reached */
		if(tag == EC_TAG_TERMINATE)
		 	return;

		if(tag >= EC_TAG_TERMINATE || !ecFctRead[tag]) {
#ifdef DEBUG
			if(ecFctRead[tag])
				dprintf(("[EXEC: Skipping unknown exec context: %u]\n", tag));
#endif
			ecPop();
			continue;
		}

		/* as we are about to aquire a new command line, some
			options are resetted to their defaults */
		_fmemcpy(&ctxtSharedFlags, ctxtFlagsP, sizeof(ctxtSharedFlags));

		chkHeap
		if((line = (ecFctRead[tag])(ctxt)) == 0)
			ecPop();
		else if(line != cmdlineIgnore) {
			/* process this command line */
			char *p, *q;

			chkHeap
			dprintf(("[CTXT: aquired line: %s]\n", line));
			/* 
			 * The question mark '?' has a double meaning:
			 *	C:\> ?
			 *		==> Display short help
			 *
			 *	C:\> ? command arguments
			 *		==> enable tracemode for just this line
			 */
			if(*(p = ltrimcl(line)) == '?' && *(q = ltrimcl(p + 1))) {
				/* something follows --> tracemode */
				lflag_trace = 1;
				p = q;			/* skip the prefix */
				dprintf(("[CTXT: Enable local TRACE status]\n"));
			}

			chkHeap
			/* Placed here, FOR even preceeds any checks for 
				redirections, pipes etc. */
			if(cmd_for_hackery(p)) {
				myfree(line);
				continue;
			}

			chkHeap
			p = expEnvVars(p);
			chkHeap
			myfree(line);
			chkHeap
			if(p != 0) {
				parsecommandline(p);
				chkHeap
				myfree(p);
				chkHeap
			}
		}
		/* else ignore this call */
	}
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
#undef 	TEXT_MSG_REBOOT_NOW
	puts(TEXT_MSG_REBOOT_NOW);
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

	if(!gflag_canexit)
		hangForever();

#ifdef FEATURE_KERNEL_SWAP_SHELL
	kswapDeRegister(kswapContext);
#endif
	ctxtDestroy();

	return 0;
}
