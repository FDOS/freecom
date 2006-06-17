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

ob(ject): lookupwd_
su(bsystem): cmdline
ty(pe): L
sh(ort description): Search for a word in an array of MSGID's
lo(ng description): Searches for a word in an array of language
	specific message strings. To test if the word matches an entry
	of the array the callback \para{cmpfct()} is invoked, which
	must return a \tok{strcmp()}-like value, e.g.:
	\item <0: for s1 < s2
	\item =0: for s1 == s2
	\item >0: for s1 > s2
	\endlist
	This function handles all required stuff to deal with MSGIDs.
pr(erequistes): cmpfct != NULL
va(lue): 0: no match found, \tok{wd == NULL} or \tok{arr == NULL}
	\item else: \tok{(index + 1)} into the array (incremented to
		ensure that the returned index is never zero itself)
re(lated to): 
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>
#include "msgs.loc"
#include "str.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

size_t lookupwd_(const char * const wd, const MSGID * const arr
	, inM(int cmpfct, int (*cmpfct)(char *, char *, size_t)))
{	size_t len;
	const MSGID *p;

	DBG_ENTER("lookupwd_", Suppl_cmdline)

	assert(cmpfct);

	if(!wd || (p = arr) == 0)
		return 0;

	DBG_ARGUMENTS( ("word=\"%s\", arr=%p", wd, arr) )

	len = strlen(wd) + 1;

	do if((cmpfct)((char*)wd, (char*)msgLock(*p), len) == 0) {
			msgUnlock(*p);
			DBG_RETURN_BU( (size_t)(p - arr) + 1)
		}
		else msgUnlock(*p);
	while(*++p);

	DBG_RETURN_U( 0)
}
