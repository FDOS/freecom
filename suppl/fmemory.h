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
/* $Id$
   $Locker$	$Name$	$State$

	Far memory handling function.

*/

#ifndef __FMEMORY_H
#define __FMEMORY_H

#include <supl_def.h>

#ifdef _MICROC_				/* for use for Micro-C only */

#define MK_FP(seg,ofs) (seg), (ofs)
#define TO_FP(var) get_ds(), (var)
#define _fnormalize(seg,ofs) _fnormalize_(&(seg), &(ofs))

#else	/* !_MICROC_ */

#ifdef _TC_LATER_
#include <string.h>

#else	/* !_TC_LATER_ */
#if defined(_PAC_NOCLIB_) || defined(_TC_EARLY_) || defined(__GNUC__)

#ifdef _PAC_NOCLIB_
#include <stdlib.h>
#define _fmemcpy farmemcpy
#define _fmemset farmemset

#else	/* !_PAC_NOCLIB_ */
void far *_fmemcpy(void far * const dst, const void far * const src, unsigned length);
void far *_fmemset(void far * const dst, int ch, unsigned length);
#endif	/* _PAC_NOCLIB_ */

unsigned _fstrlen(const char far * const s);
char far *_fstrchr(const char far * const s, int ch);
char far *_fmemchr(const char far * const s, int ch, unsigned length);
void far *_fmemmove(void far * const dst, const void far * const src, unsigned length);
int _fmemcmp(const void far * const dst, const void far * const src, unsigned length);
int _fmemicmp(const void far * const dst, const void far * const src, unsigned length);
int _fstrcmp(const char far * const dst, const char far * const src);
int _fstricmp(const char far * const dst, const char far * const src);
void _fstrcpy(char far * const dst, const char far * const src);

#endif /* defined(_PAC_NOCLIB_) || defined(_TC_EARLY_) || defined(__GNUC__) */
#endif		/* _TC_LATER_ */

#if defined(HI_TECH_C) || defined(_TC_EARLY_)
#endif
	/* HiTech C has no huge pointers
	 & early Turbo C does ignore the cast */
	/* In some circumstances any Borland C compiler do ignore the
		cast, so the macro is not suitable */
void far *_fnormalize(void far *poi);
/* #define _fnormalize(poi) ((void huge *)(poi)) */

char *_fdupstr(const char far * const s);
#endif	/* _MICROC_ */

#ifdef _MICROC_
unsigned _fstpcpy(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length)
#else	/* !_MICROC_ */
char far* _fstpcpy (char far* dst, const char far* src);
#endif	/* _MICROC_ */

#endif
