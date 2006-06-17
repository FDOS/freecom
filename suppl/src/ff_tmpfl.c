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


ob(ject): F_tmpfile
su(bsystem): supplio
ty(pe): L
sy(nopsis): 
sh(ort description): Open a scratch (temporary) file
he(ader files): 
lo(ng description): 
	Opens a scratch (temporary) file via the the \tok{Fxopen()} call
	in overwrite, binary, read & write mode.
	This file must be closed by the \tok{Fxclose()} function in order to
	remove the file.\par
	If to open the file fails, a file named "fnam" is removed, if it
	already exists before calling this function. This is necessary, because
	this function is intended to be the back-end of \tok{Ftmpnam()}, which
	already creates the temporary file.
pr(erequistes): 
va(lue): 
re(lated to): Ftmpnam Ftmpfile
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

#include "supplio.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

FILE *F_tmpfile(const char * const fnam)
{	Suppl_ExtAttr *a;

	DBG_ENTER("F_tmpfile", Suppl_supplio)

	if(!fnam)
		DBG_RETURN_P( 0)

	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	if((a = F_xopen(fnam, "w+b")) == 0) {
		remove(fnam);			/* Ftmpnam() creates the file already */
		DBG_RETURN_P( 0)
	}

	a->sea_istmp = 1;		/* mark it a temporary file */

	DBG_RETURN_P( a->sea_fp)
}
