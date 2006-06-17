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

	char *StrChar(int ch)

	Create a dynamic string containing the single character ch.

	Return:
		NULL: malloc() returned NULL
		else: pointer to the malloc()'ed string

	Target compilers: Micro-C v3.13, v3.14; Borland C v2, v3.1, v4.52

ob(ject): StrChar
su(bsystem): dynstr
ty(pe): H
sh(ort description): Create a dynstring from a character
lo(ng description): Creates a dynamic string out of a single character
va: NULL: if \tok{malloc()} fails (ENOMEM)
	\item else: pointer to dynamically allocated string
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrchar.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>
#include "dynstr.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrChar(int ch)
{	char *h;

	DBG_ENTER("StrChar", Suppl_dynstr)
	DBG_ARGUMENTS( ("ch='%c' (0x02x)", ch > 32? ch: ' ', ch) )

	chkHeap
	if((h = eno_strdup(" ")) != 0)
		*h = ch;

	chkHeap
	DBG_RETURN_S( h)
}
