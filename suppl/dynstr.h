/* $Id$
   $Locker$	$Name$	$State$

	Dynamic string functions.

ob(ject): dynstr
su(bsystem): dynstr
ty(pe): 
sh(ort description): Basic data type to represent a dynamic string
lo(ng description): 
pr(erequistes): 
va(lue): a (char*) pointer, must be initialized prior use
re(lated to): 
se(condary subsystems): 
in(itialized by): user
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): STR_SAVED_TOKENS
su(bsystem): dynstr
ty(pe): 
sh(ort description): data type for reentrant \tok{strtok()} function
lo(ng description): This data type is used to save to and restore from
	reentrant information of the function \tok{StrTokenize()}.
pr(erequistes): 
va(lue): Blackbox type used by \tok{StrTokSave()} and \tok{StrTokRestore()}
	only.
re(lated to): StrTokenize StrTokSave StrTokRestore
se(condary subsystems): 
in(itialized by): StrTokSave
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): STR_QUOTES
su(bsystem): dynstr
ty(pe): 
sh(ort description): data type for quote-related functions
lo(ng description): This structure consists of members describing
	what quotes and character to be quoted the quote-related functions
	shall be aware of. The members enumerate the word delimiters,
	characters to be quoted, single character quotes, pairily used
	quotes and unpairily used quotes.
pr(erequistes): 
va(lue): 
re(lated to): StrQuote StrUnquote StrAppQStr
se(condary subsystems): 
in(itialized by): user
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): StrTokStop
su(bsystem): dynstr
ty(pe): 
sh(ort description): End a \fct{StrTokenize()} inspection
lo(ng description): Instructres the function \fct{StrTokenize} to
	flush all internal values in order to return \fct{NULL} next time
	it is called and, therefore, definitely end a tokenizer loop \em{and}
	ensures that the string has been restored to its original value.
pr(erequistes): 
va(lue): none
re(lated to): StrTokenize
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

   $Log$
   Revision 1.3  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 1.14  2000/03/31 09:09:32  ska
   add: DBG_CLOSELOG, suppl_l_openmode
   add: SYSLOG_ALWAYS_FLUSHLOG
   add: fcommit(), Fcommit(), Fflush(), commit()
   add: suppl_log_flush(), DBG_FLUSHLOG, DBG_ALWAYS_FLUSHLOG
   fix: dfnsearch(): DBG_ARGUMENT()
   chg: F[gs]etpos() -> true replacements of f[gs]etpos(); removed
   	Fp[gs]etpos(); added FF[gs]etpos() using a non-pointer argument
   bugfix: secure string functions: memory functions don't accept length == 0
   add: MKSTRFCT.PL: generate DOC\SSTR.FCT
   fix: dfnsplit(): a pathname without path components returns the root
   	directory as path portion
   add: dfnsplit(): debug output of return values of found drive/path/name/ext
   fix: dfnsqueeze(): DBG_ENTER() contains wrong names
   fix: dfnsplit(): chkHeap in drive spec detection routine breaks if/else
   chg: moved NLS-depended stuff from DFN.H into NLS_F.H
   add: integer SUPPL error codes -- all functions returning (int) error codes
   	return unique codes used throughout all SUPPL, see ERRCODES.H

   Revision 1.13  1999/12/13 02:23:41  ska
   add: debug subsystem
   bugfix: Fposcmp(): If abs(pos1 - pos2) > 32767, the return value is random
   add: strend() --> returns the address of the NUL byte of a string
   chg: StrAppend() --> StrAppQStr()
   add: StrQuote(), StrUnquote()
   bugfix: Strspn(): if any parameter is NULL, returns NULL now
   bugfix: MemiCmp(): returned wrong sign
   bugfix: dfnstat(): Win95 LFN's entry never or invalidly detected
   add: dfndelim() & dfndelim2()
   chg: environ.h: mcb_toenv() --> macro mcb_env()
   fix: cfg_osen.c: cfg_ostkEnum(): If output stack empty --> random return value
   fix: strnum(): '0x' prefix detection fails
   chg: long*(): Except longcmp() all functions are void; for portable subsys
   bugfix: Erealloc(): if len == 0, the program is terminated erroreously
   add: Fcopyi() & Fcopyl() & Fcopybuf()
   chg: Fcopyto(): 'topos' must contain a valid position returned by Fgetpos()
   add: supplio.h: Fpos2dword() & Fppos2dword(): Extract (dword) pos from(fpos_t)
   chg: env_newsize() renamed into env_setsize()
   bugfix: env_check(): accepts no segm==0, as it is standard
   sub: dynstr.h: STR_SAVE_MODES, not used anymore (StrSaveTokens() removed)
   chg: dynstr.h: STR_SAVED_TOKENS is its own type now
   bugfix: StrTok*(): if(st) effects first assignment only
   bugfix: StrTail(): string is strdup()'ed two times
   bugfix: _fStriCmp(): temporary storage into (char) is unportable
   bugfix: StriCmp(): sign is inverted
   bugfix: StriCmp(): accepts no NULL parameters
   add: DFN_LFN to dfn.h and dfnstat()
   bugfix: UNC-aware dfnmatch2(): function name wrong
   sub: removed "Target compilers" note everywhere
   fix: dfnstat(): returns DFN_DIRECTORY on "X:" and "X:\"
   chg: No SUPPL function will invoke openlog() automatically

   Revision 1.12  1999/04/13 00:11:50  ska
   bugfix: dfnexpand(): fname == "" --> fname := "."
   bugfix: dfnsearch(): Searching root instead of '.' if no search path
   add: syslog subsystem
   fix: spelling in comments etc.
   bugfix: not all SUPPORT_UNC_PATH macros correct
   add: INI file random access subsystem
   add: Fmaxbuf()
   add: Fcopy(), Fcopyto()
   add: Fposcpy(), Fposcmp()
   add: memzero(): unportable memory zero'ing function

   Revision 1.11  1999/01/05 01:14:57  ska
   add: Esetsize()

   Revision 1.10  1998/12/08 04:04:11  ska
   add: dpeekc(), dpeekb(), dpeekw()
   add: Eresize()
   chg: env_change() to use env_ovrVarOffset() --> replaced variables
   	remain at the position they were before
   add: env_ovrVarOffset(), env_insVarOffset(), env_appVar()
   sub: env_addVar() [replaced by env_appVar()]
   bugfix: DOSalloc(): add FreeDOS compliant setting of flags prior call DOS

   Revision 1.9  1998/12/04 07:30:36  ska
   chg: toupperx.c: to use nlsInfo(); added toLower()
   add: cntry.?: nlsInfo(): NLS information provider

   Revision 1.8  1998/12/04 06:00:44  ska
   add: fpos_t/fsetpos()/fgetpos() to PAC
   bugfix: DOSalloc(): If called with length == 0, the allocation
   	strategy is not resetted
   bugfix: env_newsize(): Grow block, if fails completely, the old environment
   	was destroyed unneceesaryly
   add: getbootdisk()
   add: Ftmpfile(), Ftmpnam(), Etmpfile()
   add: dfnstat(), dfnmktmp()
   add: dfnwrdir(), dfnstat(), dfnmkfile()
   fix/chg: StrTrim(): returns "s" if realloc() fails
   fix: _getdcwd(): removed Carry()
   bugfix: dfnsqueeze(): In non-UNC mode, there was nothing actually
   	sequeezed, except the slashes were flipped
   add: env_fetch(): DOS-ish getenv()
   add: _fdupstr(): Duplicates far string into local heap

   Revision 1.7  1998/11/25 09:33:08  ska
   add: ff*.c/supplio.h: F*(), equal (FILE*) functions for Micro-C & ANSI C
   add: gm_*.c: get memory, wrappers of malloc() and assoc. functions
   chg: getopt.c: to use msgs.h
   add: msgs.h: shared MSGLIB/internal message handling
   add: StrChr(): searches for a character within a string
   add: StrWord(): strips leading & trailing whitespaces and trim string
   bugfix: StrTrim(): if realloc() fails, no NULL was returned.

   Revision 1.6  1998/08/17 07:19:21  ska
   bugfix: dfnsearch(): "c:\path\fnam" doesn't work
   chg: splitted StrSaveTokens() into StrTokSave() & StrTokRestore()
   add: StrTokStop(): stop StrTokenize() cycle (next call to it returns NULL)

   Revision 1.5  1998/07/20 08:42:46  ska
   Release Version 2.4:

   bugfix: getopt.c: Micro-C complains about comment in #include
   add: dfnmatch(): check if a filename matches a p‘ttern, DOS style
   add: dfnmatch2(): check if a filename matches a p‘ttern, Win32 style
   ren: *MemCmp() --> *MemiCmp()
   bugfix: env_resize(): growing/shrinking swapped
   bugfix: env_resize() and assoc. fail on length == 0 or non-existent
   	environment segments

   Revision 1.4  1998/01/14 02:45:07  ska
   fix: dossize.c: In Micro-C mode the INT 21h was called with random values
   chg: env_nullStrings(): Eliminated the Missing Prototype warning
   fix: env_sub.c: segm must >>NOT<< be zero :)

   Revision 1.3  1996/12/11 06:22:46  ska
   Release Version 2.2:

   fix: dfnumerge(): double backslash with UNC drive & root path
   add: StrCString(), toxdigit(), isodigit()
   add: UNC path support to the dfn*() functions
   add: StrLeft(), StrRight(), StrMiddle(), StrTail(), StrBeg()

   Revision 1.2  1996/12/05 07:37:57  ska
   add: dfnsearch(), StrTokenize(), StrSaveTokens()
   add: Carry() for Micro-C only
   chg: dfnpath(): use _getdcwd() for Micro-C
   add: _getdcwd() for Micro-C only
   fix: dfnpath(): getdrive() -> get_dir()

   Revision 1.1  1996/12/02 03:28:30  ska
   Initial revision

*/

#ifndef __DYNSTR_H
#define __DYNSTR_H

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>
#include "sstr.h"
#include "nls_c.h"

struct STR_SAVED_TOKENS_ {
	char *str_next_saved_tokens;
	char str_ch_saved_tokens;
};

#ifdef _MICROC_
#define STR_SAVED_TOKENS struct STR_SAVED_TOKENS_
#else
typedef struct STR_SAVED_TOKENS_ STR_SAVED_TOKENS;
#endif

/* Enumerates all quotes a string may be quoted with: */
/* If one member is equal to NULL or "", the particular value
	does not exist. */
struct STR_QUOTES_ {
	const char
		/* Characters that delimit two strings and must be quoted
			if they appear within a string. */
	 *str_delimiters
		/* Characters that must be quoted if they appear within a
			string. */
	,*str_toquote
		/* Characters that quote the immediately next character
			as the backslash '\' does in C. These quotes are
			still "active" within otherwise quoted strings. */
	,*str_singleQuotes
		/* Characters that enclose a quoted string. The same character
			appears before and after the string, just like the '"' in C. */
	,*str_pairyQuotes
		/* Characters that enclose a quoted string. The first character,
			the one on an even offset (first character has offset == 0),
			is the left quote, the second character is the right one.
			Compareble to parenthizes.  */
	,*str_unpairyQuotes
		/* Pairy and unpairy quotes (the left one is checked) that quote
			single quotes, too. */
	,*str_fullQuotes
	;
};
#ifdef _MICROC_
#define STR_QUOTES STR_QUOTES_
#else
typedef struct STR_QUOTES_ STR_QUOTES;
#endif

#ifdef _MICROC_
#define dynstr char *
#else
typedef char * dynstr;
#endif

int StriCmp(const char *s1, const char *s2);
char *StrDup(const char * const s);

#define StrChg(poi,size)	StrChg_(&(poi), (size))
char *StrChg_(char ** const poi, size_t len);

#define StrFree(str) StrRepl_(&(str), NULL)
#define StrFree_(str) StrRepl_((str), NULL)

#define StrRepl(dest,src) StrRepl_(&(dest), (src))
char *StrRepl_(char ** const dest, char * const src);

#ifdef _MICROC_
register char *StrConcat(int argcnt);
register char *EStrConcat(int argcnt);
#else
char *StrConcat(int argcnt, ...);
char *EStrConcat(int argcnt, ...);
#endif

char *StrTrim(char * const s);
char *StrChar(int ch);
char *StrStrip(char * const s, int ch);

#define StrCpy(dest,src) StrCpy_(&(dest), (src))
char *StrCpy_(char ** const dest, const char * const src);
/* Copy (duplicate) the string src[] into dest.
	If src == NULL, this function is equal to StrFree(dest).

	Return:
		NULL: malloc() failed || src == NULL
		else: *dest
*/

#define StrAppDelimStr(dst,src,q)		\
	StrAppDelimStr_(&(dst), (src), (q))
char *StrAppDelimStr_(char ** const dst, const char * const src
 , const STR_QUOTES * const quotes);

#define StrAppStr(dst,src)	StrCat_(&(dst), (src))
#define StrAppStr_	StrCat_
#define StrCat(dest,src) StrCat_(&(dest), (src))
char *StrCat_(char ** const dest, const char * const src);

#define StrAppQStr(dst,src,quote_struct)		\
	StrAppQStr_(&(dst), (src), (quote_struct))
char *StrAppQStr_(char ** const dest, const char * const src
	, const STR_QUOTES * const quotes);

#define StrAppChr(dest,chr) StrAppChr_(&(dest), (chr))
char *StrAppChr_(char ** const dest, char chr);

char *StrQuote(const char * const s, const STR_QUOTES * const quotes);

char *StrUnquote(const char * const s, const STR_QUOTES * const quotes);

int StrUnquoteTokenAppend(const char **src, const STR_QUOTES * const quotes
	, char ** const dst);

int StrUnquoteToken(const char **src, const STR_QUOTES * const quotes
	, char ** const dst);


char *StrTokenize(char *str, const char * const token);
#define StrTokStop() (void)StrTokenize(NULL, NULL)
void StrTokSave(STR_SAVED_TOKENS *st);
void StrTokRestore(STR_SAVED_TOKENS *st);

char *StrLeft(const char * const s, size_t length);
char *StrRight(const char * const s, size_t length);
char *StrMiddle(const char * const s, size_t pos, size_t length);
char *StrTail(const char * const s, unsigned pos);

#ifdef _MICROC_
#define StrBeg strbeg
#else
int StrBeg(const char * const s1, const char * const s2);
#endif		/* !defined(_MICROC_) */

int StrCString(char *str);

char *StrWord(char *s);

char *StrChr(const char * const s, int ch);
size_t StrnLen(const char * const s, size_t len);
char *StrDupe(const char * const beg, const char * const end);

#define StrEnd Strend
//char *StrEnd(char *str);


extern void *Emalloc(size_t len);
extern void *Ecalloc(size_t len);
extern void *Erealloc(void *p, size_t size);
#define Esetsize(poi,size)	Esetsize_(&(void*)(poi), (size))
extern void *Esetsize_(void **poi, size_t size);
#define Eresize(poi,size)	Eresize_(&(void*)(poi), (size))
extern void *Eresize_(void **poi, size_t size);
extern char *Estrdup(const char * const str);
#define Estrchg(var,str) Estrchg_(&(var),(str))
extern char *Estrchg_(char ** const pvar, const char * const str);
extern char *Estrdupe(const char * const start, const char * const end);

#endif
