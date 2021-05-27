/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-2000 Steffen Kaiser

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $RCSfile$
   $Locker$	$Name$	$State$

ob(ject): nlsNewInfo
su(bsystem): nls
ty(pe): 0
sy(nopsis): 
sh(ort description): Reread the NLS information and return a pointer to it
he(ader files): 
lo(ng description): Updates the internal, static buffer caching
	all NLS information provided by the DOS-65-XX interface and returns
	a pointer to it. The buffer caches information, such as:
	\enum code page
	\item country ID
	\item date format
	\item currency symbol string
	\item thousands separator string
	\item decimal sperator string
	\item date separator string
	\item time separator string
	\item currency format
	\item time format
	\item data-list separator string
	\item pointer to uppercase table
	\item pointer to lowercase table
	\item pointer to filenmae uppercase table
	\item pointer to filename character table
	\item pointer to collating sequence table
	\endlist
	\par
	This function should be called whenever the NLS information is required,
	but the possibility exists that the DOS NLS has been changed during
	the last call to \tok{nlsInfo()}.\par
pr(erequistes): 
va(lue): always a pointer to a static buffer
re(lated to): nlsInfo
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): cntry.c
in(itialized by): 
wa(rning): 
	Note: Though for future release one should not rely on the fact that
	always the \em{same} pointer is returned.
bu(gs): 

ob(ject): nlsInfo
su(bsystem): nls
ty(pe): 0
sy(nopsis): 
sh(ort description): Read the NLS information and return a pointer to it
he(ader files): 
lo(ng description): As \tok{nlsNewInfo}, but does update the internal
	buffer only once per life-time of the program
pr(erequistes): 
va(lue): pointer to internal buffer
re(lated to): nlsNewInfo
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): suppl_cntry
su(bsystem): nls
ty(pe): 
sy(nopsis): 
sh(ort description): Cached information for \tok{nlsInfo()}
he(ader files): 
lo(ng description): Caches information retreived by nlsNewInfo() from
	the DOS-65-XX NLS interface.
pr(erequistes): 
va(lue): 
re(lated to): 
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): nlsNewInfo
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif /*#	defined(_MICROC_) */

#include "cntry.h"
#include "fmemory.h"
#include "str.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif /*#	defined(RCS_Version) */

static Country suppl_cntry = {0};	/* make sure the structure is zeroed out */


#ifdef _MICROC_
#define savePtr(field)	savePtr_(&suppl_cntry.field/**/Len		\
							, suppl_cntry.field/**/Tbl		\
							, &buf[1])
static void savePtr_(int *len, dword *tbl, word *ptr)
{	word segm, ofs;

	ofs = *ptr;
	segm = ptr[1];

	if((*len = peekw(segm, ofs)) <= 0x80)
		ofs -= 0x80;
	ofs += 2;

	tbl->lo = ofs;
	tbl->hi = segm;

	if(!longtst(tbl))		/* no table --> no length */
		*len = 0;
}
#else /*#	!(defined(_MICROC_)) */
#define savePtr(field)	savePtr_(&suppl_cntry.field##Len		\
							, &suppl_cntry.field##Tbl		\
							, (byte far* *)&buf[1])
static void savePtr_(int *len, char far * *tbl, byte far * *ptr)
{	byte far *p;

	p = *ptr;
	if((*len = *(word far*)p) <= 0x80)
		p -= 0x80;
	p += 2;
	if((*tbl = (char far*)p) == 0)		/* no table --> no length */
		*len = 0;
}
#endif /*#	defined(_MICROC_) */



Country *nlsInfo(void)
{	DBG_ENTER("nlsInfo", Suppl_nls)
	DBG_LEAVE( suppl_cntry.initialized? aS(suppl_cntry): nlsNewInfo())
}

void nlsWriteFallback(Country *nls)
{
	static char illchars[] = "\"+,./:;<=>[\\]|";
#if defined(DBCS)
# if defined(JAPANESE)
	static unsigned char dbcstbl[] = { 0x81, 0x9f, 0xe0, 0xfc, 0, 0 };
# else
	static unsigned char dbcstbl[] = { 0, 0, 0, 0, 0, 0 };
# endif
#endif
	
	nls->country = 1;
	nls->charset = 437;
	nls->datefmt = 0;
	nls->curSymbol[0] = '$';
	nls->thousendsSep[0] = ',';
	nls->decimalSep[0] = '.';
	nls->dateSep[0] = '-';
	nls->timeSep[0] = ':';
	nls->curFormat = 0;
	nls->precision = 2;
	nls->timefmt = 0;
	nls->listSep[0] = ',';

#if defined(JAPANESE)
	nls->country = 81;
	nls->charset = 932;
	nls->datefmt = 2;
	nls->curSymbol[0] = '\\';
	nls->curFormat = 0;
	nls->precision = 0;
	nls->timefmt = 1;
#endif

	nls->inclFirst = 0; nls->inclLast = 0xff;
	nls->exclFirst = 0; nls->exclLast = 0x20;
	nls->illegalLen = strlen(illchars);
	nls->illegalChars = (char far *)illchars;
#if defined(DBCS)
	nls->dbcsTbl = (unsigned char far *)dbcstbl;
#endif
}

Country *nlsNewInfo(void)
{	unsigned char buf[50];
#if 1
	IREGS r;

	DBG_ENTER("nlsNewInfo", Suppl_nls)

#ifdef _MICROC_
	r.r_es = get_ds();
	r.r_di = &buf[0];
#else
	r.r_es = FP_SEG(&buf[0]);
	r.r_di = FP_OFF(&buf[0]);
#endif
	
	r.r_bx = r.r_dx = 0xffff;
#define DOS(val) r.r_ax = 0x6500 | (val);	\
	r.r_cx = sizeof(buf);					\
    intrpt( 0x21, &r );                       \
	if(0 == ( r.r_flags & 1 ) ? r.r_ax : 0)
#else
#ifdef _MICROC_
	IREGS r;

	DBG_ENTER("nlsNewInfo", Suppl_nls)

	r.r_es = get_ds();
	r.r_di = &buf[0];
	r.r_bx = r.r_dx = 0xffff;
#define errnr ((r.r_flags & 1)? r.r_ax : 0)
#define DOS(val) r.r_ax = 0x6500 | (val);	\
	r.r_cx = sizeof(buf);					\
	intrpt(0x21, r);							\
	if(!errnr)
#else /*#	!(defined(_MICROC_)) */
	IREGS r;

	DBG_ENTER("nlsNewInfo", Suppl_nls)

	r.r_es = FP_SEG(&buf[0]);
	r.r_di = FP_OFF(&buf[0]);
	
	r.r_bx = r.r_dx = 0xffff;
	r.r_ax = 0x6500;
#define DOS(val) r.r_ax = 0x6500 | (val);	\
	r.r_cx = sizeof(buf);					\
	intrpt(0x21, &r);					\
	if(!errnr)
#define errnr (r.r_flags & 1)? r.r_ax : 0)
#endif /*#	defined(_MICROC_) */
#endif 

	chkHeap
#define getbyte(idx)	buf[(idx)]
#define getword(idx)	(*(word*)&buf[(idx)])
#define cpybyte(field,idx)	(suppl_cntry.field = buf[(idx)])
#define cpyword(field,idx)	(suppl_cntry.field = (*(word*)&buf[(idx)]))
#define cpymem(field,idx)	memcpy(suppl_cntry.field, &buf[(idx)]		\
			, sizeof(suppl_cntry.field) - 1)

	memzerostrct(suppl_cntry);
	nlsWriteFallback(&suppl_cntry);

	DOS(1)	{	/* Extended country inmformation */
		cpyword(country, 3);
		cpyword(charset, 5);
		cpyword(datefmt, 7);
		cpymem(curSymbol, 9);
		cpymem(thousendsSep, 14);
		cpymem(decimalSep, 16);
		cpymem(dateSep, 18);
		cpymem(timeSep, 20);
		cpybyte(curFormat, 22);
		cpybyte(precision, 23);
		cpybyte(timefmt, 24);
		cpymem(listSep, 29);
	}

	DOS(2)		/* uppercase table for normal characters */
		savePtr(uppercase);

	DOS(3)		/* lowercase table for normal characters */
		savePtr(lowercase);

	DOS(4)		/* uppercase table for filename characters */
		savePtr(fupcase);

	DOS(6)		/* lowercase table for normal characters */
		savePtr(collate);

	DOS(5) {	/* filename character table */
#ifdef _MICROC_
		cpymem(illegalChars, 11);
		suppl_cntry.illegalChars.lo += 10;
		copy_seg(get_ds(), buf, *(word*)&buf[3], *(word*)&buf[1], 10);
#else /*#	!(defined(_MICROC_)) */
		suppl_cntry.illegalChars = *(char far * *)&buf[1] + 10;
		_fmemcpy(buf, *(byte far* *)&buf[1], 10);
#endif /*#	defined(_MICROC_) */

		cpybyte(inclFirst, 3);
		cpybyte(inclLast, 4);
		cpybyte(exclFirst, 6);
		cpybyte(exclLast, 7);
		suppl_cntry.illegalLen= getbyte(9); 
	}

#if defined(DBCS)
	r.r_ax = 0x6300;
	r.r_ds = r.r_si = 0;
	intrpt(0x21, &r);
	if ( !(r.r_flags & 1) && r.r_ds && r.r_si ) {
		suppl_cntry.dbcsTbl = MK_FP(r.r_ds, r.r_si);
	}
#endif

	suppl_cntry.initialized = 1;

	chkHeap
	DBG_LEAVE( aS(suppl_cntry))
}

#ifdef TEST
main(void)
{	nlsInfo();
	return 0;
}
#endif /*#	defined(TEST) */
