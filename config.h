/*
 * config.h - Used to configure what will be compiled into the shell.
 *
 */

/* define DEBUG to add debugging code */
#ifndef DEBUG			/* possibly already defined via command line */
//#define DEBUG
#endif

/* Define to enable the alias command, and aliases. */
#define FEATURE_ALIASES
#define ALIAS_DEFAULT_SIZE 1024

/* Define to enable enhanced input (prerequisite of History and Filename
	completion */
#define FEATURE_ENHANCED_INPUT
/* If this feature is NOT used, DOS-0A is called, specify the internal
	buffer for this API function below. */
#define MAX_INTERNAL_COMMAND_SIZE 256


/* Define to enable history (aka DOSKEY); requires: Enhanced Input */
#define FEATURE_HISTORY
#define HISTORY_DEFAULT_SIZE 256

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

/* How many batch files should be nestable minimally */
/* OBSOLETED -- DON'T USE!
	see EXEC_CONTEXT_MINIMUM_SIZE instead */
/*** obsolted #define BATCH_NESTLEVEL_MIN 5 ***/

/* How many bytes may be used at minimum by batch file nesting,
	FOR loops, redirection state recordings,& buffer for batch script
	arguments, previous contents of FOR variables and internal
	variables */
#define EXEC_CONTEXT_MINIMUM_SIZE 512

/* Define to support kernel-supported swapout of FreeCOM
	see DOCS\K-SWAP.TXT
*/
#define FEATURE_KERNEL_SWAP_SHELL

/* Define the size of the buffer used to store old paths for PUSHD/POPD */
#define DIRSTACK_DEFAULT_SIZE 256

/* Use this errorlevel if an external program was terminated by
	^C or ^Break */
#define CBREAK_ERRORLEVEL 3

/* Use these filemode while searching for file completion */
#define FILE_SEARCH_MODE FA_RDONLY | FA_ARCH | FA_DIREC


/* Default message settings:
  PATTERN: how the string is constructed for the ID (with \n)
  OUTOFMEMORY: string to issue on out-of-memory condition (no \n)
  ID_: the error ID when no default pattern is to be created, but
  	the out-of-memory string is to be displayed
*/
#define MSG_DFL_PATTERN "String #%u\n"
#define MSG_DFL_OUTOFMEMORY "Out of memory!"
#define MSG_ERR_ID_OUTOFMEMORY TEXT_ERROR_OUT_OF_MEMORY


/* Default prompt */
#define DEFAULT_PROMPT "$P$G"

/* Provides the uppermost size the context may have */
#define CONTEXT_MAX_SIZE (65535U - 12)

/* Define this value to select the initialization value of fddebug */
#define FDDEBUG_INIT_VALUE 1

/* How many dynamically allocated strings shall be internally cached
	before they are considered "old" and gets removed. */
#define STRING_CACHE_ITEMS 5

/* What quotes COMMAND shell honor (only paired quotes) */
/* Note: at least the double quotes MUST be included */
#define QUOTE_STR "\"'`"

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
#define INCLUDE_CMD_MEMORY
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

/* Define if your compiler does not have 'dosdate_t' or 'dostime_t' */
#ifdef _TC_EARLY_
	/* TC++1 */
#define _NO__DOS_DATE
#define _NO__DOS_TIME
#define _NO_FMEMCHR
#define _NO_FMEMCMP
#endif


	/* set by MKDIST.BAT */
#ifdef IGNORE_ENHANCED_INPUT
#undef FEATURE_ENHANCED_INPUT
#endif

#ifndef FEATURE_ENHANCED_INPUT
#undef FEATURE_HISTORY
#undef FEATURE_FILENAME_COMPLETION
#endif

#ifdef INCLUDE_CMD_PUSHD
#define FEATURE_DIRSTACK
#endif

#if CONTEXT_MAX_SIZE > 65535U - 12
#error "The maximal context size may not exceed 65535 - 12 bytes"
#endif

#if sizeof(char*) > sizeof(short)
/* Compiling in Large memory model --> turn of Warning:
	"Conversation may lose significant digits"	*/
#pragma warn -sig
#endif

#define FORCE_INTERNAL_COMMAND_STRING "::="

#include "../include/debug.h"
