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

	char *StrStrip(char s[], int ch);
	Remove any number of the character ch from the end of the string s.

	s[] may be NULL.
	ch may be 0.

	Return:	always s[].

ob(ject): StrStrip
su(bsystem): dynstr
ty(pe): 
sh(ort description): Remove all trailing \para{ch} characters
lo(ng description): Removes all trailing \para{ch} characters
	from the end of the string \para{s}. The string is overwritten.\par
	The \tok{NUL} character cannot be removed and is ignored.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrstri.c

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

char *StrStrip(char * const s, int ch)
{	unsigned char *p;

	DBG_ENTER("StrStrip", Suppl_dynstr)

	if(s && (ch &= 0xFF) != 0) {
		p = (unsigned char*)strchr(s, NUL);
		while(--p >= (unsigned char*)s && *p == ch);
		p[1] = NUL;
	}

	DBG_RETURN_S( s)
}
