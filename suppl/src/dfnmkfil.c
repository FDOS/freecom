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

ob(ject): dfnmkfile
su(bsystem): dfn
ty(pe): 
sh(ort description): Atomic \em{file create}-operation
he(ader files): 
lo(ng description): Creates a given file, but fails if the file already exists
	or the path to file does not exist already. This function should be
	used for semaphore-type operations, as there is no way to do so with
	other than the \tok{<io.h>} file operations.\par
	On success this function creates a file with the given name with no special
	file attribute set and zero length.
pr(erequistes): 
va(lue): 0: on success\item 0x50: file already exists (which is an OS error code, too)\item else: OS error code
re(lated to): dfnmktmp
se(condary subsystems): 
in(itialized by): 
wa(rning): 
fi(le): dfnmkfil.c

*/


#include <io.h>
#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include "dfn.loc"
#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dfnmkfile(const char * const fnam)
{	struct REGPACK r;
	int rv;

	DBG_ENTER("dfnmkfile", Suppl_dfn)
	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	assert(fnam);

	r.r_ds = FP_SEG(fnam);
	r.r_dx = FP_OFF(fnam);
	r.r_cx = 0;
	r.r_ax = 0x5b00;					/* Create _new_ file */
    intr( 0x21, &r );

	if((rv = ( r.r_flags & 1 ) ? r.r_ax : 0) != 0)		/* failure */
		DBG_RETURN_I( rv)

	/* Because the file descriptor was not opened via the <io.h>
		interface it could be problematic to close the descriptor
		via close() */

#if 0
	r.x.bx = r.x.ax;
	r.x.ax = 0x3e00;					/* Close file descriptor */
	int86(0x21, &r, &r);
#else
    _close( r.r_ax );
#endif

	DBG_RETURN_I( 0)
}
