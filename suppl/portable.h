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


*/

#ifndef __PORTABLE_H
#define __PORTABLE_H

#include <regproto.h>
#define NUL '\0'
#define MK_OSVERS(major,minor) ((major << 8) | (minor))

#ifdef FARDATA
#undef FARDATA
#endif
#ifdef FARCODE
#undef FARCODE
#endif

#ifdef _MICROC_

#include <p-mc.h>

#else	/* no _MICROC_ */

#if defined(__HUGE__) || defined(__LARGE__) || defined(__COMPACT__)
#define FARDATA
#endif
#if defined(__HUGE__) || defined(__LARGE__) || defined(__MEDIUM__)
#define FARCODE
#endif

/*
 *	Currently supported:
 *		Borland C/C++ v2+
 *		Pacific C v7.51
 *		Watcom v10.6
 */

#define iM(q)			/* if Micro-C */
#define nM(q)	q		/* not Mirco-C */
#define inM(q,w)	w	/* if-not Micro-C */
#define aF(fct) (fct)		/* address of function */
#define aS(structure) &(structure)	/* address of structure */
#define cS(strucType) (strucType)	/* casting of a structure */


/* Handling of pointers to structures */
#define sInc(poi) ++(poi)
#define sDec(poi) --(poi)
#define sIdx(array,index) ((array) + (index))
#define sAdd(poi,displacement) ((poi) += (displacement))
#define sSub(poi,displacement) ((poi) -= (displacement))

/* far pointer variable handling */
#define TO_FP(var) ((void far*) (var))	/* cast variable to far pointer */
#define FARARG(var,type) type far* var
#define COPYFARARG(var) var

/* DWORD variables handling */
#define setDW(var,seg,ofs) ((var) = (dword)MK_FP(seg,ofs))
#define setDWP(varP,seg,ofs) (*(varP) = (dword)MK_FP(seg,ofs))
#define DW_LO(var) ((word)(var))
#define DW_HI(var) ((word)((var) >> 16))
#define DWP_LO(var) ((word)(var))
#define DWP_HI(var) ((word)((var) >> 16))
#define DWARG dword

/* Simulate long constants and long arguments */
#define TO_LONG(num) (unsigned long)(num)
#define MK_LONG(hi,lo) (((unsigned long)(hi) << 16) | (unsigned)(lo))


#ifndef NULL
#define NULL ((void*)0)
#endif

/* LONGMATH */
#define longcpy(a,b) ((a) = (b))
#define longset(a,b) ((a) = (b))
#define longmul(a,b) ((a) *= (b))
#define longdiv(a,b) ((a) /= (b))
#define longadd(a,b) ((a) += (b))
#define longsub(a,b) ((a) -= (b))
#define longtst(a) (!!(a))
int longcmp(long d1, long d2);
#define longcmp1(a,b) ((signed long)(a) - (signed long)(b))

#define longseti(a,b) ((a) = (int)(b))
#define longmuli(a,b) ((a) *= (int)(b))
#define longdivi(a,b) ((a) /= (int)(b))
#define longaddi(a,b) ((a) += (int)(b))
#define longsubi(a,b) ((a) -= (int)(b))

#define longsetu(a,b) ((a) = (unsigned)(b))
#define longmulu(a,b) ((a) *= (unsigned)(b))
#define longdivu(a,b) ((a) /= (unsigned)(b))
#define longaddu(a,b) ((a) += (unsigned)(b))
#define longsubu(a,b) ((a) -= (unsigned)(b))


/* Source In compiler specific settings */
#ifdef __TURBOC__
#include "p-bc.h"
#endif

#ifdef HI_TECH_C
#include "p-pac.h"
#endif

#ifdef __WATCOMC__
#include "p-watcom.h"
#endif

#endif	/* _MICROC_ */

#ifndef strend
#define strend(s)	strchr((s), '\0')
#endif

#endif
