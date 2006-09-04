/*
 * config.h - Used to configure what will be compiled into the shell.
 *
 */

#include <io.h>

/* define DEBUG to add debugging code */
#ifndef DEBUG           /* possibly already defined via command line */
//#define DEBUG
#endif

/* Define to enable the alias command, and aliases. */
#define FEATURE_ALIASES
#define ALIAS_DEFAULT_SIZE 1024

/* Define to disable context error messages that may scare a user */
/* #define NO_CONTEXT_ERROR */

/* Define to enable enhanced input (prerequisite of History and Filename
    completion */
#define FEATURE_ENHANCED_INPUT

/* Define to enable history (aka DOSKEY); requires: Enhanced Input */
#define FEATURE_HISTORY
#define HISTORY_DEFAULT_SIZE 256

/* Define to enable filename completion; requires: Enhanced Input */
#define FEATURE_FILENAME_COMPLETION

/* Define to enable to load messages into memory */
#define FEATURE_LOAD_MESSAGES

/* Define to enable /Z support, display exit code on program exit */
#define DISP_EXITCODE

/* Define to enable usage of LFNs */
#define FEATURE_LONG_FILENAMES

/* Define to enable usage or the DOS switch character in option processing */
#define FEATURE_SWITCHAR

/* Define to enable printing of environment variables for "SET VAR" syntax */
/* (Like Win2k's cmd.exe does) */
#define FEATURE_CMD_SET_PRINT

/* Define for limited support of 4DOS DESCRIPT.ION files */
#define FEATURE_DESCRIPT_ION

/* Define to enable DOS NLS */
#define FEATURE_NLS

/* Define to enable F5/F8 key test on startup if /P is present on
    command line */
/* #define FEATURE_BOOT_KEYS */

/* Command line logging feature */
/* #define FEATURE_CALL_LOGGING */

/* Preserves last directory (CD, CHDIR, CDD, PUSHD, POPD);
    "CD -" chdir's there */
#define FEATURE_LAST_DIR

/* Enables CDD to a filepath (ignores filename portion) */
#define FEATURE_CDD_FNAME

/* Enable to support installable COMMAND extensions (MUX-AE)
 */
#define FEATURE_INSTALLABLE_COMMANDS
/* How often gets the MUX-AE called, when an installable command
    extension rewrote the command. */
#define MUX_AE_MAX_REPEAT_CALL  32

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
#define BATCH_NESTLEVEL_MIN 5

/* Define to support kernel-supported swapout of FreeCOM
    see DOCS\K-SWAP.TXT
*/
#define FEATURE_KERNEL_SWAP_SHELL

/* Define to support XMS-only swap support of FreeCOM
    This setting is incompatible with the above one!
*/
/* #define FEATURE_XMS_SWAP */

/* Define the size of the buffer used to store old paths for PUSHD/POPD */
#define DIRSTACK_DEFAULT_SIZE 256

/* Use this errorlevel if an external program was terminated by
    ^C or ^Break --> DOS error code interrupted system call */
#define CBREAK_ERRORLEVEL 0x5F

/* Use these filemode while searching for file completion */
#define FILE_SEARCH_MODE FA_RDONLY | FA_ARCH | FA_DIREC

/* If no /E: is specified, always keep this amount of bytes
    free within the environment */
#define ENVIRONMENT_KEEP_FREE 256


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

#define INCLUDE_CMD_BEEP
#define INCLUDE_CMD_BREAK
#define INCLUDE_CMD_CHDIR
#define INCLUDE_CMD_CHCP
#define INCLUDE_CMD_CDD
#define INCLUDE_CMD_CLS
#define INCLUDE_CMD_COPY
#define INCLUDE_CMD_CTTY
#define INCLUDE_CMD_DATE
#define INCLUDE_CMD_DEL
#define INCLUDE_CMD_DIR
#define INCLUDE_CMD_DIRS
#ifdef FEATURE_LONG_FILENAMES
#define INCLUDE_CMD_LFNFOR
#endif
#define INCLUDE_CMD_LOADFIX
#define INCLUDE_CMD_LOADHIGH
/*#define INCLUDE_CMD_FAKELOADHIGH */ /* DO NOT include this and CMD_LOADHIGH nor CMD_LOADFIX */
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
#define INCLUDE_CMD_CDD
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

#if defined(FEATURE_KERNEL_SWAP_SHELL) && defined(FEATURE_XMS_SWAP)
#error "FreeCOM cannot support both Kernel- and XMS-based swapping simultaneously"
#endif

/* Externally defined to compile with XMS-Swap support */
#if defined(XMS_SWAP)
#undef FEATURE_KERNEL_SWAP_SHELL
#define FEATURE_XMS_SWAP
#endif

#if sizeof(char*) > sizeof(short)
/* Compiling in Large memory model --> turn of Warning:
    "Conversation may lose significant digits"  */
#pragma warn -sig
#endif

#include "../include/debug.h"
