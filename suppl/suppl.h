/* $Id$
   $Locker$	$Name$	$State$

	Declarations for the supplemental functions.

	This header file is part of the "portable" subsystem
	and will be removed, or moved to another purpose.

   $Log$
   Revision 1.2  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 2.13  2000/03/31 09:09:34  ska
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

   Revision 2.12  1999/04/13 00:11:50  ska
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

   Revision 2.11  1998/12/08 04:04:11  ska
   add: dpeekc(), dpeekb(), dpeekw()
   add: Eresize()
   chg: env_change() to use env_ovrVarOffset() --> replaced variables
   	remain at the position they were before
   add: env_ovrVarOffset(), env_insVarOffset(), env_appVar()
   sub: env_addVar() [replaced by env_appVar()]
   bugfix: DOSalloc(): add FreeDOS compliant setting of flags prior call DOS

   Revision 2.10  1998/12/04 07:30:36  ska
   chg: toupperx.c: to use nlsInfo(); added toLower()
   add: cntry.?: nlsInfo(): NLS information provider

   Revision 2.9  1998/12/04 06:00:44  ska
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

   Revision 2.8  1998/11/25 09:33:08  ska
   add: ff*.c/supplio.h: F*(), equal (FILE*) functions for Micro-C & ANSI C
   add: gm_*.c: get memory, wrappers of malloc() and assoc. functions
   chg: getopt.c: to use msgs.h
   add: msgs.h: shared MSGLIB/internal message handling
   add: StrChr(): searches for a character within a string
   add: StrWord(): strips leading & trailing whitespaces and trim string
   bugfix: StrTrim(): if realloc() fails, no NULL was returned.

   Revision 2.7  1998/08/19 07:26:56  ska
   fix: incompatiblies Watcom to other compilers

   Revision 2.6  1998/08/19 06:32:54  ska
   chg: added Watcom C v10.6

   Revision 2.5  1998/08/05 09:42:22  ska
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

   Revision 2.4  1998/01/14 02:45:07  ska
   fix: dossize.c: In Micro-C mode the INT 21h was called with random values
   chg: env_nullStrings(): Eliminated the Missing Prototype warning
   fix: env_sub.c: segm must >>NOT<< be zero :)

   Revision 2.3  1996/12/05 03:56:02  ska
   fix: suppl.h: invokeDOS() struct REGPACK typedef for Micro-C

   Revision 2.2  1996/12/02 03:28:30  ska
   add: StrAppend(), StrAppChr()
   fix: env_replace(): the 3rd argument as length with native CREATE mode
   chg: DOSalloc(): update the name field of the MCB to the caller's one
   add: StrStrip(), StrChar()
   add: toFUpper(), MemFCmp(), _fMemFCmp()
   add: dfnpath(), dfntruename()
   chg: toUpper(): retreive the upcase table only once
   add: _fStrCmp(), _fMemCmp(), StrCmp(),& MemCmp()
   add: toUpper()
   add: env_matchVar()
   add: env_dupvar() & dupvar()
   chg: env_switch() -> env_replace()

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

   Revision 1.2  1996/07/29 06:53:45  ska
   add: DOSalloc(), addu()
   chg: newenv(), sizenv()

   Revision 1.1  1996/07/25 06:34:48  ska
   Initial revision

*/

#ifndef __SUPPL_H
#define __SUPPL_H

#include <stdio.h>

#include <supl_def.h>

#include <portable.h>

#define itemsof(array)	(sizeof(array) / sizeof(array[0]))

/* length of the longmath numbers. */
#define LSIZE sizeof(dword)

#ifndef _MICROC_

#define longsubi(a,b) ((a) -= (int)(b))
#define longsubu(a,b) ((a) -= (unsigned)(b))
#define longaddi(a,b) ((a) += (int)(b))
#define longaddu(a,b) ((a) += (unsigned)(b))
#define longmuli(a,b) ((a) *= (int)(b))
#define longmulu(a,b) ((a) *= (unsigned)(b))
#define longdivi(a,b) ((a) /= (int)(b))
#define longdivu(a,b) ((a) /= (unsigned)(b))
#define longseti(a,b) ((a) = (int)(b))
#define longsetu(a,b) ((a) = (unsigned)(b))
#define longshln(a,c) ((a) <<= (c))
#define longshrn(a,c) ((a) >>= (c))
int longcmp(long d1, long d2);
#define longcmp_(a,b) ((signed long)(a) - (signed long)(b))
#define longsgn(a)    ((signed long)(a))
#define longclr(a)	  ((a) = 0)

#define poked(seg,ofs,value)	(*(dword far*)MK_FP(seg,ofs) = (value))
#define pokedd(seg,ofs,hi,lo)	(*(dword far*)MK_FP(seg,ofs) = ((dword)(hi)<<16)|(lo))
#define peekd(segm,ofs,value)	((value) = *(dword far*)MK_FP(segm,ofs))
int peekdd(word segm, word ofs, word *valHi, word *valLo);

typedef char far *fartbl;		/* far table */
#define dpeekc(fartable,idx)	(*((char far *)fartable) + (idx))
#define dpeekb(fartable,idx)	(*((byte far *)fartable) + (idx))
#define dpeekw(fartable,idx)	(*(word far*)(((byte far *)fartable) + (idx)))

#define _fmemcmpi _fmemicmp
#define _fstrcmpi _fstricmp
#define memcmpi memicmp

#else		/* defined(_MICROC_) */

#define longcmp_ longcmp
#define longsgn(a) (longtst(a) && ((int*)(a))[1])

#define fartbl dword		/* pointer to far table */
//char dpeekc(fartbl fartable, int idx);
#define dpeekc(tbl,idx) (char)dpeekb((tbl), (idx))
int dpeekb(fartbl fartable, int idx);
word dpeekw(fartbl fartable, int idx);

#endif		/* !defined(_MICROC_) */



#define putcm fputcm
int fputmc(int ch, int cnt, FILE *f);
/*	Emit the same character several times into the stream
*/


word DOSalloc(const word length, const int mode);
/* Allocate a chunk of memory directly via the DOS API.
	length is the number of paragraphes that shall be allocated.
	mode is the allocation strategy to be used.

	If mode specifies to use high memory, the high memory will be
	automatically made accessable.

	If the bit 0x10 of mode is set, length is ignored and the size of
	the largest unallocated block is returned, which can be allocated
	later.

	This function does not alter the allocation strategy permanently.

	Return:
		0: Allocation failed
		else:	if 0x10 of mode was clear: segment address of allocated memory
				if 0x10 of mode was set: the largest unallocated block
*/

int DOSresize(const word segm, const word length);
/* Resize a chunk of memory directly via the DOS API.
	segm is the segment to resize, it must be allocated via the DOS API
		or a comparable function, like DOSalloc(), allocBlk().
	length is the new number of paragraphes.

	If to resize the segment fails, the block of memory is not relocated
	into a larger space of the DOS memory.

	Return:
		0: resize failed
		!0: resize OK
*/

int DOSfree(const word segm);
/* Free a chunk of memory directly via the DOS API.
	segm is the segment to resize, it must be allocated via the DOS API
		or a comparable function, like DOSalloc(), allocBlk().

	Return:
		0: free OK
		!0: DOS error code
*/

//word allocBlk(const unsigned length, const int allocMode);
#define allocBlk(length,allocMode) DOSalloc(BLK_byte2para(length), allocMode)
/* Allocates a block of memory using the DOSalloc() function.
	The only difference is the ¯length® parameter:
		Here it is the requested length of the block in bytes, thus,
		the block can contain only 0..65535 bytes (up to 64KB).
*/

#ifdef _MICROC_
word resizeBlk(const word segm, const unsigned length);
#else
#define resizeBlk(segm,length) DOSresize(segm, BLK_byte2para(length))
#endif
/* Resizes the segment using the DOSresize() function.
	The only difference is the ¯length® parameter:
		Here it is the requested length of the block in bytes, thus,
		the block can contain only 0..65535 bytes (up to 64KB).
*/

//int freeBlk(const word segm);
#define freeBlk DOSfree
/* Free the segment using the DOSfree() function.
	No differences, only for completeness.
*/

unsigned BLK_byte2para(const unsigned bytes);
/* Return the number of paragraphes that will be needed to store ¯bytes®
	bytes in.

	If bytes == 0, the return value is zero, too.

*/

int addu(unsigned *u1, unsigned u2);
/* Performs the operation:
		*u1 += u2;
	and returns, if an overflow occured.

	Return:	0: no overflow
			!0: overflow
*/
#ifdef __WATCOMC__
#ifdef FARDATA
#pragma aux addu = \
	"add word ptr es:[bx], ax"	\
	"sbb ax, ax"	\
	parm [es bx] [ax]	\
	value [ax]	\
	modify [es bx]
#else
#pragma aux addu = \
	"add word ptr [bx], ax"	\
	"sbb ax, ax"	\
	parm [bx] [ax]	\
	value [ax]	\
	modify [bx]
#endif
#endif

int invokeDOS(nM(IREGS *prp));
/* Invoke DOS using intr(0x21, prp).

	Return:
		 0: DOS returned with Carry flag cleared
		!0: AX value (DOS error code) if DOS returned with Carry flag set
*/

#ifndef _CLIB_
char *stpcat(char * const dst, const char * const src);
/* Append src to dst and return the address of the '\0' character of dst
*/
#endif


#ifdef __WATCOMC__
char *stpcpy(char * const dst, const char * const src);
/*	Copy src into dst
	Return pointer to '\0' terminator of dst
*/

/* different names */
struct ffblk {
    char        ff_reserved[21];
    byte        ff_attrib;
    unsigned    ff_ftime;
    unsigned    ff_fdate;
    long        ff_fsize;
    char        ff_name[13];
};

#endif	/* WATCOMC */


#ifdef _PAC_NOCLIB_
char *stpcpy(char * const dst, const char * const src);
/* Copy src onto dst, Return a pointer to the '\0' character of dst
*/

int getdisk(void);
/*	Return the current working drive. 0 == A:, 1 == B: ...
*/

int setdisk(void);
/*	Change the current working drive. 0 == A:, 1 == B: ...
	Returns the number of available drive letters (not necessary
	currently accessable).
*/

/*
 *	Pacific does not offer any OS version information
 */
unsigned osversion(void);
/*	Return: _osmajor | (_osminor << 8)
*/

/*
 *	No findfirst()
 */
struct ffblk {
    char        ff_reserved[21];
    byte        ff_attrib;
    unsigned    ff_ftime;
    unsigned    ff_fdate;
    long        ff_fsize;
    char        ff_name[13];
};

int findfirst(const char * const pattern, struct ffblk *ff, int attrib);
/*	Initiate file search
	Return:	0 on success
*/

int findnext(struct ffblk *ff);
/*	Next file search
	Return:	0 on success
*/

void far *getdta(void);
/*	Return current working DTA
*/

void setdta(void far *dta);
/*	Set current working DTA
*/

unsigned _psp_(void);
/*	Return the current PSP
*/

void ctrlbrk(int (*fct)(void));
/*	Set new ^Break handler
*/

#endif	/* _PAC_NOCLIB_ */

#ifdef COMPILE
#undef COMPILE
#endif

#ifdef _MICROC_
#define COMPILE
#endif
#ifdef _PAC_NOCLIB_
#define COMPILE
#endif
#ifdef __WATCOMC__
#define COMPILE
#endif
#ifdef _TC_EARLY_
#define COMPILE
#endif

#ifdef COMPILE
#undef COMPILE

char *_getdcwd(int drive, char *buf, unsigned length);
/*	Return the fully-qualified path of drive in the format:
		?:\...\...\...
	If buf == NULL, a buffer of length bytes is allocated.

	Return:
		NULL: On failure
		else: on success (buf or allocated buffer)
*/
#endif

int getbootdisk(void);
/*	as getdisk() but returns the boot drive
	Return:
		1 == A:, 2 == B:
		0: failure
*/

#endif
