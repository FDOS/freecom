/* COMMAND.H
 * header file for the modules in COMMAND.COM
 * Tim Norman
 * 7-15-95
 *
 * 06/29/98 (Rob Lake)
 *  - Moved error messages in here
 *
 * 07/12/98 (Rob Lake)
 *  - Moved more error messages here
 *
 * 30-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - Added compile date to version
 *
 * 12-Aug-1998 ska
 * - added: prototypes getEnv(), putEnv(), chgEnv(), chgEnv1()
 *    see environ.c for more details
 *
 * 1999/07/02 ska
 * chg: implementation of exist() --> dfnstat()
 *
 * 1999/07/06 ska
 * add: Exit codes for certain functions E_*
 * add: new error functions error_*()
 * add: persistent message flag
 *
 * 2000/01/15 ska
 * add: prototypes of error functions for CTTY implementation
 * add: declaration of oldinfd & oldoutfd; for CTTY, too
 *
 * 2000/06/22 ska
 *	add: DIRS/PUSHD/POPD patch D. Lucas Parker
 *	add: CDD
 *	chg: shellver & shellname: from pointer --> const[]
 *	add: enum OnOff, onoffStr()
 *
 * 2000/07/05 Ron Cemer
 * fix: TC++1 compatibly: FA_NORMAL macro
 *
 * 2000/07/13 ska
 * chg: Moved into MISC.H: OnOff, BREAK_* macros
 */

#include <dos.h>
#include "misc.h"
#include "context.h"

#define MAX_INTERNAL_COMMAND_SIZE 256
#define MAX_EXTERNAL_COMMAND_SIZE 128

#define CTL_C 3

enum InternalErrorCodes {
	E_None = 0,
	E_Useage = 1,
	E_Other = 2,
	E_CBreak = 3,
	E_NoMem,
	E_NoOption,
	E_Exit,
	E_Ignore			/* Error that can be ignored */
};

#define EnvSeg (*(unsigned far*)MK_FP(_psp, 0x2c))
#define OwnerPSP (*(unsigned far *)MK_FP(_psp, 0x16))

extern const char shellver[];
extern const char shellname[];

#define FINDFIRST(path,attrib,ffblk) findfirst(path,attrib,ffblk)
#define FINDNEXT(ffblk)  findnext(ffblk)
#define FINDSTOP(ffblk)
#ifndef FA_NORMAL
#define FA_NORMAL 0
#endif

enum
{
  FALSE, TRUE
};

/* prototypes for INIT.C */

/* prototypes for COMMAND.C */
extern int ctrlBreak;
extern int exitflag;
extern int errorlevel;
extern int forceLow;
extern unsigned far *maxx;
extern unsigned char far *maxy;
extern char *ComPath;            /* absolute filename of COMMAND shell */
extern int oldinfd, oldoutfd;

void fatal_error(char *);
int is_delim(int);
void execute(char *, char *);
void command(char *);
void parsecommandline(char *);
int initialize(void);
void short_version(void);
int process_input(int xflg, char *cmdline);

/* prototypes for INTERNAL.C */
int cmd_break(char *);
int cmd_chdir(char *);
int cmd_cdd(char *);
int cmd_cls(char *);
int cmd_copy(char *);
int cmd_ctty(char *);
int cmd_date(char *);
int cmd_del(char *);
int cmd_dir(char *);
int cmd_doskey(char *);
int cmd_fddebug(char *);
int cmd_history(char *);
int cmd_loadfix(char *);
int cmd_loadhigh(char *);
int cmd_mkdir(char *);
int cmd_path(char *);
int cmd_prompt(char *);
int cmd_rem(char *);
int cmd_rename(char *);
int cmd_rmdir(char *);
int cmd_set(char *);
int cmd_time(char *);
int cmd_truename(char *);
int cmd_type(char *);
int cmd_ver(char *);
int cmd_verify(char *);
int cmd_vol(char *);
int internal_exit(char *);
int cmd_pushd(char *);          /*DLP 06/01/2000 */
int cmd_popd(char *);           /*DLP 06/01/2000 */
int cmd_dirs(char *);           /*DLP 06/01/2000 */

/* prototypes for ENVIRON.C */
char *getEnv(char *);
int putEnv(char[]);
int chgEnv(const char[], const char[]);
int chgEnv1(const char[], const char[]);

/* prototypes of TMPNAM.C */
char *tmpfn(void);
char *mktempfile(char *path, char *ext);
void killtmpfn(char *);

/* prototypes from PROMPT.C */
void printprompt(void);

/* prototypes for CMDINPUT.C */
void readcommand(char *, int);

/* prototypes for BATCH.C */
/* int batch(char *, char *); - HBP */
int _getch(void);

/* prototypes for WHERE.C */
char *find_which(char *);

/* prototypes for EXEC.C */
int exec(const char *, char *, const unsigned);

/* prototypes for REDIR.C */

/* prototypes for ALIAS.C */
void aliasexpand(char *, int);
int aliasswapout(void);
int aliasswapin(void);
int cmd_alias(char *);

/* Prototypes for ERR_HAND.C */
#if 0
void init_error_handler(void);
void printstring(char *);
char *get_err(unsigned);
void interrupt far dos_critical_error(unsigned, unsigned, unsigned, unsigned,
                       unsigned, unsigned, unsigned, unsigned, unsigned);
#endif

/* Prototypes for MISC.C */
/* Moved to MISC.H */

/* Prototypes for ERROR.C */
void error_no_pipe(void);
void error_bad_command(void);
void error_invalid_drive(void);
void error_req_param_missing(void);
void error_sfile_not_found(const char * const);
void error_file_not_found(void);
void error_path_not_found(void);
void error_too_many_parameters(const char * const);
void error_invalid_switch(char);
void error_invalid_lswitch(const char * const);
void error_illformed_option(const char * const);
void error_opt_arg(const char * const);
void error_opt_noarg(const char * const);
void error_invalid_number(const char * const);
void error_out_of_memory(void);
void error_syntax(const char * const);
void error_long_batchline(const char * const fnam, long linenr);
void error_bfile_vanished(const char * const fnam);
void error_bfile_no_such_label(const char * const fnam, const char * const line);
void error_env(int, const char * const);
void error_env_var(const char * const);
void error_filename_too_long(const char * const fname);
void error_command_too_long(void);
void error_line_too_long(void);
void error_env_var_not_found(const char * const var);
void error_open_file(const char * const fnam);
void error_read_file(const char * const fnam);
void error_write_file(const char * const fnam);
void error_copy(void);
void error_selfcopy(const char * const fnam);
void error_nothing_to_do(void);
void error_tempfile(void);
void error_close_quote(int);
void error_leading_plus(void);
void error_trailing_plus(void);
void error_init_fully_qualified(const char * const);
void error_corrupt_command_line(void);
void error_quoted_c_k(void);
void error_ctty_excluded(void);
void error_l_notimplemented(void);
void error_u_notimplemented(void);
void error_save_session(void);
void error_restore_session(void);
void error_no_rw_device(const char * const);
void error_ctty_dup(const char * const);
void error_no_cwd(int drive);
void dispCopy(const char src[], const char dst[], int append);
void msg_pause(void);

#define D_ON         "on"
#define D_OFF        "off"

/* Flags used in CMDTABLE */

/* CMD_SPECIAL_* modify the default behaviour, which delimiters are
	ignored and which are passed into the internal command.
	ALL --> everything is passed into the cmd [ECHO only]
	DIR --> passed: "\\/", ignored: ",;="
	without --> passed: "/", ignored: ",;="
*/
#define CMD_SPECIAL_ALL 1		/* pass unmodified line into cmd */
#define CMD_SPECIAL_DIR	2		/* pass directory specific chars into cmd */
#define CMD_BATCHONLY   4

struct CMD
{
  char *name;
  int flags;
  int (*func) (char *);
  unsigned help_id;
};

/* New procs in BATCH.C */

char *readbatchline(int *, char *, int);
char *find_arg(int);

/* The MSDOS Batch Commands [MS-DOS 5.0 User's Guide and Reference p359] */
int cmd_call(char *);
int cmd_echo(char *);
int cmd_for(char *);
int cmd_goto(char *);
int cmd_if(char *);
int cmd_pause(char *);
int cmd_shift(char *);

int cmd_beep(char *);

int get_redirection(char *, char **, char **, int *);

int batch(char *, char *, char *);

void beep(void);
void beep_low(void);

/* functions to handle external strings */
void displayString(unsigned id,...);
void unloadMsgs(void);
char *fetchString(unsigned id);
char *getMessage(unsigned id);
extern int persistentMSGs;

int showcmds(char *);

/* parse numbers */
char *parsenum(char *s, int maxCnt, int *cnt, int nums[]);
