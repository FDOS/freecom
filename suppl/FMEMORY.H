/* $Id$
   $Locker$	$Name$	$State$

	Far memory handling function.

   $Log$
   Revision 1.1  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 2.4  2000/03/31 09:09:33  ska
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

   Revision 2.3  1998/12/04 06:00:44  ska
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

   Revision 2.2  1998/08/05 09:42:22  ska
   Release Version 2.5:

   chg: getopt() to not depend on msglib
   add: portable.h: getvect(), setvect(), isr
   chg: moving _f*() memory/string function declarations into fmemory.h
   bugfix: env_size.c: env_newsize(): shrinking causes unpredictable results
   add: _fmemcmp(), _fnormalize(), _fmemmove() for PAC
   add: dta.c/suppl.h: setdta()/getdta()
   add: suppl.h/filefind.c: findfirst()/findnext()/ffblk for PAC
   add: _getdcwd.c: Pacific C version
   add: suppl.h: settings for Pacific C, incl. (struct REGPACK)
   chg: all *.c files include INITSUPL.INC as the very first
   	line after the welcome header. This will setup various
   	compiler depend macros (e.g. _PAC_NOCLIB_)
   add: setdisk.c, getdisk.c, osvers.c: for Pacific C
   add: portable.h: various settings: Support for Pacific C v7.51

   Revision 2.1  1996/11/08 03:18:16  ska
   chg: Makefile: automatically detect, when switching compiler or mem model etc.
   fix: mcb_walk: evaluating _FLAGS after destroying them by a compare
   add: env_noSpace: check if environment can hold another variable
   fix: env_change: don't delete the variable if environment too small
   fix: env_strput: check env size, ¡f a new string is appended
   chg: environ.h: prototype: putenv(char *var) to conform to DOS quasi-standard

   Revision 2.0  1996/08/01 08:22:35  ska
   Release Version 2

   Target compilers: Micro-C, Borland C v2.0, v3.1, v4.52

   Revision 1.3  1996/07/29 06:53:45  ska
   add: DOSalloc(), addu()
   chg: newenv(), sizenv()

   Revision 1.2  1996/07/25 06:34:48  ska
   fix: (signed) cast bug in suppl.h

   Revision 1.1  1996/05/19 03:43:03  ska
   Initial revision

*/

#ifndef __FMEMORY_H
#define __FMEMORY_H

#include <SUPL_DEF.H>

#ifdef _MICROC_				/* for use for Micro-C only */

#define MK_FP(seg,ofs) (seg), (ofs)
#define TO_FP(var) get_ds(), (var)
#define _fnormalize(seg,ofs) _fnormalize_(&(seg), &(ofs))

#else	/* !_MICROC_ */

#ifdef _TC_LATER_
#include <string.h>

#else
#if defined(_PAC_NOCLIB_) || defined(_TC_EARLY_)

#ifdef _PAC_NOCLIB_
#include <stdlib.h>
#define _fmemcpy farmemcpy
#define _fmemset farmemset

#else
void _fmemcpy(void far * const dst, const void far * const src, unsigned length);
void _fmemset(void far * const dst, int ch, unsigned length);
#endif

unsigned _fstrlen(const char far * const s);
char far *_fstrchr(const char far * const s, int ch);
void _fmemmove(void far * const dst, const void far * const src, unsigned length);
int _fmemcmp(const void far * const dst, const void far * const src, unsigned length);
int _fmemicmp(const void far * const dst, const void far * const src, unsigned length);
int _fstrcmp(const char far * const dst, const char far * const src);
int _fstricmp(const char far * const dst, const char far * const src);
void _fstrcpy(char far * const dst, const char far * const src);

#endif
#endif		/* _TC_LATER_ */

#if defined(HI_TECH_C) || defined(_TC_EARLY_)
	/* HiTech C has no huge pointers
	 & early Turbo C does ignore the cast */
void far *_fnormalize(void far *poi);
#else
#define _fnormalize(poi) ((void huge *)(poi))
#endif

char *_fdupstr(const char far * const s);
#endif

#endif
