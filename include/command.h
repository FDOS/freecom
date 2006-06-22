/* COMMAND.H
 * header file for the modules in COMMAND.COM
 */

#ifndef FREECOM_COMMAND_H
#define FREECOM_COMMAND_H

#include <dos.h>
#include <stdio.h>
#include <fmemory.h>
#include "../include/misc.h"

#define MAX_INTERNAL_COMMAND_SIZE 256
#define MAX_EXTERNAL_COMMAND_SIZE 125
	/* The maximal external command line is:
		  128: overall space for the command line)
		- 1: Pascal string length byte
		- 1: '\r' (at the end)
		- 1: '\0' (at the very end)
		= 125
	*/

enum InternalErrorCodes {
	E_None = 0,
	E_Useage = 1,
	E_Other = 2,
	E_CBreak = 3,
	E_NoMem,
	E_CorruptMemory,
	E_NoOption,
	E_Exit,
	E_Ignore,			/* Error that can be ignored */
	E_Empty,
	E_Syntax,
	E_Range,				/* Numbers out of range */
	E_NoItems,
	E_Help,		/* Help screen */
	E_User		/* MUST be the last one */
};

#define PROMPTVAR "PROMPT"
#define PATHVAR "PATH"

#define EnvSeg (*(unsigned far*)MK_FP(_psp, 0x2c))
#define OwnerPSP (*(unsigned far *)MK_FP(_psp, 0x16))

extern const char shellver[];
extern const char shelldate[];
extern const char shellname[];

#define FINDFIRST(path,attrib,ffblk) findfirst(path,attrib,ffblk)
#define FINDNEXT(ffblk)  findnext(ffblk)
#ifdef FEATURE_LONG_FILENAMES
#define FINDSTOP(ffblk) lfnfindclose(ffblk)
#else
#define FINDSTOP(ffblk)
#endif
#ifndef FA_NORMAL
#define FA_NORMAL 0
#endif

/* Useage:
	FALSE: no, false etc.
	TRUE: yes, OK, etc.
	ERROR: only used in tri-state flags, in binary flags same as TRUE
*/
enum
{
  FALSE, TRUE, ERROR
};

/* prototypes for INIT.C */
extern void interrupt dummy_criter_handler();
extern void interrupt cbreak_handler();
/* extern void initCBreak(void);*/

/* prototypes for COMMAND.C */
extern int interactive_command;
extern int persistentMSGs;
extern int far CBreakCounter;
#define ctrlBreak CBreakCounter
/* extern int ctrlBreak;*/
extern int exitflag;
extern unsigned int echo;       /* The echo flag */
extern int tracemode;                   /* debug script? */
extern int autofail;
#ifdef FEATURE_XMS_SWAP
extern byte far canexit;
#else
extern int canexit;
#endif
extern int inInit;
extern int errorlevel;
extern int isSwapFile;
extern int forceLow;
extern unsigned far *maxx;
extern unsigned char far *maxy;
extern char *ComPath;            /* absolute filename of COMMAND shell */
extern int oldinfd, oldoutfd;

void fatal_error(char *);
int is_delim(int);
void execute(char *, char *);
void command(char *);
void parsecommandline(char *, int);
int initialize(void);
void short_version(void);
int process_input(int xflg, char *cmdline);
/* void perform_exec_result(int rc); */
int expandEnvVars(char *ip, char * const line);

/* prototypes for INTERNAL.C */
int cmd_break(char *);
int cmd_chdir(char *);
int cmd_chcp(char *);
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
int cmd_lfnfor(char *);
int cmd_loadfix(char *);
int cmd_loadhigh(char *);
int cmd_memory(char *);
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
int force_exit(char *);
int cmd_pushd(char *);          /*DLP 06/01/2000 */
int cmd_popd(char *);           /*DLP 06/01/2000 */
int cmd_dirs(char *);           /*DLP 06/01/2000 */
int cmd_which(char *);
void history(int, char *);      /* prototype for the command-line history */
void complete_filename(char *str, unsigned charcount);
int show_completion_matches(char *str, unsigned charcount);


/* prototypes from PROMPT.C */
void printprompt(void);

/* prototypes for ALIAS.C */
void aliasexpand(char *, int);
int aliasswapout(void);
int aliasswapin(void);
int cmd_alias(char *);

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
extern struct CMD internalCommands[];

/* New procs in BATCH.C */

char *readbatchline(int *, char *, int);
/* char *find_arg(int); */

/* The MSDOS Batch Commands [MS-DOS 5.0 User's Guide and Reference p359] */
int cmd_call(char *);
int cmd_echo(char *);
int cmd_for(char *);
int cmd_for_hackery(char *);
int cmd_goto(char *);
int cmd_if(char *);
int cmd_pause(char *);
int cmd_shift(char *);

int cmd_beep(char *);

int get_redirection(char *, char **, char **, int *);

int batch(char *, char *, char *);

#endif
