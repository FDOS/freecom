/*
 *  INIT.C - initialization code
 *
 *      This is more or less a complete rewrite based on the
 *      initialize() code of v0.74c, but also some changes of 0.75b.
 *
 *  Comments:
 *      1998/07/19 ska  removed from COMMAND.C and put into this file
 *
 * 09-Sep-1998 ska
 * - fix/chg: ^Break handler now handles external programs properly
 *
 * 01-Dec-1998 jpp
 * - finished batch file trace mode.  Fixed bug with /C command line switch
 *   running batch files
 * - Added about 2 second delay where user can press F8 to run autoexec.bat
 *   in tracemode, or press F5 to bypass autoexec.bat completely.
 *
 * 1998/12/04 ska
 *  bugfix: "/C" collection of already broken-up arguments
 *
 * 1998/12/05 ska
 * - add: command line parsing as mention in the strings\strings.txt help page
 * - add: issue the help screen on "/?" [with displayString()]
 * - add: set "exitflag" if help screen was issued or an invalid option
 *  was found. If exit is OK, COMMAND will terminate then
 * - add: load messages into conventional far memory
 *
 * 1999/01/08 ska
 * - bugfix: on suggestion of Charles Dye (raster@highfiber.com) "/ccommand"
 *      "/c" must work without a space between [MS COMMAND compatibly]
 *      also mimics: "/c/? dir" --> bad command or filename
 * - add: DR DOS compatibly: "/c:" and "/c="
 *      even supports wierd looking: "/c:dir/?"
 * - add: DR DOS compatibly: "/p:AUTOEXEC_replacement"
 * - sub: calling _fullpath() for ComDir; at this point the path must be
 *      fully-qualtifed by convention of the environ
 *
 * 1/Feb/1999 Rob Linwood
 * - removed the "static" modifier from "showcmds()" so it could be used
 *   in the "?" command
 *
 * 11-Feb-1999 (John P Price <linux-guru@gcfl.net>)
 * - using keypressed() that I wrote instead of Turbo C's kbhit() function
 *   because for some reason kbhit() hangs under the FreeDOS kernel.
 * - prints seconds as it waits for a keypress from user (during initial
 *   boot)
 * - Fixed path to autoexec.bat.
 *
 * 24-Mar-1999 (John P Price <linux-guru@gcfl.net>)
 * - changed the way we run autoexec.bat or a batch file on the command line.
 *
 * 1999/04/23 ska
 * chg: ComDir: replaced by ComPath, comFile() & comPathFile()
 * fix: combination of "/p" and "/c"|"/k" did not work
 *
 * 1999/07/06 ska
 * chg: FreeCom's command line to be scanned by the internal parser
 * chg: how numerical arguments are scanned (no too low/high warnings)
 * chg: ComPath (absolute path to FreeCom's executable file) is first
 *  derived from the environment (value passed from DOS). It will always
 *  fall back on this value, if a) this is a valid file and b) the
 *  position specified as first argument is invalid.
 *  This behaviour shall ensure that the error messages become avilable
 *  very soon during the run of FreeCom.
 * chg: manual specification of the path to FreeCom can be a directory
 *  or a file. If it's a dir, COMMAND.COM is appended.
 *  It may be relative. <<-- THIS IS A POSSIBLE COMPATIBLY PROBLEM!!
 * chg: the environment is not resized unless instructed so by the /E:
 *  option and/or it fails to set the COMSPEC variable to the absolute
 *  path to the FreeCom executable.
 * chg: options (except /C and /K) can be quoted.
 * chg: load messages -> persistent message flag
 *  By default the messages are loaded into memory and kept there.
 *  When a program is being executed (thus, when FreeCom is shutting
 *  down), it is freed and automatically re-loaded when the program
 *  exits and a message is required. By setting the /MSG option the
 *  messages are persitently loaded until FreeCom exits itself.
 *
 * 2000/01/05 ska
 * add: feature command line logging
 *
 * 2000/06/22 ska
 * add: feature: last directory
 *
 * 2000/07/05 Ron Cemer
 *	bugfix: renamed skipwd() -> skip_word() to prevent duplicate symbol
 *	fix: add typecase for local variables of _fmemcpy()
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <dos.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <alloc.h>
#include <limits.h>   /* INT_MAX */
#include <conio.h>

#include "mcb.h"
#include "environ.h"
#include "dfn.h"
#define _TC_EARLY
#include "fmemory.h"
#include <suppl.h>

//#include "err_hand.h"
#include "batch.h"
#include "timefunc.h"
#include "cmdline.h"
#include "context.h"
#include "command.h"
#include "module.h"

#include "strings.h"

        /* Check for an argument; ch may be evaluated multiple times */
#define isargsign(ch)           \
        ((ch) == ':' || (ch) == '=')

#ifdef FEATURE_CALL_LOGGING
static char logFilename[] = LOG_FILE;
#endif

extern int canexit;

static unsigned oldPSP;
char *ComPath;                   /* absolute filename of COMMAND shell */

int fddebug = 0;    /* debug flag */

static context_t far* far* criterCtxt = 0;
static context_t far* oldCriterCtxt = 0;

/* Without resetting the owner PSP, the program is not removed
   from memory */
void exitfct(void)
{
  unloadMsgs();        /* free the message strings segment */
  if(oldCriterCtxt) {
  	*criterCtxt = oldCriterCtxt;
  } else {
  	freeBlk(FP_SEG(criterCtxt));
  	*(BYTE far*)MK_FP(SEG2MCB(FP_SEG(criterCtxt)), 8 + 4) = '-';
  	dprintf(("[Critical Error handler deallocated (0x%04x).]\n"
  	 , FP_SEG(criterCtxt)));
  }
  OwnerPSP = oldPSP;
}

/*
 * show commands and options that are available.
 *
 */
#pragma argsused
int showcmds(char *rest)
{
  struct CMD *cmdptr;
  unsigned char y;
  extern struct CMD cmds[];     /* The internal command table */

  puts("Internal commands available:");
  y = 0;
  cmdptr = cmds;
  while (cmdptr->name)
  {
    if (++y == 8)
    {
      puts(cmdptr->name);
      y = 0;
    }
    else
      printf("%-10s", cmdptr->name);

    cmdptr++;
  }
  if (y != 0)
    putchar('\n');
  printf("\nFeatures available: ");
#ifdef FEATURE_ALIASES
  printf("[aliases] ");
#endif
#ifdef FEATURE_HISTORY
  printf("[history] ");
#endif
#ifdef FEATURE_FILENAME_COMPLETION
  printf("[filename completion] ");
#endif
#ifdef FEATURE_SWAP_EXEC
  printf("[swapping] ");
#endif
#ifdef FEATURE_CALL_LOGGING
  printf("[start logging] ");
#endif
#ifdef FEATURE_LAST_DIR
  printf("[last dir] ");
#endif
  putchar('\n');

  return 0;
}

int keypressed(void)
{
  union REGS r;

  r.h.ah = 0x01;

  int86(0x16, &r, &r);

  /* Check the zero flag.  Z=0 means a key was pressed; Z=1 means no key */
  if (r.x.flags & 0x40)
    return 0;
  else
    return 1;
}

/* Waits about 3 secs for a keypress.
   returns 0 if none else returns key pressed.
 */

int WaitForFkeys(void)
{
  struct dostime_t start;
  struct dostime_t now;
  int secs = 3;
  int ch;

  _dos_gettime(&start);
  printf("%d", secs);
  while (secs)
  {
    _dos_gettime(&now);
    if (now.second != start.second)
    {
      _dos_gettime(&start);
      secs--;
      printf("\b%d", secs);
    }
    if (keypressed())
    {
      if ((ch = getch()) == 0)
        ch = getch() + 256;
      printf("\b ");
      return ch;
    }
  }
  printf("\b ");
  return 0;
}

#define KEY_F5   319
#define KEY_F8   322


int showhelp = 0, internalBufLen = 0, inputBufLen = 0;
int spawnAndExit = E_None;
int newEnvSize = 0;
char *user_autoexec = NULL;

optScanFct(opt_init)
{ int ec = E_None;

  switch(ch) {
  case '?': showhelp = 1; return E_None;
  case '!': return optScanBool(fddebug);
  case 'Y': return optScanBool(tracemode);
  case 'F': return optScanBool((unsigned)autofail);
  case 'P':
    if(arg)     /* change autoexec.bat */
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
    }
    break;
  }
  optErr();
  return E_Useage;
}


/*
 * Grab the filename of COMMAND.COM
 *
 *  If warn != 0, warnings can be issued; otherwise this functions
 *  is silent.
 */
void grabComFilename(int warn, char far *fnam)
{
  char *buf;
  size_t len;

  assert(fnam);

  /* Copy the filename into the local heap */
  len = _fstrlen(fnam);
  if(len >= INT_MAX || len < 1) {
    /* no filename specified */
    if(warn)
      error_syntax(NULL);
    return;
  }

  if((buf = malloc(len + 1)) == NULL) {
    if(warn) error_out_of_memory();
    return ;
  }
  _fmemcpy((char far*)buf, fnam, len);
  buf[len] = '\0';

    if (buf[1] != ':' || buf[2] != '\\')
    { char *p;

        /* expand the string for the user */
      p = dfnexpand(buf, NULL);
      free(buf);
      if((buf = p) == NULL) {
		  if(warn) error_out_of_memory();
		  return;
      }
      if(warn)
          error_init_fully_qualified(buf);
    }

    if(dfnstat(buf) & DFN_DIRECTORY) {
      /* The user specified a directory, try if we can find the
        COMMAND.COM with the standard name in there */
      char *p;

      if((p = realloc(buf, len + sizeof(COM_NAME) + 1)) == NULL) {
        if(warn) error_out_of_memory();
        free(buf);
        return;
      }
      buf = p;
      strcpy(&buf[len], "\\" COM_NAME);
    }

    if(!(dfnstat(buf) & DFN_FILE)) {
      /* not found */
      if(warn) error_open_file(buf);
      free(buf);
      return;
    }

  free(ComPath);    /* Save the found file */
  ComPath = buf;
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
  int comPath;                /* path to COMMAND.COM (for COMSPEC/reload) */
  char *newTTY;                 /* what to change TTY to */
  int showinfo;                 /* show initial info only if no command line options */
  int key;

  int ec;           /* error code */
  unsigned offs;        /* offset into environment segment */

  int cmdlen;         /* length of command line */
  char *cmdline;        /* command line duplicated into heap */
  char *p, *h, *q;
#ifdef FEATURE_CALL_LOGGING
  FILE *f;
#endif

/* Set up the host environment of COMMAND.COM */

  /* Install the ^Break handler (see chkCBreak() for more details) */
  extern void initCBreakCatcher(void);
  initCBreakCatcher();

  /* DOS shells patch the PPID to the own PID, how stupid this is, however,
    because then DOS won't terminate them, e.g. when a Critical Error
    occurs that is not detected by COMMAND.COM */

  oldPSP = OwnerPSP;
  atexit(exitfct);
  OwnerPSP = _psp;

  /* Some elder DOSs may not pass an initialzied environment segment */
  if (env_glbSeg && !isMCB(SEG2MCB(env_glbSeg)))
    env_setGlbSeg(0);       /* Disable the environment */

/* Now parse the command line parameters passed to COMMAND.COM */
  /* Preparations */
  newTTY = NULL;
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

  /* Aquire the command line, there are three possible sources:
    1) DOS command line @PSP:0x80 as pascal string,
    2) extended DOS command line environment variable CMDLINE,
      if peekb(PSP, 0x80) == 127,&
    3) MKS command line @ENV:2, if peekb(ENV, 0) == '~'

    Currently implemented is version #1 only
  */
  cmdlen = peekb(_psp, 0x80);
  if(cmdlen < 0 || cmdlen > 126) {
    error_corrupt_command_line();
    cmdlen = 0;
  }
    /* duplicate the command line into the local address space */
  if((cmdline = malloc(cmdlen + 1)) == NULL) {
    error_out_of_memory();  /* Cannot recover from this problem */
    return E_NoMem;
  }
  _fmemcpy((char far*)cmdline, MK_FP(_psp, 0x81), cmdlen);
  cmdline[cmdlen] = '\0';
#ifdef FEATURE_CALL_LOGGING
  if((f = fopen(logFilename, "at")) == NULL) {
    fprintf(stderr, "Cannot open logfile: \"%s\"\n", logFilename);
    exit(125);
  }

  putc('"', f);
  if(ComPath)   /* path to command.com already known */
    fputs(ComPath, f);
  putc('"', f);
  putc(':', f);

  fputs(cmdline, f);
  putc('\n', f);
  fclose(f);
#endif

  p = cmdline;    /* start of the command line */
  do {
  ec = leadOptions(&p, opt_init, NULL);
  if(ec == E_NoOption) {    /* /C or /K */
    assert(p && *p);
    if(!isoption(p)) {
      error_quoted_c_k();
      p = NULL;
      break;
    }
    assert(p[1] && strchr("kKcC", p[1]));
    p += 2;   /* p := start of command line to execute */
    break;
  } else if(ec != E_None) {
        showhelp = 1;
    p = NULL;
    break;
  }

  assert(p && !isoption(p) && !isspace(*p));
  if(!*p) {
    p = NULL;
    break;      /* end of line reached */
  }
  q = unquote(p, h = skip_word(p));
  p = h;      /* Skip this word */
  if(!q) {
    error_out_of_memory();
    p = NULL;
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
  if (newTTY) {                   /* change TTY as early as possible so the caller gets
                                   the messages into the correct channel */
    cmd_ctty(newTTY);
    free(newTTY);
  }
#endif

  if(!ComPath) {
    /* FreeCom is unable to find itself --> print error message */
    /* Emergency error */
    puts("You must specify the complete path to " COM_NAME);
    puts("as the first argument of COMMAND,");
    puts("for instance: C:\\FDOS");
    return E_Useage;
  }

  /* First of all, set up the environment */
    /* If a new valid size is specified, use that */
  env_resizeCtrl |= ENV_USEUMB | ENV_ALLOWMOVE;
  if(newEnvSize > 16 && newEnvSize < 32767)
    env_setsize(0, newEnvSize); // SUPPL27+
    //env_newsize(0, newEnvSize);   // SUPPL26-

  /* Otherwise the path is placed into the environment */
  if (chgEnv("COMSPEC", ComPath)) {
    /* Failed to add this variable, the most likely problem should be that
      the environment is too small --> it is increased and the
      operation is redone */
    env_resize(0, strlen(ComPath) + 10);
    if (chgEnv("COMSPEC", ComPath))
    error_env_var("COMSPEC");
  }

  /* Install INT 24 Critical error handler */
  /* Needs the ComPath variable */
  //init_error_handler();
  if((criterCtxt = modCriter()) == 0) {
	puts("Cannot load Critical Error handler.");
	return E_NoMem;
	}
	oldCriterCtxt = *criterCtxt;
	*criterCtxt = (context_t far*)&context;
	setvect(0x24, (void interrupt(*)())(criterCtxt + 1));

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
  if (!canexit)
  {
    char *autoexec;

    autoexec = user_autoexec? user_autoexec: AUTO_EXEC;

    showinfo = 0;
    short_version();

    /* JP: changed so that if autoexec does not exist, then don't ask
       to trace or bypass.
     */
    if (exist(autoexec))
    {
      printf("\nPress F8 for trace mode, or F5 to bypass %s... ", autoexec);
      key = WaitForFkeys();
      putchar('\n');

      if (key == KEY_F8)
      {
        tracemode = 1;
      }

      if (key == KEY_F5)
      {
        printf("Bypassing %s\n", autoexec);
      }
      else
        process_input(1, autoexec);
    }
    else
    {
      if(user_autoexec)
        printf("%s not found.\n", autoexec);
#ifdef INCLUDE_CMD_DATE
      cmd_date(NULL);
#endif
#ifdef INCLUDE_CMD_TIME
      cmd_time(NULL);
#endif
    }

    free(user_autoexec);
  }
  else
  {
    assert(user_autoexec == NULL);
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
    putchar('\n');
    showcmds(NULL);
    putchar('\n');
  }

  return E_None;
}
