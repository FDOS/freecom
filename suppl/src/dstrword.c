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

	char *StrWord(char s[])

	Reduces the malloc()'ed area of string s by any leading and/or
	trailing whitespaces.

	Return:
		NULL: if s == NULL
		else: the pointer to the shrunk memory location.

ob(ject): StrWord
su(bsystem): dynstr
ty(pe): H
sh(ort description): Remove leading and trailing whitespaces
lo(ng description): Removes all leading and trailing whitespaces from a
	dynamic string and reallocates it to its smallest possible size.
pr(erequistes): 
re(lated to): 
va: \tok{s}: (changed if to shrink a memory relocates it)
se(condary subsystems): 
in(itialized by): 
wa(rning): The string \para{s} is not expected to be a variable, thus,
	the result is not stored there, but is returned as return value of
	this function only.
bu(gs): 
fi(le): dstrword.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#endif
#include <ctype.h>
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrWord(char * const s)
{	char *p;

	DBG_ENTER("StrWord", Suppl_dynstr)

	if(!s)
		DBG_RETURN_S( 0)

	if(isspace(*s)) {			/* left trim */
		p = s;
		while(*++p && isspace(*p));
		memmove(p, s, strlen(p) + 1);	/* we need to copy this area
											anyway */
	}

	/* right trim */
	p = strchr(s, '\0');
	while(--p >= s && isspace(*p));
	p[1] = '\0';

	chkHeap
	DBG_RETURN_BS( StrTrim(s))
}
