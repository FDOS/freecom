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

ob(ject): Fcopyto
su(bsystem): supplio
ty(pe): 
sh(ort description): Copy a portion of a file into another one
lo(ng description): Copies stream \para{fsrc} beginning with the current
	position upto, but excluding the specified position \para{pos} into
	stream \para{fdst} beginning at the current position.\par
	If the destination stream \para{fdst} is positioned before
	the end of the physical file, its contents is overwritten, but
	not truncated, if it has more bytes left than are actually
	copied into.\par
pr(erequistes): fsrc != NULL && fdst != NULL && pos != NULL
va(lue): 0: on success
	\item 1: \para{fsrc} not at EOF, possible read error, not enough
		bytes in source stream, general access error \para{fsrc}
	\item 2: write error
	\item 3: out of memory
re(lated to): Fcopy Fcopybuf Fcopyi Fcopyl
se(condary subsystems): 
in(itialized by): 
wa(rning): The specified position must be a valid one returned by
	\tok{FFgetpos()} or assoc., because if the stream is opened in
	text mode and a special text handling is active, such as multi-byte
	characters, the result is unpredictable otherwise.
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int Fcopyto(FILE * const fdst, FILE * const fsrc, const fpos_t * const pos)
{	fpos_t cpos;
	dword dw_cpos, dw_topos;

	DBG_ENTER("Fcopyto", Suppl_supplio)

	assert(fdst);
	assert(fsrc);
	assert(pos);

	/* Because the (fpos_t) data type is a blackbox type completely,
		one would need to copy the stream character-by-character in
		order to ensure that the pos is reached.

		SUPPL assumes that there is a way to calculate the offset
		of data type (dword) from the (fpos_t) values, which is
		hidden into the Fpos2dword() function.
	*/
	if(FFgetpos(fsrc, cpos))			/* get current position */
		DBG_RETURN_I( 1)			/* general access error */

	Fpos2dword(cpos, dw_cpos);
	Fpos2dword(pos, dw_topos);

	if(longcmp1(dw_topos, dw_cpos) <= 0)
		DBG_RETURN_I( 0)	/* current position already behind end position */

	/* Amount of bytes to be copied */
	longsub(dw_topos, dw_cpos);

	DBG_RETURN_BI( Fcopyl(fdst, fsrc, dw_topos))
}
