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

	char *EStrConcat(int argcnt, ...)

	Concats up to argcnt strings together and malloc() a buffer that will
	receive the result.

	If one of the string == NULL, this string is ignored.

	On failure, the program is terminated with the "out of memory" error.

	Return:
		the malloc'ed buffer

ob(ject): EStrConcat
su(bsystem): error
ty(pe): H
sh(ort description): Concat several strings together
lo(ng description): Concats several strings together, by using the
	\tok{StrConcat()} function, and terminates the program on failure
	with the error message: "Out of memory"
pr(erequistes): 
re(lated to): StrConcat
se(condary subsystems): dynstr
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): constructed dynamic string
fi(le): eestrcon.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#endif
#include "dynstr.h"
#include "msgs.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#ifdef _MICROC_
register char *EStrConcat(int argcnt)
{	unsigned cnt, *poi;
	unsigned Xcnt, *Xpoi;
	unsigned length;
	char *h, *p;
	DBG_ENTER1

	cnt = nargs();

	DBG_ENTER2("EStrConcat", "error")
	DBG_ARGUMENTS( ("argcnt=%u cnt=%u", argcnt, cnt) )

	Xpoi = poi = cnt * 2 - 2 + &argcnt;
	Xcnt = cnt = min(cnt, *poi);
	for(length = 1; cnt--;)
		if(*--poi) length += strlen(*poi);

	chkHeap
	if((h = p = malloc(length)) == 0)
		Esuppl_noMem();

	chkHeap
	while(Xcnt--)
		if(*--Xpoi)
			p = stpcpy(p, *Xpoi);

	chkHeap
	DBG_RETURN_S( h)
}
#else		/* !_MICROC_ */

char *EStrConcat(int argcnt, ...)
{	va_list strings;
	char *p, *s;
	size_t length, l;

	DBG_ENTER("EStrConcat", Suppl_error)
	DBG_ARGUMENTS( ("argcnt=%u cnt=%u", argcnt, argcnt) )

	va_start(strings, argcnt);
	chkHeap
	p = Estrdup("");
	chkHeap
	length = 1;
	while(argcnt--) {
		s = va_arg(strings, char *);
		if(s && *s) {
			l = length - 1;
			Eresize(p, length += strlen(s));
			strcpy(p + l, s);
		}
	}
	va_end(strings);
	chkHeap
	DBG_RETURN_S( p)
}
#endif		/* _MICROC_ */
