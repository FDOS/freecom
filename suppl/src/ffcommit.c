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

ob(ject): fcommit
su(bsystem): supplio
ty(pe): 
sy(nopsis): 
sh(ort description): Flush a stream
lo(ng description): Flushes stream, its data and directory information,
	so that if it asynchroneously terminates, every byte written so far
	really is in the file.
pr(erequistes): 
va(lue): 0: on success
	\item else: on failure
re(lated to): 
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

#include "supplio.h"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int fcommit(FILE *fp)
{
	DBG_ENTER("fcommit", "supplio")

	/* To commit the file
		first we make sure the data chached by the C lib is written to
			the system, and
		second we tweak the system to flush the information to the
			disk by "COMMIT" the file (DOS-68).
	*/
	if(fp) {
		Fflush(fp);
		DBG_RETURN_BI( commit(Fileno(fp)))
	}
	DBG_RETURN_I( 0)
}
