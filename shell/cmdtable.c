/* $Id$
 *  CMDTABLE.C - table of internal commands.
 *
 *  Comments:
 *
 * 16 Jul 1998 (Hans B Pufal)
 *   started.
 *   New file to keep the internal command table. I plan on
 *   getting rid of the table real soon now and replacing it
 *   with a dynamic mechnism.
 *
 *
 * 27 Jul 1998  John P. Price
 * - added config.h include
 *
 * 12-Aug-1998 ska
 * - enabled the reference to the CTTY dummy
 *
 * 2000/01/15 ska
 * - CTTY is dummy no longer
 *
 * 2000/06/22 ska
 *	add: DIRS/PUSHD/POPD patch D. Lucas Parker
 *	add: CDD
 *
 * 2000/12/10 ska
 *	chg: Uppercased command names (support for Installable Commands)
 *
 * 2001/02/16 ska
 * add: command WHICH
 */

#include <stdlib.h>

#include "../config.h"
#include "../include/command.h"
#include "../strings.h"


/* a list of all the internal commands, associating their command names */
/* to the functions to process them                                     */

/* Lines marked

 * $$ are external commands
 * !! internal commands which are not yet implemented
 * ** special FREEDOS specific implementation
 */

struct CMD internalCommands[] =
{
#ifdef FEATURE_ALIASES
        {"ALIAS", 0, cmd_alias, TEXT_CMDHELP_ALIAS},
#endif

#ifdef INCLUDE_CMD_BEEP
        {"BEEP", 0, cmd_beep, TEXT_CMDHELP_BEEP},
#endif

#ifdef INCLUDE_CMD_BREAK
        {"BREAK", CMD_NO_ERRORLEVEL, cmd_break, TEXT_CMDHELP_BREAK},
#endif

        {"CALL", CMD_SPECIAL_DIR | CMD_BATCHONLY, cmd_call, TEXT_CMDHELP_CALL},

#ifdef INCLUDE_CMD_CHDIR
  {"CD", CMD_SPECIAL_DIR, cmd_chdir, TEXT_CMDHELP_CD},
  {"CHDIR", CMD_SPECIAL_DIR, cmd_chdir, TEXT_CMDHELP_CD},
#endif

#ifdef INCLUDE_CMD_CDD
	{"CDD", CMD_SPECIAL_DIR, cmd_cdd, TEXT_CMDHELP_CDD},
#endif

#ifdef INCLUDE_CMD_CHCP
    { "CHCP",     0,             cmd_chcp, TEXT_CMDHELP_CHCP},
#endif

#ifdef INCLUDE_CMD_CLS
  {"CLS", CMD_NO_ERRORLEVEL, cmd_cls, TEXT_CMDHELP_CLS},
#endif

#ifdef INCLUDE_CMD_COPY
        { "COPY", CMD_SPECIAL_DIR,      cmd_copy, TEXT_CMDHELP_COPY },
#endif

#ifdef INCLUDE_CMD_CTTY
        { "CTTY",     0,             cmd_ctty      , TEXT_CMDHELP_CTTY},
#endif

#ifdef INCLUDE_CMD_DATE
  {"DATE", CMD_NO_ERRORLEVEL, cmd_date, TEXT_CMDHELP_DATE},
#endif

#ifdef INCLUDE_CMD_DEL
  {"DEL", CMD_SPECIAL_DIR, cmd_del, TEXT_CMDHELP_DEL},
#endif

#ifdef INCLUDE_CMD_DIR
  {"DIR", CMD_SPECIAL_DIR, cmd_dir, TEXT_CMDHELP_DIR},
#endif

#ifdef INCLUDE_CMD_DIRS
  {"DIRS", 0, cmd_dirs, TEXT_CMDHELP_DIRS},
#endif

#ifdef FEATURE_HISTORY
  {"DOSKEY", CMD_NO_ERRORLEVEL, cmd_doskey, TEXT_CMDHELP_DOSKEY},
#endif

  {"ECHO", CMD_SPECIAL_ALL | CMD_NO_ERRORLEVEL, cmd_echo, TEXT_CMDHELP_ECHO},

#ifdef INCLUDE_CMD_DEL
  {"ERASE", CMD_SPECIAL_DIR, cmd_del, TEXT_CMDHELP_DEL},
#endif

  {"EXIT", 0, internal_exit, TEXT_CMDHELP_EXIT},
#ifdef DEBUG
  {"EXIT!!", 0, force_exit, TEXT_CMDHELP_EXIT},
#endif

  {"FOR", CMD_NO_ERRORLEVEL, cmd_for, TEXT_CMDHELP_FOR},

  {"GOTO", CMD_BATCHONLY | CMD_NO_ERRORLEVEL, cmd_goto, TEXT_CMDHELP_GOTO},

#ifdef FEATURE_HISTORY
  {"HISTORY", 0, cmd_history, TEXT_CMDHELP_HISTORY},
#endif

  {"IF", CMD_NO_ERRORLEVEL, cmd_if, TEXT_CMDHELP_IF},

#ifdef FEATURE_LONG_FILENAMES
#ifdef INCLUDE_CMD_LFNFOR
  {"LFNFOR", CMD_NO_ERRORLEVEL, cmd_lfnfor, TEXT_CMDHELP_LFNFOR},
#endif
#endif

#ifdef INCLUDE_CMD_FAKELOADHIGH
  {"LH", CMD_SPECIAL_DIR, cmd_call, TEXT_CMDHELP_CALL},
  {"LOADHIGH", CMD_SPECIAL_DIR, cmd_call, TEXT_CMDHELP_CALL},
#endif

#ifdef INCLUDE_CMD_LOADHIGH
  {"LH", CMD_SPECIAL_DIR, cmd_loadhigh, TEXT_CMDHELP_LH},
  {"LOADHIGH", CMD_SPECIAL_DIR, cmd_loadhigh, TEXT_CMDHELP_LH},
#endif

#ifdef INCLUDE_CMD_LOADFIX
  {"LOADFIX", CMD_SPECIAL_DIR, cmd_loadfix, TEXT_CMDHELP_LOADFIX},
#endif

#ifdef INCLUDE_CMD_MEMORY
  {"MEMORY", CMD_NO_ERRORLEVEL, cmd_memory, TEXT_CMDHELP_MEMORY},
#endif

#ifdef INCLUDE_CMD_MKDIR
  {"MD", CMD_SPECIAL_DIR, cmd_mkdir, TEXT_CMDHELP_MD},
  {"MKDIR", CMD_SPECIAL_DIR, cmd_mkdir, TEXT_CMDHELP_MD},
#endif

#ifdef INCLUDE_CMD_PATH
  {"PATH", CMD_SPECIAL_ALL, cmd_path, TEXT_CMDHELP_PATH},
#endif

#ifdef INCLUDE_CMD_PAUSE
  {"PAUSE", CMD_NO_ERRORLEVEL, cmd_pause, TEXT_CMDHELP_PAUSE},
#endif

#ifdef INCLUDE_CMD_PROMPT
  {"PROMPT", 0, cmd_prompt, TEXT_CMDHELP_PROMPT},
#endif

#ifdef INCLUDE_CMD_PUSHD
  {"PUSHD", CMD_SPECIAL_DIR | CMD_NO_ERRORLEVEL, cmd_pushd, TEXT_CMDHELP_PUSHD},
#endif

#ifdef INCLUDE_CMD_POPD
  {"POPD", CMD_NO_ERRORLEVEL, cmd_popd, TEXT_CMDHELP_POPD},
#endif

#ifdef INCLUDE_CMD_RMDIR
  {"RD", CMD_SPECIAL_DIR | CMD_NO_ERRORLEVEL, cmd_rmdir, TEXT_CMDHELP_RD},
#endif

#ifdef INCLUDE_CMD_REM
  {"REM", CMD_NO_ERRORLEVEL, cmd_rem, TEXT_CMDHELP_REM},
#endif

#ifdef INCLUDE_CMD_RENAME
  {"REN", CMD_SPECIAL_DIR, cmd_rename, TEXT_CMDHELP_REN},
  {"RENAME", CMD_SPECIAL_DIR, cmd_rename, TEXT_CMDHELP_REN},
#endif

#ifdef INCLUDE_CMD_RMDIR
  {"RMDIR", CMD_SPECIAL_DIR | CMD_NO_ERRORLEVEL, cmd_rmdir, TEXT_CMDHELP_RD},
#endif

#ifdef INCLUDE_CMD_SET
  {"SET", 0, cmd_set, TEXT_CMDHELP_SET},
#endif

  {"SHIFT", CMD_BATCHONLY | CMD_NO_ERRORLEVEL, cmd_shift, TEXT_CMDHELP_SHIFT},

#ifdef INCLUDE_CMD_TIME
  {"TIME", CMD_NO_ERRORLEVEL, cmd_time, TEXT_CMDHELP_TIME},
#endif

#ifdef INCLUDE_CMD_REM
  {"TITLE", CMD_NO_ERRORLEVEL, cmd_rem, TEXT_CMDHELP_REM},
#endif

#ifdef INCLUDE_CMD_TRUENAME
  {"TRUENAME", CMD_SPECIAL_DIR | CMD_NO_ERRORLEVEL, cmd_truename, TEXT_CMDHELP_TRUENAME},
#endif

#ifdef INCLUDE_CMD_TYPE
  {"TYPE", CMD_SPECIAL_DIR, cmd_type, TEXT_CMDHELP_TYPE},
#endif

#ifdef INCLUDE_CMD_VER
  {"VER", CMD_NO_ERRORLEVEL, cmd_ver, TEXT_CMDHELP_VER},
#endif

#ifdef INCLUDE_CMD_VERIFY
  {"VERIFY", 0, cmd_verify, TEXT_CMDHELP_VERIFY},
#endif

#ifdef INCLUDE_CMD_VOL
        {"VOL", 0, cmd_vol, TEXT_CMDHELP_VOL},
#endif

#ifdef INCLUDE_CMD_QUESTION
  {"?", CMD_NO_ERRORLEVEL, showcmds, TEXT_CMDHELP_QUESTION},
#endif

#ifdef DEBUG
  {"FDDEBUG", CMD_NO_ERRORLEVEL, cmd_fddebug, TEXT_CMDHELP_FDDEBUG},
#endif

#ifdef INCLUDE_CMD_WHICH
  {"WHICH", 0, cmd_which, TEXT_CMDHELP_WHICH},
#endif


        {0, 0, 0, 0}
};

/*    append,                             $$    */
/*    assign,                             $$    */
/*    attrib,                             $$    */
/*    backup,                             $$    */
/*    chkdsk,                             $$    */
/*    command,                              $$    */
/*    comp,                               $$    */
/*    debug,                              $$    */
/*    diskcomp,                           $$    */
/*    diskcopy,                           $$    */
/*    doskey,                             **    */
/*    dosshell,                           $$    */
/*    edit,                               $$    */
/*    edlin,                              $$    */
/*    emm386,                             $$    */
/*    exe2bin,                              $$    */
/*    expand,                             $$    */
/*    fastopen,                           $$    */
/*    fc,                                 $$    */
/*    fdisk,                              $$    */
/*    find,                               $$    */
/*    format,                             $$    */
/*    graftabl,                           $$    */
/*    graphics,                           $$    */
/*    help,                               $$    */
/*    join,                               $$    */
/*    keyb,                               $$    */
/*    label,                              $$    */
/*    mem,                                $$    */
/*    mirror,                             $$    */
/*    mode,                               $$    */
/*    more,                               $$    */
/*    nlsfunc,                              $$    */
/*    print,                              $$    */
/*    qbasic,                             $$    */
/*    recover,                              $$    */
/*    replace,                              $$    */
/*    restore,                              $$    */
/*    setver,                             $$    */
/*    share,                              $$    */
/*    sort,                               $$    */
/*    subst,                              $$    */
/*    sys,                                $$    */
/*    tree,                                $$    */
/*    undelete,                           $$    */
/*    unformat,                           $$    */
/*    xcopy,                              $$    */
/*    pushd,                              **    */
/*    popd,                               **    */
