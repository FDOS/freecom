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

ob(ject): Fxinfo
su(bsystem): supplio
ty(pe): L
sh(ort description): Return extended attributes attached to a stream
lo(ng description): Searches the list of the extended attributes for
	the specified stream / \tok{(FILE*)} pointer.\par
	\em{Note:} The returned address always points to a structure
	containing default, but valid values. See \tok{suppl_dfltExtAttr}
	for more information.
pr(erequistes):
va(lue): \tok{suppl_dfltExtAttr}: on failure
	\item else: pointer to the extended attributes of the stream
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): F_xinfo
su(bsystem): supplio
ty(pe): LH
sh(ort description): Return extended attributes attached to a stream
lo(ng description): Searches the list of the extended attributes for
	the specified stream / \tok{(FILE*)} pointer.\par
	This function acts as a backend for Fxinfo.\newline
	If \tok{prev != NULL},
	the previous item is not returned to the caller.
pr(erequistes):
va(lue): \tok{suppl_dfltExtAttr}: on failure; \tok{*prev := NULL}
	\item else: pointer to the extended attributes of the stream;
		\tok{*prev} := address of previous item in list, always
		unequal to \tok{NULL}.
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#include "supplio.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

Suppl_ExtAttr *F_xinfo(const FILE * const fp, Suppl_ExtAttr ** const prev)
{	Suppl_ExtAttr *p, *prv;

	DBG_ENTER("F_xinfo", Suppl_supplio)
	DBG_ARGUMENTS( ("fp=%p, prev=%p", fp, prev) )

	p = aS(suppl_dfltExtAttr);
	while((p = (prv = p)->sea_nxt) != 0)
		if(p->sea_fp == fp) {
			if(prev) *prev = prv;
			DBG_RETURN_P( p)
		}

	if(prev) *prev = 0;
	chkHeap
	DBG_STRING("Failed, returning default item")
	DBG_LEAVE( aS(suppl_dfltExtAttr))
}

Suppl_ExtAttr *Fxinfo(const FILE * const fp)
{	return F_xinfo(fp, 0);
}
