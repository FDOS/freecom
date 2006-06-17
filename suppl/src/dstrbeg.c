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

	int StrBeg(const char s1[], const char s2[])

	Return if s1[] begins with s2[].

	Neither s1[] nor s2[] may be NULL.

	If strlen(s2) == 0, return 0.

	Returns:  >0 s1 is greater than s2
			 ==0 both are equal
			  <0 s1 is less than s2

ob(ject): StrBeg
su(bsystem): dynstr
ty(pe): 
sh(ort description): Test if a string begins with a certain string
lo(ng description): Tests if a string begins with a certain string.\newline
	Return value as \tok{strcmp()}. If \tok{strlen(\para{s2}) == 0}, \tok{0}
	(zero), meaning matched, is returned.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
value: <0: first string is less\item =0: both are equal\item >0: first string is greater
fi(le): dstrbeg.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

int StrBeg(const char * const s1, const char * const s2)
{	int len;

	DBG_ENTER("StrBeg", Suppl_dynstr)
	assert(s1);
	assert(s2);
	DBG_ARGUMENTS( ("s1=\"%s\", s2=\"%s\"", s1, s2))

	if((len = strlen(s2)) == 0)
		DBG_RETURN_I(0)

	DBG_RETURN_BI( memcmp(s1, s2, len))
}
#endif
