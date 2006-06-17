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

ob(ject): skipwd
su(bsystem): misc
ty(pe): 
sh(ort description): Skip to end of word
lo(ng description): Skips to the end of the word pointed to by \para{s}.\par
	The end of a word is any character \tok{ch}, for which
	\tok{isspace(ch) || ch == '\0'} is true.\newline
	If \tok{*s} is an end-of-word character itself, \tok{s} is returned.
pr(erequistes): 
va(lue): NULL: if \tok{s == NULL}
	\item \para{s}: if \tok{*s ==} end-of-word character
	\item else: first end-of-word character of string \para{s}
re(lated to): skipws
se(condary subsystems): 
wa: No quotes are recognized.
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>
#include <ctype.h>
#include "str.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *skipwd(const char *s)
{
	DBG_ENTER("skipwd", Suppl_misc)

	if(!s)
		DBG_RETURN_P( 0)

	DBG_ARGUMENTS( ("str=%p \"%s\"", s, s) )

	while(*s && !isspace(*s)) ++s;

	DBG_RETURN_P( (char *)s)
}
