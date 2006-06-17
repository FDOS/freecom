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

	void Exclose(FILE *fp)

	As Fxclose(), but terminate program on failure.

ob(ject): Exclose
su(bsystem): error
ty(pe): S
sh(ort description): Close a stream
lo(ng description): Closes a stream via the \tok{Fxclose()} function.
	\par On failure the program is terminated with an appropriate
	error message.
pr(erequistes): 
re(lated to): 
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): none
fi(le): eexclose.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void Exclose(FILE *fp)
{	DBG_ENTER("Exclose", Suppl_error)

	Eerror(fp);
	Fxclose(fp);

	chkHeap
	DBG_EXIT
}
