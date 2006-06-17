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

	Defines the StriCmp() function, a strcmpi() which upcases the characters
	with toUpper();

	Returns:  >0 s1 is greater than s2
			 ==0 both are equal
			  <0 s1 is less than s2

	Target compilers: Micro-C v3.13, v3.14; Borland C v2, v3.1, v4.52

ob(ject): StriCmp
su(bsystem): dynstr
ty(pe): 
sh(ort description): Compare two strings case-insensitively
lo(ng description): Compares two strings case-insensitively using the
 DOS NLS.
pr(erequistes): 
re(lated to): 
se(condary subsystems): nls
in(itialized by): 
wa(rning): 
bu(gs): 
value: <0: first string is less\item =0: both are equal\item >0: first string is greater
fi(le): dstrcmp.c

*/

#include "initsupl.loc"
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

int StriCmp(const char *s, const char *p)
{	int result, d;

	DBG_ENTER("StriCmp", Suppl_dynstr)
	DBG_ARGUMENTS( ("s1=\"%s\", s2=\"%s\"", s, p) )

	/* Handle NULL parameters */
	if(!s)
		DBG_RETURN_BI( p? -1: 0)
	if(!p)
		DBG_RETURN_I( 1)

	while((result = (d = toUpper(*s++)) - toUpper(*p++)) == 0 && d);
	DBG_RETURN_I( result)
}
