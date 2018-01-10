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

ob(ject): Fmaxbuf
su(bsystem): supplio
ty(pe): H
sh(ort description): Allocate a large chunk of memory
pr(erequistes): buf != NULL && len != NULL
va(lue): \tok{0}: on success; \tok{*buf} := pointer to dynamically allocated
	buffer, \tok{*len} := length of the buffer
	\item else: on failure
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include "supplio.loc"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int Fmaxbuf(byte ** const Xbuf, size_t * const Xlen)
{	byte *buf;
	size_t len;

	DBG_ENTER("Fmaxbuf", Suppl_supplio)

	assert(Xbuf);
	assert(Xlen);

	len = 32 * 1024u;			/* try to allocate 32KB */
	chkHeap
	while((buf = malloc(len)) == 0)
		if((len >>= 1) < 16) {
			eno_set(ENOMEM);
			DBG_RETURN_I( 1)			/* out of memory */
		}

	*Xbuf = buf;
	*Xlen = len;

	chkHeap
	DBG_RETURN_VALUES( ("buf=%p, len=%u", buf, len) )
	DBG_RETURN_I( 0)
}
