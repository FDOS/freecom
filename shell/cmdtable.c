/*
 *  CMDTABLE.C - table of internal commands.
 *
 */

#include <stdlib.h>

#include "../config.h"
#include "../include/command.h"
#include "../strings.h"


/* a list of all the internal commands, associating their command names */
/* to the functions to process them                                     */

#define HIDDEN (CMD_HIDDEN | CMD_HIDDEN_DEFAULT)
#define ENABLED 0

struct CMD internalCommands[] =
{
#ifdef FEATURE_ALIASES
	{"ALIAS", ENABLED, cmd_alias, TEXT_CMDHELP_ALIAS},
#endif

#ifdef INCLUDE_CMD_BEEP
	{"BEEP", ENABLED, cmd_beep, TEXT_CMDHELP_BEEP},
#endif

#ifdef INCLUDE_CMD_BREAK
	{"BREAK", ENABLED, cmd_break, TEXT_CMDHELP_BREAK},
#endif

	{"CALL", CMD_SPECIAL_DIR | CMD_BATCHONLY, cmd_call, TEXT_CMDHELP_CALL},

#ifdef INCLUDE_CMD_CHDIR
	{"CD", CMD_SPECIAL_DIR, cmd_chdir, TEXT_CMDHELP_CD},
	{"CHDIR", CMD_SPECIAL_DIR, cmd_chdir, TEXT_CMDHELP_CD},
#endif

#ifdef INCLUDE_CMD_CDD
	{"CDD", CMD_SPECIAL_DIR, cmd_cdd, TEXT_CMDHELP_CDD},
#endif

/*    { "CHCP", 0, cmd_chcp, TEXT_CMDHELP_CHCP}, !!    */

#ifdef INCLUDE_CMD_CLS
	{"CLS", ENABLED, cmd_cls, TEXT_CMDHELP_CLS},
#endif

#ifdef INCLUDE_CMD_COPY
	{ "COPY", CMD_SPECIAL_DIR, cmd_copy, TEXT_CMDHELP_COPY },
#endif

#ifdef INCLUDE_CMD_CTTY
	{ "CTTY", ENABLED, cmd_ctty, TEXT_CMDHELP_CTTY},
#endif

#ifdef INCLUDE_CMD_DATE
	{"DATE", ENABLED, cmd_date, TEXT_CMDHELP_DATE},
#endif

#ifdef INCLUDE_CMD_DEL
	{"DEL", CMD_SPECIAL_DIR, cmd_del, TEXT_CMDHELP_DEL},
#endif

#ifdef INCLUDE_CMD_DIR
	{"DIR", CMD_SPECIAL_DIR, cmd_dir, TEXT_CMDHELP_DIR},
#endif

#ifdef INCLUDE_CMD_DIRS
	{"DIRS", ENABLED, cmd_dirs, TEXT_CMDHELP_DIRS},
#endif

#ifdef FEATURE_HISTORY
	{"DOSKEY", ENABLED, cmd_doskey, TEXT_CMDHELP_DOSKEY},
#endif

	{"ECHO", CMD_SPECIAL_ALL, cmd_echo, TEXT_CMDHELP_ECHO},
	{"ECHOERR", CMD_SPECIAL_ALL | HIDDEN, cmd_echoerr, TEXT_CMDHELP_ECHO},

#ifdef INCLUDE_CMD_DEL
	{"ERASE", CMD_SPECIAL_DIR, cmd_del, TEXT_CMDHELP_DEL},
#endif

	{"EXIT", ENABLED, cmd_exit, TEXT_CMDHELP_EXIT},

	{"FOR", ENABLED, cmd_for, TEXT_CMDHELP_FOR},

	{"GOTO", CMD_BATCHONLY, cmd_goto, TEXT_CMDHELP_GOTO},

#ifdef FEATURE_HISTORY
	{"HISTORY", ENABLED, cmd_history, TEXT_CMDHELP_HISTORY},
#endif

	{"IF", ENABLED, cmd_if, TEXT_CMDHELP_IF},

#ifdef INCLUDE_CMD_LOADHIGH
	{"LH", CMD_SPECIAL_DIR, cmd_loadhigh, TEXT_CMDHELP_LH},
#endif

#ifdef INCLUDE_CMD_LOADFIX
	{"LOADFIX", CMD_SPECIAL_DIR, cmd_loadfix, TEXT_CMDHELP_LOADFIX},
#endif

#ifdef INCLUDE_CMD_LOADHIGH
	{"LOADHIGH", CMD_SPECIAL_DIR, cmd_loadhigh, TEXT_CMDHELP_LH},
#endif

#ifdef INCLUDE_CMD_MEMORY
	{"MEMORY", ENABLED, cmd_memory, TEXT_CMDHELP_MEMORY},
#endif

#ifdef INCLUDE_CMD_MKDIR
	{"MD", CMD_SPECIAL_DIR, cmd_mkdir, TEXT_CMDHELP_MD},
	{"MKDIR", CMD_SPECIAL_DIR, cmd_mkdir, TEXT_CMDHELP_MD},
#endif

#ifdef INCLUDE_CMD_PATH
	{"PATH", CMD_SPECIAL_ALL, cmd_path, TEXT_CMDHELP_PATH},
#endif

#ifdef INCLUDE_CMD_PAUSE
	{"PAUSE", ENABLED, cmd_pause, TEXT_CMDHELP_PAUSE},
#endif

#ifdef INCLUDE_CMD_PROMPT
	{"PROMPT", ENABLED, cmd_prompt, TEXT_CMDHELP_PROMPT},
#endif

#ifdef INCLUDE_CMD_PUSHD
	{"PUSHD", CMD_SPECIAL_DIR, cmd_pushd, TEXT_CMDHELP_PUSHD},
#endif

#ifdef INCLUDE_CMD_POPD
	{"POPD", ENABLED, cmd_popd, TEXT_CMDHELP_POPD},
#endif

#ifdef INCLUDE_CMD_RMDIR
	{"RD", CMD_SPECIAL_DIR, cmd_rmdir, TEXT_CMDHELP_RD},
#endif

#ifdef INCLUDE_CMD_REM
	{"REM", ENABLED, cmd_rem, TEXT_CMDHELP_REM},
#endif

#ifdef INCLUDE_CMD_RENAME
	{"REN", CMD_SPECIAL_DIR, cmd_rename, TEXT_CMDHELP_REN},
	{"RENAME", CMD_SPECIAL_DIR, cmd_rename, TEXT_CMDHELP_REN},
#endif

#ifdef INCLUDE_CMD_RMDIR
	{"RMDIR", CMD_SPECIAL_DIR, cmd_rmdir, TEXT_CMDHELP_RD},
#endif

#ifdef INCLUDE_CMD_SET
	{"SET", ENABLED, cmd_set, TEXT_CMDHELP_SET},
#endif

	{"SHIFT", CMD_BATCHONLY, cmd_shift, TEXT_CMDHELP_SHIFT},

#ifdef INCLUDE_CMD_TIME
	{"TIME", ENABLED, cmd_time, TEXT_CMDHELP_TIME},
#endif

#ifdef INCLUDE_CMD_TRUENAME
	{"TRUENAME", CMD_SPECIAL_DIR, cmd_truename, TEXT_CMDHELP_TRUENAME},
#endif

#ifdef INCLUDE_CMD_TYPE
	{"TYPE", CMD_SPECIAL_DIR, cmd_type, TEXT_CMDHELP_TYPE},
#endif

#ifdef INCLUDE_CMD_VER
	{"VER", ENABLED, cmd_ver, TEXT_CMDHELP_VER},
#endif

#ifdef INCLUDE_CMD_VERIFY
	{"VERIFY", ENABLED, cmd_verify, TEXT_CMDHELP_VERIFY},
#endif

#ifdef INCLUDE_CMD_VOL
	{"VOL", ENABLED, cmd_vol, TEXT_CMDHELP_VOL},
#endif

#ifdef INCLUDE_CMD_QUESTION
	{"?", ENABLED, showcmds, TEXT_CMDHELP_QUESTION},
#endif

#ifdef DEBUG
	{"FDDEBUG", ENABLED, cmd_fddebug, TEXT_CMDHELP_FDDEBUG},
#endif

#ifdef INCLUDE_CMD_WHICH
	{"WHICH", ENABLED, cmd_which, TEXT_CMDHELP_WHICH},
#endif

/* Internally used <-> hidden commands
	these ones are usually invoked via the ::= hack */

	{"ICMDS", HIDDEN, cmd_icmds, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"IVAR", HIDDEN, cmd_ivar, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"FD", HIDDEN, cmd_fd, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"ARG", HIDDEN, cmd_arg, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"CANCEL", HIDDEN, cmd_cancel, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"QUIT", HIDDEN, cmd_quit, TEXT_CMDHELP_INTERNAL_COMMAND},

#ifdef DEBUG
	{"@", HIDDEN, cmd_dispNLS, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"DISPLAY_NLS", HIDDEN, cmd_dispNLS, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"!", HIDDEN, cmd_dispFlags, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"DISPLAY_FLAGS", HIDDEN, cmd_dispFlags, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"DISPLAY_ARGS", HIDDEN, cmd_dispArgs, TEXT_CMDHELP_INTERNAL_COMMAND},
	{"DISPLAY_IVARS", HIDDEN, cmd_dispIVars, TEXT_CMDHELP_INTERNAL_COMMAND},
#endif

	{0, 0, 0, 0}
};
