/* $Id$
 * $Locker$	$Name$	$State$

 Declaration of the subsystem environ

ob(ject): 
su(bsystem): environ
ty(pe): 
sh(ort description): Type of the walkfunction passed to \fct{env_forAll}
lo(ng description): 
pr(erequistes): 
va(lue): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

 * $Log$
 * Revision 1.3  2000/07/09 22:19:22  skaus
 * + Support for international strings without recompiling
 * + Useage of TC++1
 *
 * Revision 2.11  2000/03/31 09:09:32  ska
 * add: DBG_CLOSELOG, suppl_l_openmode
 * add: SYSLOG_ALWAYS_FLUSHLOG
 * add: fcommit(), Fcommit(), Fflush(), commit()
 * add: suppl_log_flush(), DBG_FLUSHLOG, DBG_ALWAYS_FLUSHLOG
 * fix: dfnsearch(): DBG_ARGUMENT()
 * chg: F[gs]etpos() -> true replacements of f[gs]etpos(); removed
 * 	Fp[gs]etpos(); added FF[gs]etpos() using a non-pointer argument
 * bugfix: secure string functions: memory functions don't accept length == 0
 * add: MKSTRFCT.PL: generate DOC\SSTR.FCT
 * fix: dfnsplit(): a pathname without path components returns the root
 * 	directory as path portion
 * add: dfnsplit(): debug output of return values of found drive/path/name/ext
 * fix: dfnsqueeze(): DBG_ENTER() contains wrong names
 * fix: dfnsplit(): chkHeap in drive spec detection routine breaks if/else
 * chg: moved NLS-depended stuff from DFN.H into NLS_F.H
 * add: integer SUPPL error codes -- all functions returning (int) error codes
 * 	return unique codes used throughout all SUPPL, see ERRCODES.H
 *
 * Revision 2.10  1999/12/13 02:23:41  ska
 * add: debug subsystem
 * bugfix: Fposcmp(): If abs(pos1 - pos2) > 32767, the return value is random
 * add: strend() --> returns the address of the NUL byte of a string
 * chg: StrAppend() --> StrAppQStr()
 * add: StrQuote(), StrUnquote()
 * bugfix: Strspn(): if any parameter is NULL, returns NULL now
 * bugfix: MemiCmp(): returned wrong sign
 * bugfix: dfnstat(): Win95 LFN's entry never or invalidly detected
 * add: dfndelim() & dfndelim2()
 * chg: environ.h: mcb_toenv() --> macro mcb_env()
 * fix: cfg_osen.c: cfg_ostkEnum(): If output stack empty --> random return value
 * fix: strnum(): '0x' prefix detection fails
 * chg: long*(): Except longcmp() all functions are void; for portable subsys
 * bugfix: Erealloc(): if len == 0, the program is terminated erroreously
 * add: Fcopyi() & Fcopyl() & Fcopybuf()
 * chg: Fcopyto(): 'topos' must contain a valid position returned by Fgetpos()
 * add: supplio.h: Fpos2dword() & Fppos2dword(): Extract (dword) pos from(fpos_t)
 * chg: env_newsize() renamed into env_setsize()
 * bugfix: env_check(): accepts no segm==0, as it is standard
 * sub: dynstr.h: STR_SAVE_MODES, not used anymore (StrSaveTokens() removed)
 * chg: dynstr.h: STR_SAVED_TOKENS is its own type now
 * bugfix: StrTok*(): if(st) effects first assignment only
 * bugfix: StrTail(): string is strdup()'ed two times
 * bugfix: _fStriCmp(): temporary storage into (char) is unportable
 * bugfix: StriCmp(): sign is inverted
 * bugfix: StriCmp(): accepts no NULL parameters
 * add: DFN_LFN to dfn.h and dfnstat()
 * bugfix: UNC-aware dfnmatch2(): function name wrong
 * sub: removed "Target compilers" note everywhere
 * fix: dfnstat(): returns DFN_DIRECTORY on "X:" and "X:\"
 * chg: No SUPPL function will invoke openlog() automatically
 *
 * Revision 2.9  1998/12/08 04:22:37  ska
 * fix: type conflict appVar() <-> insVar
 *
 * Revision 2.8  1998/12/08 04:04:11  ska
 * add: dpeekc(), dpeekb(), dpeekw()
 * add: Eresize()
 * chg: env_change() to use env_ovrVarOffset() --> replaced variables
 * 	remain at the position they were before
 * add: env_ovrVarOffset(), env_insVarOffset(), env_appVar()
 * sub: env_addVar() [replaced by env_appVar()]
 * bugfix: DOSalloc(): add FreeDOS compliant setting of flags prior call DOS
 *
 * Revision 2.7  1998/12/04 06:00:44  ska
 * add: fpos_t/fsetpos()/fgetpos() to PAC
 * bugfix: DOSalloc(): If called with length == 0, the allocation
 * 	strategy is not resetted
 * bugfix: env_newsize(): Grow block, if fails completely, the old environment
 * 	was destroyed unneceesaryly
 * add: getbootdisk()
 * add: Ftmpfile(), Ftmpnam(), Etmpfile()
 * add: dfnstat(), dfnmktmp()
 * add: dfnwrdir(), dfnstat(), dfnmkfile()
 * fix/chg: StrTrim(): returns "s" if realloc() fails
 * fix: _getdcwd(): removed Carry()
 * bugfix: dfnsqueeze(): In non-UNC mode, there was nothing actually
 * 	sequeezed, except the slashes were flipped
 * add: env_fetch(): DOS-ish getenv()
 * add: _fdupstr(): Duplicates far string into local heap
 *
 * Revision 2.6  1998/07/20 08:42:46  ska
 * Release Version 2.4:
 *
 * bugfix: getopt.c: Micro-C complains about comment in #include
 * add: dfnmatch(): check if a filename matches a p‘ttern, DOS style
 * add: dfnmatch2(): check if a filename matches a p‘ttern, Win32 style
 * ren: *MemCmp() --> *MemiCmp()
 * bugfix: env_resize(): growing/shrinking swapped
 * bugfix: env_resize() and assoc. fail on length == 0 or non-existent
 * 	environment segments
 *
 * Revision 2.5  1998/01/14 02:45:07  ska
 * fix: dossize.c: In Micro-C mode the INT 21h was called with random values
 * chg: env_nullStrings(): Eliminated the Missing Prototype warning
 * fix: env_sub.c: segm must >>NOT<< be zero :)
 *
 * Revision 2.4  1996/12/10 08:13:26  ska
 * Release Version 2.2:
 *
 * add: env_nullStrings()
 * fix: env_shell(): using MCB number instead of PSP
 *
 * Revision 2.3  1996/12/02 03:28:30  ska
 * add: StrAppend(), StrAppChr()
 * fix: env_replace(): the 3rd argument as length with native CREATE mode
 * chg: DOSalloc(): update the name field of the MCB to the caller's one
 * add: StrStrip(), StrChar()
 * add: toFUpper(), MemFCmp(), _fMemFCmp()
 * add: dfnpath(), dfntruename()
 * chg: toUpper(): retreive the upcase table only once
 * add: _fStrCmp(), _fMemCmp(), StrCmp(),& MemCmp()
 * add: toUpper()
 * add: env_matchVar()
 * add: env_dupvar() & dupvar()
 * chg: env_switch() -> env_replace()
 *
 * Revision 2.2  1996/11/17 06:50:27  ska
 * chg: env_strinsert & env_strput: more efficient when appending a new string
 * fix: env_strcpy: stringnr must be < env_strings()
 *
 * Revision 2.1  1996/11/08 03:18:16  ska
 * chg: Makefile: automatically detect, when switching compiler or mem model etc.
 * fix: mcb_walk: evaluating _FLAGS after destroying them by a compare
 * add: env_noSpace: check if environment can hold another variable
 * fix: env_change: don't delete the variable if environment too small
 * fix: env_strput: check env size, ¡f a new string is appended
 * chg: environ.h: prototype: putenv(char *var) to conform to DOS quasi-standard
 *
 * Revision 2.0  1996/08/01 08:22:35  ska
 * Release Version 2
 *
 * Target compilers: Micro-C, Borland C v2.0, v3.1, v4.52
 *
 * Revision 1.4  1996/07/30 05:15:53  ska
 * fix: revised: segm == 0 on all environment functions
 * add: the environment string functions
 * fix: getoptg(): last option preserves the boolean state flag if MARKEMPTY is set
 *
 * Revision 1.3  1996/07/29 06:53:45  ska
 * add: DOSalloc(), addu()
 * chg: newenv(), sizenv()
 *
 * Revision 1.2  1996/07/25 06:34:48  ska
 * fix: (signed) cast bug in suppl.h
 *
 * Revision 1.1  1996/05/19 03:43:03  ska
 * Initial revision
 *
*/

#ifndef __ENVIRON_H
#define __ENVIRON_H

#include <portable.h>

#ifdef _MICROC_
#define ENV_WALKFUNC int *
#else
typedef int (*ENV_WALKFUNC)(void *, unsigned, unsigned);
#endif

#ifndef _MICROC_
/* In order to use these macros, <dos.h> must be #include'd */
#define env_glbSeg peekw(_psp, 0x2c)	/* return the current env seg addr */
#define env_setGlbSeg(a) pokew(_psp, 0x2c, (a))	/* set the current env seg addr */
#else
#define env_glbSeg env_glbSeg_()
#define env_setGlbSeg env_setGlbSeg_
#endif

extern unsigned env_resizeCtrl;
/* Standard settings:
	deny shrinking & moving, use UMBs, best fit */
#define ENV_SHRINKBELOW 1		/* resize below last used byte (will destroy
									environment, is necessary to delete env) */
#define ENV_ALLOWMOVE	2		/* re-allocate environment segment if grow
									fails */
#define ENV_USEUMB		4		/* when env moves, search in UMBs first */
#define ENV_FIRSTFIT	8		/* when env moves, use allocation stragegy First Fit */
#define ENV_LASTFIT		16		/* when env moves, use allocation stragegy Last Fit */
			/* If both ENV_FIRSTFIT & ENV_LASTFIT are set, behaviour is undefined */
			/* If none of the above are set, Best Fit is used */

/* Defines for the env_replace() function: */
#define ENV_DELETE		1		/* free old environment segment */
#define ENV_COPY		2		/* copy old environment into new
									segment, may destroy the environment, if
									new seg is smaller than old one. */
#define ENV_CLEAR		4		/* initialize new environment, so it does not
									contain neither variables nor strings */
#define ENV_CREATE		8		/* create a new environment */
#define ENV_FREECOUNT	16		/* minimum amount of unused bytes specified */
#define ENV_LIMITED		32		/* create smallest possible enviroment */


/*!!!!!!!!!
	All these functions replace segm by the current used environment if
	it is zero. The environment must be initialized with the environement
	variable area and the string area (in the simpliest case, three byes 0).
	!!!!!!!!!*/

#ifdef _MICROC_
unsigned env_glbSeg_(void);
void env_setGlbSeg_(const unsigned segm);
#endif

word env_create(const unsigned length);
void env_delete(const word segm);
int cpyenv(const char * const var, char * const dst, const unsigned length);
int env_get(word segm, const char * const var, char * const dst, const unsigned length);
char *dupvar(const char * const var);
char *env_dupvar(word segm, const char * const var);
char *env_fetch(word segm, const char * const var);
int putenv(const char *var);
#define chgenv(name,value) env_change(0,name,value)
int env_change(unsigned segm, const char * const var, const char * const value);
int env_ovrVarOffset(word segm, word ofs, const char *varname
 , const char * const value);
int env_insVarOffset(word segm, word ofs, const char * const varname
	, const char * const value);

#ifndef NO_ENV_REPLACE_PROTOTYPE
#ifdef _MICROC_
register word env_replace(word env /*, int mode, word segm/length*/);
#else
word env_replace(word env, int mode, ... /*word segm/length*/);
#endif
#endif

int env_matchVar(word segm, char * const var);
int env_findAnyVar(word segm, const char * const var, word *ecase, word *icase);

int env_noSpace(word segm, const char * const name, const char * const value, const word ign);
word env_resize(word segm, int delta);
word env_setsize(word segm, unsigned length);
#define env_newsize(s,l)	env_setsize((s), (l))
int env_forAll(unsigned env, ENV_WALKFUNC fct, void *arg);
unsigned env_master(void);
unsigned env_shell(void);

/*********
	Environment String handling functions
	*********/

int env_nullStrings(word segm);

/* int env_strings(const word segm); */
#define env_strings(segm)	env_strcounter((segm), 0)
int env_strcounter(word segm, int diff);
word env_string(const word segm, int stringNr);
int env_strcpy(word segm, char * const buf, const int len, const int stringNr);
char *env_strdup(word segm, const int stringNr);
int env_strput(word segm, const char * const buf, const int stringNr);
int env_strinsert(word segm, const char * const buf, const int stringNr);

/*********
	DO NOT PASS ZERO INTO THESE FUNCTIONS!!!
	*********/


unsigned env_findVar(const unsigned segm, const char * const var);
void env_subVarOffset(const unsigned segm, const unsigned offset);
int env_appVar(word segm, const char * const name, const char * const value);

int env_check(const unsigned segm);
/*	Check the environemnt, the return value:
 		0:	environment OK
 		1:	no environment at all (envseg is NULL)
 		2:	PSP corrupt (envseg points to an invalid memory block)
 		3:	variable's space corrupted
 		4:	no string table
 		5:	string table corrupted or no string table at all
*/

unsigned env_firstFree(const unsigned segm);
unsigned env_endOfVars(const unsigned segm);
unsigned env_freeCount(const unsigned segm);
unsigned env_varlen(const unsigned segm, const unsigned offs);

#endif
