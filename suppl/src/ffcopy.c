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

ob(ject): Fcopy
su(bsystem): supplio
ty(pe): 
sy(nopsis): 
sh(ort description): Copy any remaining bytes from one file to another
he(ader files): 
lo(ng description): Copies any remaining bytes from stream \para{fsrc}
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
	\item 1: \para{fsrc} not at EOF, possible read error
	\item 2: write error
	\item 3: out of memory
re(lated to): Fcopybuf Fcopyto
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
	Read errors are ignored, but the EOF-condition is tested on return.
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

int Fcopy(FILE * const fdst, FILE * const fsrc)
{	byte *buf;
	size_t len, i;
	int err;

	DBG_ENTER("Fcopy", Suppl_supplio)

	assert(fsrc);
	assert(fdst);

	chkHeap
	if(Fmaxbuf(&buf, &len))
		DBG_RETURN_I( 3)				/* out of memory */

	chkHeap
	while((err = Fcopybuf(fdst, fsrc, buf, len, &i)) == 0);

	chkHeap
	free(buf);
		/*	err == 0 --> impossible
			err == 1 --> read error, this is OK, if read had hit EOF
		*/
	chkHeap
	DBG_RETURN_BI(err < 2? Feof(fsrc) == 0: err)
}
