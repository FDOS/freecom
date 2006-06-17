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

ob(ject): StrAppQStr_
su(bsystem): dynstr
ty(pe): _H
sh(ort description): Append a quoted string to a dynstr
lo(ng description): 
	Appends a string to a dynamic string. The string is delimited
	from the previous contents of the dynamic string by an user-supplied
	character and, if necessary, quoted by pairy or unpairy string quotes or
	single character quotes.\par
	If \tok{src == NULL}, this function performs no action.\newline
	If \tok{*dst == NULL || **dst == NUL} (== string is empty), \tok{src[]} is quoted,
	if necessary, but no delimiter is included.\newline
	If \tok{*dst} does not end with one character from the string
	\tok{quotes->str_delimiters[]} (== old strings does not end with a delimiter),
	\tok{*quotes->str_delimiters} is placed between \tok{*dst} and \tok{src[]}.\par
	The whole \tok{src[]} is quoted completely.
pr(erequistes): quotes != NULL && dst != NULL
va: NULL: if \tok{malloc()} fails (ENOMEM), \tok{*dst} may be changed
	\item else: pointer to new location, also in \tok{*dst}
re(lated to): StrQuote STR_QUOTES StrAppDelimStr
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrappe.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrAppQStr_(char ** const dst, const char * const src
 , const STR_QUOTES * const quotes)
{
	char *str;		/* temporary quoted string */
	char *p;		/* temporary result */

	DBG_ENTER("StrAppQStr_", Suppl_dynstr)

	assert(dst);

	DBG_ARGUMENTS( ("dst=\"%s\", src=\"%s\"", dst, src) )

	if(!src || !*src)
		DBG_RETURN_S(*dst)

	chkHeap
	if((str = StrQuote(src, quotes)) == 0)
		DBG_RETURN_S(0)

	if(!dst || !*dst)
		DBG_RETURN_BS(StrRepl_(dst, str))

	chkHeap
	p = StrAppDelimStr_(dst, str, quotes);
	free(str);

	chkHeap
	DBG_RETURN_S(p);
}
