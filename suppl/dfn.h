/* $Id$
   $Locker$	$Name$	$State$

	Declarations for the DOS filename functions.

   $Log$
   Revision 1.3  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 1.11  2000/03/31 09:09:32  ska
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

   Revision 1.10  1999/12/13 02:23:41  ska
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

   Revision 1.9  1999/07/02 05:16:11  ska
   bugfix: DFN_LABEL wrong; DFN_SYSTEM missing
   fix: cfgASBoolean: bad function name
   fix: cntry.h: 'char' members could be treated signed
   add: syslogo(), syslogv()
   add: SUPPL_LOG_MEMORY & *alloc/free/strdup replacements
   add: SUPPLDBG.H	as last #include into all *.C files
   fix: openlog() prevents recursive calls
   fix: syslog_printv() accepts syslog_logfile == NULL
   chg: syslog_openmode --> []

   Revision 1.8  1999/04/13 00:11:50  ska
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

   Revision 1.7  1999/01/18 05:32:29  ska
   add: INI file writing functions
   fix: dfnstat() uses wrong register for result
   add: dfnfreplace(), dfnbakname()

   Revision 1.6  1998/12/04 07:30:36  ska
   chg: toupperx.c: to use nlsInfo(); added toLower()
   add: cntry.?: nlsInfo(): NLS information provider

   Revision 1.5  1998/12/04 06:00:44  ska
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

   Revision 1.4  1998/07/20 08:42:46  ska
   Release Version 2.4:

   bugfix: getopt.c: Micro-C complains about comment in #include
   add: dfnmatch(): check if a filename matches a p‘ttern, DOS style
   add: dfnmatch2(): check if a filename matches a p‘ttern, Win32 style
   ren: *MemCmp() --> *MemiCmp()
   bugfix: env_resize(): growing/shrinking swapped
   bugfix: env_resize() and assoc. fail on length == 0 or non-existent
   	environment segments

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

#ifndef __DFN_H
#define __DFN_H

#include <supl_def.h>
#ifndef __PORTABLE_H
#include <portable.h>
#endif
#include <nls_f.h>
#ifdef _TC_LATER_
#include <direct.h>
#endif

/* To support UNC paths, all functions are macro'ed from
	dfn*()   --->     dfnu*()
*/
#ifdef SUPPORT_UNC_PATH
#define dfnmerge dfnumerge
#define dfnsplit dfnusplit
#define dfnexpand dfnuexpand
#define dfnsqueeze dfnusqueeze
#define dfnmatch dfnumatch
#endif

#define DFN_FILE		0x4000
#define DFN_LFN			0x2000
#define DFN_DIRECTORY	0x0010
#define DFN_LABEL		0x0008
#define DFN_WRITE		0x0001
#define DFN_HIDDEN		0x0002
#define DFN_SYSTEM		0x0004
/* Derived attributes */
#define DFN_READ		DFN_FILE	/* every file can be read */

char *dfnexpand(const char * const fname, char * const path);
char *dfnmerge(char *fname, const char * const dr, const char * const path
 , const char * const name, const char * const ext);
int dfnsplit(char * const fname, char ** const dr, char ** const path, char ** const name, char ** const ext);
char *dfnsqueeze(const char * const fname);
char *dfnpath(int drive);
char *dfntruename(const char * const fn);
char *dfnsearch(char * const fname, char *path, char *ext);
int dfnmatch(char *fnam, char *pattern);
int isUNCpath(const char * const fname);
char *UNCpath(const char * const fname);
int dfnwrdir(const char * const fnam);
int dfnstat(const char * const fnam);
int dfnmkfile(const char * const fnam);
void dfn_tmpnam(char * const fnam);
char *dfnmktmp(const char * const path, const char * const ext);
char *dfnfilename(const char * const fnam);
char *dfnbakname(const char * const fnam);
int dfnfreplace(const char * const newname, const char * const oldname);
int dfndelim(const int ch);
int dfndelim2(const int ch);

#endif
