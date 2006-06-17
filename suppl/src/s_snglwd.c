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

ob(ject): singleword
su(bsystem): misc
ty(pe): 
sh(ort description): Return trimmed single word
lo(ng description): Tests if a line (string) contains wholely exactly
	one word and, if so, removes any leading and trailing whitespaces.
pr(erequistes): 
va(lue): NULL: \tok{s == NULL} or the line contains either no word
	or more than one word
	\item else: the first non-whitespace character of the string; the first
		end-of-word character is overwritten by \tok{'\0'}
re(lated to): skipwd skipws lookupwd_
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

char *singleword(char *s)
{	char *p;

	DBG_ENTER("singleword", Suppl_misc)

	if(!s)
		DBG_RETURN_P( 0)

	DBG_ARGUMENTS( ("str=%p \"%s\"", s, s) )

	if(*(s = skipws(s)) == '\0'		/* no word at all */
	 || *skipws(p = skipwd(s)))		/* more than one word */
		DBG_RETURN_P( 0)

	*p = '\0';

	chkHeap
	DBG_RETURN_P( s)
}
