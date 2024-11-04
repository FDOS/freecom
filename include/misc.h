/*
 * MISC.C -- Misc. Functions declarations
 *
 * 2000/07/13 ska
 *	started
 */

#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <portable.h>

#ifndef DEBUG
#undef stdin
#undef stdout
#undef stderr
extern FILE *stdin, *stdout, *stderr;
#undef fileno
#define fileno(f) ((int)(f))
#endif

#include "../include/datefunc.h"
#include "../include/timefunc.h"

enum OnOff {		/* ON/OFF tester */
	OO_Empty = 0,		/* Empty line */
	OO_Null,			/* NULL line */
	OO_On,				/* "ON" */
	OO_Off,				/* "OFF" */
	OO_Other			/* else */
};

typedef enum {
	  STRINGS_LOADED
	, STRINGS_ID_MISMATCH
	, STRINGS_SIZE_MISMATCH
	, STRINGS_NOT_FOUND
	, STRINGS_READ_ERROR
	, STRINGS_OUT_OF_MEMORY
} loadStatus;

#define BREAK_BATCHFILE 1
#define BREAK_ENDOFBATCHFILES 2
#define BREAK_INPUT 3
#define BREAK_IGNORE 4
#define BREAK_FORCMD 5

#define cbreak chkCBreak(0)

#define MAXARGS 20
#define MAXLEN  256
#define exist dfnstat

/* transformed into constants
	unsigned far *maxx = MK_FP(0x40, 0x4a);
	unsigned char far *maxy = MK_FP(0x40, 0x84);
*/
#if defined(NEC98)
# define MAX_X (((*(unsigned char far *)MK_FP(0x0000, 0x053c)) & 2)?40:80)
# define MAX_Y (*(unsigned char far *)MK_FP(0x0060, 0x0112))
#elif defined(IBMPC)
# define MAX_X (*(unsigned int  far*)MK_FP(0x40, 0x4a))
# define MAX_Y (*(unsigned char far*)MK_FP(0x40, 0x84) == 0 ? 24 : *(unsigned char far*)MK_FP(0x40, 0x84)) /* when 0040:0084 contains 0, assume 25 rows (CGA...) */

#else
# define MAX_X 80
# define MAX_Y 24
#endif
#define SCREEN_COLS MAX_X
#define SCREEN_ROWS (MAX_Y + 1)

extern FILE *errStream;
#define outStream stdout
extern int swapOnExec, defaultToSwap, swapContext;

void display_errno_fnam_error(const char * const fnam);
void displayExitcode(void);

void outc( char c );
void outs( const char * const s );
int cgetchar(void);
int vcgetchar(void);
int vcgetcstr(const char *const legalCh);
int keypressed(void);
int cgetchar_timed(int secs);
int chkCBreak(int mode);
const char *getCurrenConDev(void);
int setCurrentConDev(const char * dnam);
void beep(void);
void beep_low(void);
char *comFile(void);
char *comPathFile(const char * fnam);
char *comResFile(void);
#define comRespawnFile comResFile
int validResFile(const char * const fnam);
void dispCount(int cnt, unsigned base_id);
int drvNum(int drive);
char *cwd(int drive);
char *abspath(const char * const fnam, const int warn);
char *truepath(const char * const fnam);
int changeDrive(int drive);
int mk_rd_dir(char *param, int (*func) (const char *, int optRecursiveMode, int optQuiet), char *fctname);
void cutBackslash(char * const s);
int cd_dir(char *param, int cdd, const char * const fctname);
enum OnOff onoffStr(char *line);
#if defined(__TURBOC__)
#define sfn_open _open
#define dos_close _close
#else
int sfn_open(const char *pathname, int flags);
#if defined(__GNUC__)
#define stricmp strcasecmp
#define strcmpi strcasecmp
#define strnicmp strncasecmp
#define memicmp strncasecmp
int dos_close(int fd);
#else
#define dos_close _dos_close
#endif
#endif
int dos_read(int fd, void *buf, unsigned int len);
int dos_write(int fd, const void *buf, unsigned int len);
#define sfnfindfirst(path,attrib,ffblk) findfirst(path,attrib,ffblk)
#define sfnfindnext(ffblk)  findnext(ffblk)
#ifndef dos_open
#define dos_open sfn_open
#define dos_creat sfn_creat
#define dos_findfirst sfnfindfirst
#define dos_findnext sfnfindnext
#define dos_findclose(ffblk)
#endif
int sfn_creat(const char *pathname, int attr);
int sfn_creatnew(const char *pathname, int attr);
size_t farread(int fd, void far*buf, size_t length);
size_t farwrite(int fd, void far*buf, size_t length);
unsigned allocPermBlk(const unsigned size, const unsigned mode);
unsigned allocSysBlk(const unsigned size, const unsigned mode);
unsigned allocMemBlk(const unsigned size, const unsigned mode);
void freeSysBlk(const unsigned segm);

void myperror(const char *msg);
void myfnsplit(const char *path, char *buf,
               char *drv, char **dir, char **name, char **ext);
void myfnmerge(char *path,
               const char *drive,
               const char *dir,
               const char *fname,
               const char *ext);

char far *_fstpcpy(char far *dst, const char far *src);
void partstrlower(char *str);
void readcommand(char * const str, int maxlen);
void readcommandDOS(char * const str, int maxlen);
void readcommandEnhanced(char * const str, const int maxlen);
int set_readcommandType(int enhanced);
void convert(unsigned long num, unsigned int billions, char * const des);

void goxy(const unsigned char x, const unsigned char y);

void setErrorLevel(int rc);
void execute(char *first, char *rest, int lh_lf);
int exec(const char *, char *, const unsigned);
void restoreSession(void);
void saveSession(void);

int samefile(const char * const f1, const char * const f2);
void fillFnam(char *dest, const char * const pattern, const char * const fnam);
int is_fnchar(const int c);
int is_fnstr(const char * const s);
int is_pathdelim(const int c);
int is_empty(const char *s);
#define is_quote(c)	((c) == '"')
char *skipfnam(const char * const fnam);

unsigned mywherex(void);
unsigned mywherey(void);

/************* Imported from LH.ASM        */

int dosGetUMBLinkState(void);
void dosSetUMBLinkState(int newState);
int dosGetAllocStrategy(void);
void dosSetAllocStrategy(int newState);
word GetFirstMCB(void);



char *curTime(void);
char *curDateLong(void);
int parsedate(const char * s, struct dosdate_t * const datep);
int parsetime(const char * s, struct dostime_t * const timep);

void displayPrompt(const char *pr);
void printprompt(void);
int showcmds(char *rest);
int grabComFilename(const int warn, const char far * const fnam);

void displayString(unsigned id,...);
void displayError(unsigned id,...);
int userprompt(unsigned id,...);
int getPromptString(unsigned id, char ** const chars, char ** const fmt);
#define freePromptString(chars,fmt)	free(chars)
int mapMetakey(const char * const chars, int ch);
char *getMessage(unsigned id);
char *defaultMessage(unsigned id);
char *getString(unsigned id);
void unloadMsgs(void);
unsigned msgSegment(void);

char *tmpfn(void);
char *mktempfile(const char * const path, const char *ext);
void killtmpfn(char * const);

char *find_which(const char * const fname);
int getbreak(void);
void setbreak(const int OnOff);

char *getEnv(char *);
int chgEnv(const char[], const char[]);
int chgEnvRemove(const char[], const char[]);
int chgEnvCase(const int, char[], const char[]);

char *trimsp(char *str);
char *ltrimsp(const char *str);
void rtrimsp(char * const str);
char *parsenum(const char *s, int maxCnt, int *cnt, int nums[]);
char *textlineEnd(const char * const buf, const size_t buflen);

int truncate(int fd);

/*
void aliasprint(void);
void aliaschange(const char * const name, const char * const value);

char *alias_mangleFNam(const char * const name);
word alias_find(char * const tagged_name);
*/


/* wrapper functions for ..\include\context.h
	- may disapeear in the future */
/* history wrapper: negative idx --> from end of buffer;
	currently no non-negative idx used */
int histGet(const int num, char * const str, const unsigned len);
void histSet(const int num, const char * const str);

void lastDirSet(void);
void lastDirGet(char ** const buf);

int appendDisable(void);
void appendRestore(const int state);

void registerCriterRepeatCheckAddr(word far *);
char *critDriveReport(void);
void critEnableRepeatCheck(void);
unsigned critDisableRepeatCheck(void);
void critEndRepCheck(void);

#if defined(IBMPC) || defined(NEC98) || defined(FMR) || defined(IGNORE_ENHANCED_INPUT)
# ifndef IGNORE_ENHANCED_INPUT
/* prf.c */
int putch_int29(int c);
int cputs_int29(const char *s);
#  if 0
#  undef putch
#  undef cputs
#  define putch(c) putch_int29(c)
#  define cputs(s) cputs_int29(s)
#  endif
# endif
/* xtra.c */
int init_mymachine(void);
enum MyMachine {
	MYMACHINE_UNKNOWN = 0,
	MYMACHINE_IBMPC,
	MYMACHINE_NEC98,
	MYMACHINE_FMR
};
extern int mymachine;
void mydelay(unsigned ms);
void mysound(unsigned f);
void mynosound(void);

#endif

#endif
