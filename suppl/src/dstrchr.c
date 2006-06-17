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

	char *StrChr(char s[], int ch)

	Searches for the character ch within the string s.

	Return:
		NULL: if 
		else: the pointer to the position witin s

	Target compilers: Micro-C v3.13, v3.14; Borland C v2, v3.1, v4.52

ob(ject): StrChr
su(bsystem): dynstr
ty(pe): 
sh(ort description): Search a character within a string
lo(ng description): Searches a character within a string.\par
	If \para{s} or \para{ch} are \tok{0} (zero), \tok{NULL}, meaning \em{failed}, is returned.
pr(erequistes): 
va: \tok{NULL}: if \tok{s == NULL || ch == 0 || ch not in s}
	\item else: pointer to first position of \tok{ch} in \tok{s}
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): \tok{\para{ch} == 0} does NOT return the end of the string, but
	returns \em{failed}!
bu(gs): 
fi(le): dstrchr.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrChr(const char * const s, int ch)
{	DBG_ENTER("StrChr", Suppl_dynstr)
	DBG_ARGUMENTS( ("str=\"%s\", ch='%c' (0x02x)", s, ch>32? ch: ' ', ch) )

	DBG_RETURN_BS((s && ch)? strchr(s, ch): 0)
}
