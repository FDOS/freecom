/* COMMAND.H
 * header file for the modules in COMMAND.COM
 */

#ifndef FREECOM_COMMAND_H
#define FREECOM_COMMAND_H

#include <dos.h>
#include <stdio.h>
#include <fmemory.h>
#include "../include/misc.h"
#include "../include/ierror.h"

/* transformed into constants
	unsigned far *maxx = MK_FP(0x40, 0x4a);
	unsigned char far *maxy = MK_FP(0x40, 0x84);
*/
#define MAX_X (*(unsigned int  far*)MK_FP(0x40, 0x4a))
#define MAX_Y (*(unsigned char far*)MK_FP(0x40, 0x84))

#define MAX_EXTERNAL_COMMAND_SIZE 125
	/* The maximal external command line is:
		  128: overall space for the command line)
		- 1: Pascal string length byte
		- 1: '\r' (at the end)
		- 1: '\0' (at the very end)
		= 125
	*/

#define PROMPTVAR "PROMPT"
#define PATHVAR "PATH"

#define EnvSeg (*(unsigned far*)MK_FP(_psp, 0x2c))
#define OwnerPSP (*(unsigned far *)MK_FP(_psp, 0x16))

extern const char shellver[];
extern const char shellname[];

/* prototypes for INIT.C */
extern void interrupt dummy_criter_handler();
extern void interrupt cbreak_handler();
extern void initCBreak(void);

/* prototypes for COMMAND.C */
extern int ctrlBreak;
extern int autofail;
extern int inInit;
extern int isSwapFile;
extern int forceLow;
extern unsigned far *maxx;
extern unsigned char far *maxy;
extern char *ComPath;            /* absolute filename of COMMAND shell */

void fatal_error(char *);
int is_delim(int);
void execute(char *, char *);
void command(char *);
void parsecommandline(char *);
int initialize(void);
void short_version(char *);
int process_input(int xflg, char *cmdline);
void perform_exec_result(int rc);

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
int cmd_exit(char *);
int cmd_pushd(char *);
int cmd_popd(char *);
int cmd_dirs(char *);
int cmd_which(char *);
void complete_filename(char * const str, const unsigned charcount);
int show_completion_matches(char * const str, const unsigned charcount);
int cmd_icmds(char *);
int cmd_ivar(char *);
int cmd_fd(char *);
int cmd_arg(char *);
int cmd_cancel(char *);
int cmd_quit(char *);
int cmd_dispNLS(char *);
int cmd_dispFlags(char *);
int cmd_dispArgs(char *);
int cmd_dispIVars(char *);
int cmd_dispEC(char *);
int cmd_dispStatus(char *);

/* Internal functions */
char *fct_tempfile(char *);
char *fct_ivar(char *);
char *fct_verbatim(char *);
char *fct_batchname(char *);
char *fct_batchline(char *);
char *fct_argv(char *);


/* prototypes from PROMPT.C */
void printprompt(void);

/* prototypes for ALIAS.C */
char *aliasexpand(char *);
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
#define CMD_SPECIAL_ALL 0x80	/* pass unmodified line into cmd */
#define CMD_SPECIAL_DIR	0x40	/* pass directory specific chars into cmd */
#define CMD_BATCHONLY   0x20

#define CMD_HIDDEN		0x01	/* is command hidden? */
#define CMD_HIDDEN_DEFAULT	(CMD_HIDDEN << 1)	/* is cmd hidden by dflt? */

struct CMD {
  char *name;
  int flags;
  int (*func) (char *);
  unsigned help_id;
};
extern struct CMD internalCommands[];

struct CMD *is_icmd(const char * const);

struct IFCT {
	char *name;
	char *(*func)(char*);
};
extern struct IFCT internalFunctions[];

struct IFCT *is_ifct(const char *);

int is_ivar(const char * const, char ** const);
int is_alias(const char * const);

/* New procs in BATCH.C */

char *find_arg(int);
int setArguments(const char * const line);

/* The MSDOS Batch Commands [MS-DOS 5.0 User's Guide and Reference p359] */
int cmd_call(char *);
int cmd_echo(char *);
int cmd_echoerr(char *);
int cmd_for(char *);
int cmd_for_hackery(const char *);
int cmd_goto(char *);
int cmd_if(char *);
int cmd_pause(char *);
int cmd_shift(char *);

int cmd_beep(char *);

int get_redirection(char *, char *** const, char *** const, char *** const);
char *expEnvVars(char * const param);

int batch(char *, char *, char *);
int qecAction(char *param, FLAG * const flag);

#endif
