/*
 *  INIT.C - initialization code
 *
 *      This is more or less a complete rewrite based on the
 *      initialize() code of v0.74c, but also some changes of 0.75b.
 *
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <process.h>
#include <stdlib.h>
#include <string.h>

#include <mcb.h>
#include <environ.h>
#include <dfn.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"
#include "../include/misc.h"
#include "../include/module.h"
#include "../include/keys.h"
#include "../strings.h"
#include "../include/kswap.h"
#include "../include/cswap.h"

#define pspTermAddr *(void far* far*)MK_FP(_psp, 0xa)

        /* Check for an argument; ch may be evaluated multiple times */
#define isargsign(ch)           \
        ((ch) == ':' || (ch) == '=')

#ifdef FEATURE_CALL_LOGGING
#ifndef INCLUDE_CMD_FDDEBUG
static char logFilename[] = LOG_FILE;
#endif
#endif

#ifdef FEATURE_XMS_SWAP
#define oldPSP	origPPID
#else
static unsigned oldPSP;
#endif
char *ComPath;                   /* absolute filename of COMMAND shell */

#ifdef DEBUG
extern unsigned _heaplen;
#endif

#ifndef FDDEBUG_INIT_VALUE
#define FDDEBUG_INIT_VALUE 0
#endif
int fddebug = FDDEBUG_INIT_VALUE;    /* debug flag */

/* Without resetting the owner PSP, the program is not removed
   from memory */
void exitfct(void)
{
	unloadMsgs();        /* free the message strings segment */
	OwnerPSP = oldPSP;
#ifdef FEATURE_XMS_SWAP
	pspTermAddr = termAddr;
	XMSexit();
#endif
}


int showhelp = 0, internalBufLen = 0, inputBufLen = 0;
int spawnAndExit = E_None;
int newEnvSize = 0;          /* Min environment table size */
char *user_autoexec = 0;
int skipAUTOEXEC = 0;

#pragma argsused
optScanFct(opt_init)
{ int ec = E_None;

  switch(ch) {
  case '?': showhelp = 1; return E_None;
  case '!': return optScanBool(fddebug);
  case 'Y': return optScanBool(tracemode);
  case 'F': return optScanBool(autofail);
  case 'D': return optScanBool(skipAUTOEXEC);
  case 'P':
    if(optHasArg())     /* change autoexec.bat */
      ec = optScanString(user_autoexec);
    canexit = 0;
    return ec;
  case 'E': return optScanInteger(newEnvSize);
  case 'L': return optScanInteger(internalBufLen);
  case 'U': return optScanInteger(inputBufLen);
  case 'C': /* spawn command, then exit */
    spawnAndExit = E_Exit;
  case 'K':   /* spawn command */
    return E_NoOption;    /* don't tread as option */
  case 0:   /* longname option */
    switch(toupper(*optstr)) {
    case 'C': /* spawn command, then exit */
      spawnAndExit = E_Exit;
    case 'K':   /* spawn command */
      return E_NoOption;    /* don't tread as option */
    case 'L':
      if(optLong("LOW"))
        return optScanBool(forceLow);
      break;
    case 'M':
      if(optLong("MSG"))
        return optScanBool(persistentMSGs);
      break;
    case 'S':
    	if(optLong("SWAP"))
    		return optScanBool(defaultToSwap);
      break;
    }
    break;
  }
  optErr();
  return E_Useage;
}


/*
 * set up global initializations and process parameters
 *
 *      This function will:
 *      1) Set up the host environment (Create a new environment segment if
 *              necessary, alter the parent process ID, catch ^Break etc.)
 *      2) Create the COMSPEC variable
 *      3) Parse the command line parameters passed to COMMAND.COM
 *      4) Perform all command line actions (spawn "/c" commands, alter
 *              size of environment etc.)
 *      5) If shell is interactive, invoke a ver() command.
 *
 * If a serious problem occurs, e.g. FreeCom has received an invalid
 * command line from the system (e.g. 128 bytes), FreeCom assumes that the
 * user wants an interactive shell. So the command line is scanned until
 * the problem occurs, but the rest is ignored.
 */

int initialize(void)
{
  //int rc;
  int comPath;                /* path to COMMAND.COM (for COMSPEC/reload) */
  char *newTTY;                 /* what to change TTY to */
  int showinfo;                 /* show initial info only if no command line options */

  int ec;           /* error code */
  unsigned offs;        /* offset into environment segment */

  int cmdlen;         /* length of command line */
  char *cmdline;        /* command line duplicated into heap */
  char *p, *h, *q;
#ifdef FEATURE_CALL_LOGGING
#ifndef INCLUDE_CMD_FDDEBUG
  FILE *f;
#endif
#endif
#ifdef DEBUG
	int orig_env;
#endif

/* Set up the host environment of COMMAND.COM */

	/* Give us shell privileges */
#ifdef FEATURE_XMS_SWAP
	myPID = _psp;
	residentCS = _CS;
	termAddr = pspTermAddr;
	pspTermAddr = terminateFreeCOMHook;
#endif
	oldPSP = OwnerPSP;
	atexit(exitfct);
	OwnerPSP = _psp;

	/* Install the dummy handlers for Criter and ^Break */
/*	initCBreak(); */
	setvect(0x23, cbreak_handler);
#ifdef FEATURE_XMS_SWAP
	/* There is no special handler for FreeCOM currently
		--> activate the real one */
	setvect(0x24, lowlevel_err_handler);
#else
	setvect(0x24, dummy_criter_handler);
#endif

  /* DOS shells patch the PPID to the own PID, how stupid this is, however,
    because then DOS won't terminate them, e.g. when a Critical Error
    occurs that is not detected by COMMAND.COM */

	dbg_printmem();
#ifdef DEBUG
	{ void* p;
		if((p = malloc(5*1024)) == 0)
			dprintf(("[MEM: Out of memory allocating test block during INIT]"));
		else free(p);
	}
#endif

#ifdef FEATURE_KERNEL_SWAP_SHELL
	if(kswapInit()) {		/* re-invoked */
		if(kswapLoadStruc()) {
			/* OK, on success we need not really keep the shell trick
				(pretend we are our own parent), which might cause
				problems with beta-software-bugs ;-)
				In fact, KSSF will catch up our crashes and re-invoke
				FreeCOM, probably with the loss of any internal
				settings. */
			  OwnerPSP = oldPSP;
			return E_None;
		}
	}
#endif

  /* Some elder DOSs may not pass an initializied environment segment */
  if(env_glbSeg && !isMCB(SEG2MCB(env_glbSeg))) {
    env_setGlbSeg(0);       /* Disable the environment */
    dprintf(("[ENV: Disabled invalid environment]"));
  }

/* Now parse the command line parameters passed to COMMAND.COM */
  /* Preparations */
  newTTY = 0;
  comPath = tracemode = 0;
  showinfo = 1;

  /* Because FreeCom should be executed in a DOS3+ compatible
    environment most of the time, it is assumed that its path
    can be determined from the environment.
    This has the advantage that the string area is accessable
    very early in the run.
    The name of the current file is string #0. */
  if((offs = env_string(0, 0)) != 0)    /* OK, environment filled */
    grabComFilename(0, (char far *)MK_FP(env_glbSeg, offs));
  /* After that argv[0] is no longer used and maybe zapped.
  	This also will help, as most programs altering the environment
  	segment externally don't expect a string area. */
  env_nullStrings(0);

  /* Aquire the command line, there are three possible sources:
    1) DOS command line @PSP:0x80 as pascal string,
    2) extended DOS command line environment variable CMDLINE,
      if peekb(PSP, 0x80) == 127,&
    3) MKS command line @ENV:2, if peekb(ENV, 0) == '~'
    	&& peekb(ENV, 1) == '='

    Currently implemented is version #1 only
  */
  cmdlen = peekb(_psp, 0x80);
  if(cmdlen < 0 || cmdlen > 126) {
    error_corrupt_command_line();
    cmdlen = 0;
  }
    /* duplicate the command line into the local address space */
  if((cmdline = malloc(cmdlen + 1)) == 0) {
    error_out_of_memory();  /* Cannot recover from this problem */
    return E_NoMem;
  }
  _fmemcpy((char far*)cmdline, MK_FP(_psp, 0x81), cmdlen);
  cmdline[cmdlen] = '\0';
#ifdef FEATURE_CALL_LOGGING
#ifndef INCLUDE_CMD_FDDEBUG
  if((f = fopen(logFilename, "at")) == 0) {
    fprintf(stderr, "Cannot open logfile: \"%s\"\n", logFilename);
  } else {

  putc('"', f);
  if(ComPath)   /* path to command.com already known */
    fputs(ComPath, f);
  putc('"', f);
  putc(':', f);

  fputs(cmdline, f);
  putc('\n', f);
  fclose(f);
  }
#else
	cmd_fddebug(logFilename);

	dbg_outc('"');
	dbg_outs(ComPath);
	dbg_outc('"');
	dbg_outc(':');
	dbg_outsn(cmdline);
#endif
#endif

  canexit = 1;
  p = cmdline;    /* start of the command line */
  do {
  ec = leadOptions(&p, opt_init, 0);
  if(ec == E_NoOption) {    /* /C or /K */
    assert(p && *p);
    if(!isoption(p)) {
      error_quoted_c_k();
      p = 0;
      break;
    }
    assert(p[1] && strchr("kKcC", p[1]));
    p += 2;   /* p := start of command line to execute */
    break;
  } else if(ec != E_None) {
        showhelp = 1;
    p = 0;
    break;
  }

  assert(p && !isoption(p) && !isspace(*p));
  if(!*p) {
    p = 0;
    break;      /* end of line reached */
  }
  q = unquote(p, h = skip_word(p));
  p = h;      /* Skip this word */
  if(!q) {
    error_out_of_memory();
    p = 0;
    break;
  }
  if(!comPath) {      /* 1st argument */
    grabComFilename(1, (char far*)q);
    comPath = 1;
    free(q);
  } else if(!newTTY) {  /* 2nd argument */
#ifdef INCLUDE_CMD_CTTY
    newTTY = q;
#else
      error_ctty_excluded();
    free(q);
#endif
      } else {
        error_too_many_parameters(q);
        showhelp = 1;
        free(q);
        break;
      }
   } while(1);

   /*
    * Now:
    * + autoexec: AUTOEXEC.BAT file to be executed if /P switch
    *   is enabled; if NULL, use default
    * + comPath: user-defined PATH to COMMAND.COM; if NULL, use
    *   the one from the environment
    * + newTTY: the name of the device to be CTTY'ed; if NULL,
    *   no change
    * + p: pointer to the command to be executed:
    *   *p == 'c' or 'C' --> spawn command, then terminate shell
    *   *p == 'k' or 'K' --> spawn command, then go interactive
    *   &p[1] --> command line, unless the first character is an
    *   argument character
    */

/* Now process the options */

#ifdef INCLUDE_CMD_CTTY
  if (newTTY) {      /* change TTY as early as possible so the caller gets
                          the messages into the correct channel */
    cmd_ctty(newTTY);
    free(newTTY);
  }
#endif

  if(!ComPath) {
    /* FreeCom is unable to find itself --> print error message */
    /* Emergency error */
#undef TEXT_MSG_FREECOM_NOT_FOUND
	puts(TEXT_MSG_FREECOM_NOT_FOUND);
    return E_Useage;
  }

  /* First of all, set up the environment */
    /* If a new valid size is specified, use that */
#ifdef DEBUG
	orig_env = env_glbSeg;
#endif
  env_resizeCtrl |= ENV_USEUMB | ENV_ALLOWMOVE | ENV_LASTFIT;
  if(newEnvSize > 16 && newEnvSize < 32767)
    env_setsize(0, newEnvSize);
#ifdef ENVIRONMENT_KEEP_FREE 
#if ENVIRONMENT_KEEP_FREE > 0
  else if(env_freeCount(env_glbSeg) < ENVIRONMENT_KEEP_FREE) {
	dprintf(("[ENV: auto-resize environment because too small: %d]\n"
		, env_freeCount(env_glbSeg)) );
	env_replace(0		/* Modify the default segment */
	 , ENV_DELETE | ENV_COPY | ENV_CREATE | ENV_FREECOUNT
	 , ENVIRONMENT_KEEP_FREE);
	}
#else
#error ENVIRONMENT_KEEP_FREE is non-positive
#endif
#endif

  /* Otherwise the path is placed into the environment */
    /* Set the COMSPEC variable. */
  if(chgEnv("COMSPEC", ComPath)) {		/* keep it silent */
    /* Failed to add this variable, the most likely problem should be that
      the environment is too small --> it is increased and the
      operation is redone */
    env_resize(0, strlen(ComPath) + 10);
    if(chgEnv("COMSPEC", ComPath))
    	chgEnv("COMSPEC",  NULL);	/* Cannot set -> zap an old one */
  }
  	inInit = 0;
#ifdef DEBUG
	if(orig_env != env_glbSeg) {
		dprintf(("[ENV: Environment changed: @%04x -> @%04x; free %u]\n"
		 , orig_env, env_glbSeg, env_freeCount(env_glbSeg)));
	}
#endif

#ifndef FEATURE_XMS_SWAP
	/* Install INT 24 Critical error handler */
	/* Needs the ComPath variable, eventually */
	if(!kswapContext) {
		/* Load the module/context into memory */
		if((kswapContext = modContext()) == 0) {
			error_loading_context();
			return E_NoMem;
		}
#ifdef FEATURE_KERNEL_SWAP_SHELL
		if(swapOnExec != ERROR)
			kswapRegister(kswapContext);
#endif
	}
#endif

	ctxtCreate();

#ifdef FEATURE_XMS_SWAP
	/* Now everything is setup --> initialize the XMS stuff */
	XMSinit();
	/* Initialize the EXEC Block structure used by XMS Swap Exec
		interface */
	/* envSeg = 0;		default & always updated by caller */
	dosParamDosExec.cmdtail = dosCMDTAIL;
	dosParamDosExec.FCB1 = dosFCB1;
	dosParamDosExec.FCB2 = dosFCB2;
	/* overlPtr1;		not used by this exec */
	/* overlPtr2;		not used by this exec */
#else
	/* re-use the already loaded Module */
	setvect(0x24, (void interrupt(*)())
	 MK_FP(FP_SEG(kswapContext->cbreak_hdlr), kswapContext->ofs_criter));
#endif

  if(internalBufLen)
    error_l_notimplemented();
  if(inputBufLen)
    error_u_notimplemented();

  if(tracemode)
    showinfo = 0;

  if (showhelp)
    displayString(TEXT_CMDHELP_COMMAND);

  if ((showhelp || exitflag) && canexit)
    return E_None;

  /* Now the /P option can be processed */
	if(!canexit) {
		char *autoexec;

		spawnAndExit = E_None;
		autoexec = user_autoexec? user_autoexec: AUTO_EXEC;

		showinfo = 0;
		short_version();

		if(skipAUTOEXEC) {		/* /D option */
			showinfo = 0;
			displayString(TEXT_MSG_INIT_BYPASSING_AUTOEXEC, autoexec);
		} else {
			if(exist(autoexec)) {
#ifdef FEATURE_BOOT_KEYS
				struct REGPACK r;
				int key;

				r.r_ax = 0x3000;	/* Get DOS version & OEM ID */
				intr(0x21, &r);
				if(!tracemode	/* /Y --> F8 on CONFIG.SYS */
				 || ((r.r_bx & 0xff00) == 0xfd00	/* FreeDOS >= build 2025 */
				      && !(r.r_cx > 0x101 || (r.r_bx & 0xff) > 24))) {
					displayString(TEXT_MSG_INIT_BYPASS_AUTOEXEC, autoexec);
					key = cgetchar_timed(3);
					putchar('\n');
				} else key = 0;

				if(key == KEY_F8)
					tracemode = 1;

				if(key == KEY_F5)
					displayString(TEXT_MSG_INIT_BYPASSING_AUTOEXEC, autoexec);
				else
#endif
					process_input(1, autoexec);
			} else {
				if(user_autoexec)
					error_sfile_not_found(user_autoexec);
#ifdef INCLUDE_CMD_DATE
					cmd_date(0);
#endif
#ifdef INCLUDE_CMD_TIME
					cmd_time(0);
#endif
			}
		}

		free(user_autoexec);
	} else {
		assert(user_autoexec == 0);
	}

  /* Now the /C or /K option can be processed */
  if (p)
  {
    process_input(1, p);
    return spawnAndExit;
  }

  /* Don't place something here that must be executed after a /K or /C */

  if (showinfo)
  {
    short_version();
#ifndef DEBUG
    putchar('\n');
    showcmds(0);
    putchar('\n');
#endif
  }

  return E_None;
}
