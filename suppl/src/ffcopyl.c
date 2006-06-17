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

ob(ject): Fcopyl
su(bsystem): supplio
ty(pe): 
sy(nopsis): 
sh(ort description): Copy \para{len} bytes from one stream to another
he(ader files): 
lo(ng description): Copies \para{len} bytes from stream \para{fsrc}
	beginning at the current position
	into stream \para{fdst} beginning with the current position.\par
	If the destination stream \para{fdst} is currently positioned
	before its physical end of file, this part is overwritten, but
	not truncated, if the source stream \para{fsrc} has less bytes
	left than \para{fdst}.\par
	On return the streams are positioned behind the last read or
	written byte.\par
	For copying the maximal possible buffer is dynamically allocated.
pr(erequistes): \para{fsrc} != NULL && \para{fdst} != NULL
va(lue): 0: on success
	\item 1: \para{fsrc} not at EOF, possible read error, not enough
		bytes in source stream
	\item 2: write error
	\item 3: out of memory
re(lated to): Fcopy Fcopyto Fcopybuf Fcopyi
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

int Fcopyl(FILE * const fdst, FILE * const fsrc, dword iM(*) inM(Xlen, len))
{	byte *buf;
	size_t buflen, i;
	int err;
#ifdef _MICROC_
	dword len;
#endif

	DBG_ENTER("Fcopyl", Suppl_supplio)
#ifdef _MICROC_
	DBG_ARGUMENTS( (len? "len=0x%x%04x": "len=0", len->hi, len->lo) )
#else
	DBG_ARGUMENTS( ("len=%lu", len) )
#endif

	assert(fsrc);
	assert(fdst);

	if(!len iM(|| longtst(len) == 0))				/* Nothing to do */
		DBG_RETURN_I(0)

	if(Fmaxbuf(&buf, &buflen))
		DBG_RETURN_I( 3)				/* out of memory */

	chkHeap
#ifndef _MICROC_
	while(len > buflen) {
		if((err = Fcopybuf(fdst, fsrc, buf, buflen, &i)) != 0)
			goto errRet;
		len -= i;
	}
	chkHeap
	err = Fcopybuf(fdst, fsrc, buf, (size_t)len, &i);
#else
	longcpy(len, xlen);			/* len must be preserved */
	while(len.hi || len.lo > buflen) {
		if((err = Fcopybuf(fdst, fsrc, buf, buflen, &i)) != 0)
			goto errRet;
		longsubu(len, i);
	}
	chkHeap
	err = Fcopybuf(fdst, fsrc, buf, len.lo, &i);
#endif

errRet:
	chkHeap
	free(buf);
	chkHeap
	DBG_RETURN_I(err)
}
