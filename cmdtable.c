/*
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
 */

#include <stdlib.h>

#include "config.h"
#include "command.h"
#include "strings.h"


/* a list of all the internal commands, associating their command names */
/* to the functions to process them                                     */

/* Lines marked

 * $$ are external commands
 * !! internal commands which are not yet implemented
 * ** special FREEDOS specific implementation
 */

struct CMD cmds[] =
{
#ifdef FEATURE_ALIASES
        {"alias", 0, cmd_alias, TEXT_CMDHELP_ALIAS},
#endif

#ifdef INCLUDE_CMD_BEEP
        {"beep", 0, cmd_beep, TEXT_CMDHELP_BEEP},
#endif

#ifdef INCLUDE_CMD_BREAK
        {"break",    0,             cmd_break     , TEXT_CMDHELP_BREAK},
#endif

        {"call", CMD_SPECIAL_DIR | CMD_BATCHONLY, cmd_call, TEXT_CMDHELP_CALL},

#ifdef INCLUDE_CMD_CHDIR
  {"cd", CMD_SPECIAL_DIR, cmd_chdir, TEXT_CMDHELP_CD},
  {"chdir", CMD_SPECIAL_DIR, cmd_chdir, TEXT_CMDHELP_CD},
#endif

/*    { "chcp",     0,             cmd_chcp, TEXT_CMDHELP_CHCP},    !!    */

#ifdef INCLUDE_CMD_CLS
  {"cls", 0, cmd_cls, TEXT_CMDHELP_CLS},
#endif

#ifdef INCLUDE_CMD_COPY
        { "copy", CMD_SPECIAL_DIR,      cmd_copy, TEXT_CMDHELP_COPY },
#endif

#ifdef INCLUDE_CMD_CTTY
        { "ctty",     0,             cmd_ctty      , TEXT_CMDHELP_CTTY},
#endif

#ifdef INCLUDE_CMD_DATE
  {"date", 0, cmd_date, TEXT_CMDHELP_DATE},
#endif

#ifdef INCLUDE_CMD_DEL
  {"del", CMD_SPECIAL_DIR, cmd_del, TEXT_CMDHELP_DEL},
#endif

#ifdef INCLUDE_CMD_DIR
  {"dir", CMD_SPECIAL_DIR, cmd_dir, TEXT_CMDHELP_DIR},
#endif

#ifdef FEATURE_HISTORY
  {"doskey", 0, cmd_doskey, TEXT_CMDHELP_DOSKEY},
#endif

  {"echo", CMD_SPECIAL_ALL, cmd_echo, TEXT_CMDHELP_ECHO},

#ifdef INCLUDE_CMD_DEL
  {"erase", CMD_SPECIAL_DIR, cmd_del, TEXT_CMDHELP_DEL},
#endif

  {"exit", 0, internal_exit, TEXT_CMDHELP_EXIT},

  {"for", 0, cmd_for, TEXT_CMDHELP_FOR},

  {"goto", CMD_BATCHONLY, cmd_goto, TEXT_CMDHELP_GOTO},

#ifdef FEATURE_HISTORY
  {"history", 0, cmd_history, TEXT_CMDHELP_HISTORY},
#endif

  {"if", 0, cmd_if, TEXT_CMDHELP_IF},

#ifdef INCLUDE_CMD_LOADHIGH
  {"lh", CMD_SPECIAL_DIR, cmd_loadhigh, TEXT_CMDHELP_LH},
#endif

#ifdef INCLUDE_CMD_LOADFIX
  {"loadfix", CMD_SPECIAL_DIR, cmd_loadfix, TEXT_CMDHELP_LOADFIX},
#endif

#ifdef INCLUDE_CMD_LOADHIGH
  {"loadhigh", CMD_SPECIAL_DIR, cmd_loadhigh, TEXT_CMDHELP_LH},
#endif

#ifdef INCLUDE_CMD_MKDIR
  {"md", CMD_SPECIAL_DIR, cmd_mkdir, TEXT_CMDHELP_MD},
  {"mkdir", CMD_SPECIAL_DIR, cmd_mkdir, TEXT_CMDHELP_MD},
#endif

#ifdef INCLUDE_CMD_PATH
  {"path", 0, cmd_path, TEXT_CMDHELP_PATH},
#endif

#ifdef INCLUDE_CMD_PAUSE
  {"pause", 0, cmd_pause, TEXT_CMDHELP_PAUSE},
#endif

#ifdef INCLUDE_CMD_PROMPT
  {"prompt", 0, cmd_prompt, TEXT_CMDHELP_PROMPT},
#endif

#ifdef INCLUDE_CMD_RMDIR
  {"rd", CMD_SPECIAL_DIR, cmd_rmdir, TEXT_CMDHELP_RD},
#endif

#ifdef INCLUDE_CMD_REM
  {"rem", 0, cmd_rem, TEXT_CMDHELP_REM},
#endif

#ifdef INCLUDE_CMD_RENAME
  {"ren", CMD_SPECIAL_DIR, cmd_rename, TEXT_CMDHELP_REN},
  {"rename", CMD_SPECIAL_DIR, cmd_rename, TEXT_CMDHELP_REN},
#endif

#ifdef INCLUDE_CMD_RMDIR
  {"rmdir", CMD_SPECIAL_DIR, cmd_rmdir, TEXT_CMDHELP_RD},
#endif

#ifdef INCLUDE_CMD_SET
  {"set", 0, cmd_set, TEXT_CMDHELP_SET},
#endif

  {"shift", CMD_BATCHONLY, cmd_shift, TEXT_CMDHELP_SHIFT},

#ifdef INCLUDE_CMD_TIME
  {"time", 0, cmd_time, TEXT_CMDHELP_TIME},
#endif

#ifdef INCLUDE_CMD_TRUENAME
  {"truename", CMD_SPECIAL_DIR, cmd_truename, TEXT_CMDHELP_TRUENAME},
#endif

#ifdef INCLUDE_CMD_TYPE
  {"type", CMD_SPECIAL_DIR, cmd_type, TEXT_CMDHELP_TYPE},
#endif

#ifdef INCLUDE_CMD_VER
  {"ver", 0, cmd_ver, TEXT_CMDHELP_VER},
#endif

#ifdef INCLUDE_CMD_VERIFY
  {"verify", 0, cmd_verify, TEXT_CMDHELP_VERIFY},
#endif

#ifdef INCLUDE_CMD_VOL
        {"vol", 0, cmd_vol, TEXT_CMDHELP_VOL},
#endif

#ifdef INCLUDE_CMD_QUESTION
  {"?", 0, showcmds, TEXT_CMDHELP_QUESTION},
#endif

#ifdef DEBUG
  {"fddebug", 0, cmd_fddebug, TEXT_CMDHELP_FDDEBUG},
#endif

        {NULL, 0, NULL, 0}
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
