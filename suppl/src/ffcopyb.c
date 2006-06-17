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

ob(ject): Fcopybuf
su(bsystem): supplio
ty(pe): 
sy(nopsis): 
sh(ort description): Copy one block from one stream to another one
he(ader files): 
lo(ng description): Copies one block from stream \para{fsrc}
	beginning at the current position
	into stream \para{fdst} beginning with the current position.\par
	If the destination stream \para{fdst} is currently positioned
	before its physical end of file, this part is overwritten, but
	not truncated, if the source stream \para{fsrc} is less bytes
	left than \para{fdst}.\par
	On return the streams are positioned behind the last read or
	write operation.\par
pr(erequistes): \para{fsrc} != NULL && \para{fdst} != NULL && \para{num} != NULL
	&& \para{len} > 0
va(lue): 0: on success
	\item 1: possible read error / EOF / \para{*num} < \para{len}
	\item 2: write error
re(lated to): Fcopy Fcopyto Fcopyi Fcopyl
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <assert.h>
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int Fcopybuf(FILE * const fdst, FILE * const fsrc, byte * const buf
	, const size_t len, size_t * const num)
{	
	DBG_ENTER("Fcopybuf", Suppl_supplio)

	assert(fsrc);
	assert(fdst);
	assert(buf);
	assert(num);
	assert(len > 0);

	chkHeap
	if((*num = Fget(buf, len, fsrc)) <= 0)		/* possible read error */
		DBG_RETURN_I(1)	

	chkHeap
	DBG_RETURN_VALUES( ("num=%u", *num) )

	if(FFput(buf, *num, fdst))
		DBG_RETURN_I( 2)			/* write error */

	DBG_RETURN_I(*num < len)		/* == 1 on read error */
}
