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

	char *StrConcat(int argcnt, ...)

	Concats up to argcnt strings together and malloc() a buffer that will
	receive the result.

	If one of the string == NULL, this string is ignored.

	Return:
		NULL: if malloc() returned NULL
		else: the malloc'ed buffer

ob(ject): StrConcat
su(bsystem): dynstr
ty(pe): H
sh(ort description): Concat several strings
lo(ng description): Concats several strings and places the result into
	a dynamic string.\par
	In Micro-C \para{argcnt} cannot exceed the number of arguments passed
	to the functions.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrconc.c
va: NULL: memory allocation error (ENOMEM)
	\item else: dynamically allocate buffer with joined strings; if all
	strings are NULL or no string was passed into this function at all,
	an empty string is created

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#endif
#include "dynstr.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#ifdef _MICROC_
register char *StrConcat(int argcnt)
{	unsigned cnt, *poi;
	unsigned Xcnt, *Xpoi;
	unsigned length;
	char *h, *p;
	DBG_ENTER1

	cnt = nargs();

	DBG_ENTER2("StrConcat", "dynstr")
	DBG_ARGUMENTS( ("argcnt=%u cnt=%u", argcnt, cnt) )

	Xpoi = poi = cnt * 2 - 2 + &argcnt;
	Xcnt = cnt = min(cnt, *poi);
	for(length = 1; cnt--;)
		if(*--poi) length += strlen(*poi);

	chkHeap
	if((h = p = eno_malloc(length)) == 0)
		DBG_RETURN_S( 0)

	chkHeap
	while(Xcnt--)
		if(*--Xpoi)
			p = stpcpy(p, *Xpoi);

	chkHeap
	DBG_RETURN_S( h)
}
#else		/* !_MICROC_ */

char *StrConcat(int argcnt, ...)
{	va_list strings;
	char *h, *p, *s;
	unsigned length;

	DBG_ENTER("StrConcat", Suppl_dynstr)
	DBG_ARGUMENTS( ("argcnt=%u cnt=%u", argcnt, argcnt) )

	va_start(strings, argcnt);
	chkHeap
	if((p = eno_strdup("")) == 0)
		DBG_RETURN_S( 0)
	chkHeap
	length = 1;
	while(argcnt--) {
		s = va_arg(strings, char *);
		if(s && *s) {
			chkHeap
			if((h = eno_realloc(p, length += strlen(s))) == 0) {
				free(p);
				DBG_RETURN_S( 0)
			}
			strcat(p = h, s);
		}
	}
	chkHeap
	DBG_RETURN_S( p)
}
#endif		/* _MICROC_ */
