
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

enum OnOff {		/* ON/OFF tester */
	OO_Empty = 0,		/* Empty line */
	OO_Null,			/* NULL line */
	OO_On,				/* "ON" */
	OO_Off,				/* "OFF" */
	OO_Other			/* else */
};

#define BREAK_BATCHFILE 1
#define BREAK_ENDOFBATCHFILES 2
#define BREAK_INPUT 3
#define BREAK_IGNORE 4
#define BREAK_FORCMD 5

#define cbreak chkCBreak(0)

#define MAXARGS 20
#define MAXLEN  256
#define exist dfnstat


int cgetchar(void);
int vcgetchar(void);
int vcgetcstr(const char *const legalCh);
int chkCBreak(int mode);
void beep(void);
void beep_low(void);
char *comFile(void);
char *comResFile(void);
#define comRespawnFile comResFile
char *comPathFile(const char * fnam);
void dispCount(int cnt, const char * const zero, const char * const one
 , const char * const multiple);
int drvNum(int drive);
char *cwd(int drive);
int changeDrive(int drive);
char *textlineEnd(const char * const buf, const size_t buflen);
enum OnOff onoffStr(char *line);
size_t farread(void far*buf, size_t length, FILE *f);
unsigned allocSysBlk(const unsigned size, const unsigned mode);
void freeSysBlk(const unsigned segm);

char far *_fstpcpy(char far *dst, const char far *src);

/************* Imported from LH.ASM        */

int dosGetUMBLinkState(void);
void dosSetUMBLinkState(int newState);
int dosGetAllocStrategy(void);
void dosSetAllocStrategy(int newState);
word GetFirstMCB(void);



char *curTime(void);
char *curDateLong(void);

#endif
