/*
 * config.h - Used to configure what will be compiled into the shell.
 *
 *
 * Comments:
 * 27 Jul 1998 - John P. Price
 * - started.
 *
 * 12-Aug-1998 ska
 * - added CTTY macro
 *
 * 2000/01/05 ska
 * chg: moved some declarations from INIT.C here (COM_NAME, AUTO_EXEC, &
 *	LOG_NAME)
 *
 * 2000/01/15 ska
 * add: FEATURE_AUTOREDIRECT_TO_CON: Number of loops after the output is
 *	redirected to CON, if FreeCom hangs in "hangForever()" in COMMAND.C
 *
 * 2000/06/22 ska
 *	add: DIR_STACK_LEN, commands: DIRS, POPD, PUSHD, CDD
 *	add: FEATURE_LAST_DIR
 *
 * 2000/09/26 ska
 *	add: FEATURE_ENHANCED_INPUT
 *
 * 2000/12/10 ska
 *	add: FEATURE_INSTALLABLE_COMMANDS
 */

/* define DEBUG to add debugging code */
#ifndef DEBUG			/* possibly already defined via command line */
//#define DEBUG
#endif

/* Define to enable the alias command, and aliases. */
#define FEATURE_ALIASES

/* Define to enable enhanced input (prerequisite of History and Filename
	completion */
#define FEATURE_ENHANCED_INPUT

/* Define to enable history (aka DOSKEY); requires: Enhanced Input */
#define FEATURE_HISTORY

/* Define to enable filename completion; requires: Enhanced Input */
#define FEATURE_FILENAME_COMPLETION

/* Define to enable to load messages into memory */
#define FEATURE_LOAD_MESSAGES

/* Define to enable DOS NLS */
#define FEATURE_NLS

/* Command line logging feature */
//#define FEATURE_CALL_LOGGING

/* Preserves last directory (CD, CHDIR, CDD, PUSHD, POPD);
	"CD -" chdir's there */
#define FEATURE_LAST_DIR

/* Enable to support installable COMMAND extensions (MUX-AE)
 */
#define FEATURE_INSTALLABLE_COMMANDS

/* Name of the executable */
#define COM_NAME "COMMAND.COM"
/* standard name of AUTOEXEC.BAT */
#define AUTO_EXEC "\\autoexec.bat"
/* Logfile for some logging features; MUST be fully-qualified! */
#define LOG_FILE "C:\\FreeCom.log"

/* Define to number of loops when to redirect to CON if the
	shells hangs in the "hangForEver()" loop
   Undefine to remove this feature */
#define FEATURE_AUTO_REDIRECT_TO_CON 5

/* Define to support kernel-supported swapout of FreeCOM
	see DOCS\K-SWAP.TXT
*/
#define FEATURE_KERNEL_SWAP_SHELL

/* Define the size of the buffer used to store old paths for PUSHD/POPD */
#define DIR_STACK_LEN 256

/* Define this value to select the initialization value of fddebug */
#define FDDEBUG_INIT_VALUE 1

/* Define if your compiler does not have 'dosdate_t' or 'dostime_t' */
#if defined(__TURBOC__) && __TURBOC__ <= 0x297
	/* TC++1 */
#define _NO__DOS_DATE
#define _NO__DOS_TIME
#define _NO_FMEMCHR
#define _NO_FMEMCMP
#endif


#define INCLUDE_CMD_BEEP
#define INCLUDE_CMD_BREAK
#define INCLUDE_CMD_CHDIR
#define INCLUDE_CMD_CDD
#define INCLUDE_CMD_CLS
#define INCLUDE_CMD_COPY
#define INCLUDE_CMD_CTTY
#define INCLUDE_CMD_DATE
#define INCLUDE_CMD_DEL
#define INCLUDE_CMD_DIR
#define INCLUDE_CMD_DIRS
#define INCLUDE_CMD_LOADFIX
#define INCLUDE_CMD_LOADHIGH
#define INCLUDE_CMD_MKDIR
#define INCLUDE_CMD_PATH
#define INCLUDE_CMD_PAUSE
#define INCLUDE_CMD_PROMPT
#define INCLUDE_CMD_PUSHD
#define INCLUDE_CMD_POPD
#define INCLUDE_CMD_REM
#define INCLUDE_CMD_RENAME
#define INCLUDE_CMD_RMDIR
#define INCLUDE_CMD_SET
#define INCLUDE_CMD_TIME
#define INCLUDE_CMD_TRUENAME
#define INCLUDE_CMD_TYPE
#define INCLUDE_CMD_VER
#define INCLUDE_CMD_VERIFY
#define INCLUDE_CMD_VOL
#define INCLUDE_CMD_QUESTION
#define INCLUDE_CMD_WHICH
#ifdef DEBUG
#define INCLUDE_CMD_FDDEBUG
#endif
/*
   command that do not have a define:

   exit
   call
   echo
   goto
   for
   if
   shift

 */

/********
	***** Resolve dependencies
	***** Don't change without change the appropriate sources!
	************/
#if defined(INCLUDE_CMD_PUSHD) || defined(INCLUDE_CMD_POPD)
#define	INCLUDE_CMD_CDD
#endif

/* Not available with TURBOC++ 1.0 or earlier: */
#ifdef __TURBOC__

#if __TURBOC__ > 0x297
#define _TC_LATER_
#else
#define _TC_EARLY_
#endif

#endif

	/* set by MKDIST.BAT */
#ifdef IGNORE_ENHANCED_INPUT
#undef FEATURE_ENHANCED_INPUT
#endif

#ifndef FEATURE_ENHANCED_INPUT
#undef FEATURE_HISTORY
#undef FEATURE_FILENAME_COMPLETION
#endif

#include "debug.h"
