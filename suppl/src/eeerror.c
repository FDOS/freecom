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

	Eerror(const FILE * const fp)

	terminate program if the ferror() statue is true

ob(ject): Eerror
su(bsystem): error
ty(pe): S
sh(ort description): Check the error flag of a FILE stream
lo(ng description): Tests if the error flag of the given stream is
	set. If the error flag is true, the program is terminated with the
	message: "Cannot access file \"%s\""
pr(erequistes): 
re(lated to): Eopen Ecreate Eclose
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): none
fi(le): eeerror.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>
#include "msgs.h"
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void Eerror(const FILE * const fp)
{	DBG_ENTER("Eerror", Suppl_error)

	if(Ferror(fp))
		Esuppl_accessFile(Fxinfo(fp)->sea_fnam);

	chkHeap
	DBG_EXIT
}
