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

ob(ject): Esetpos
su(bsystem): error
ty(pe): S
sh(ort description): Set the position within a FILE stream
lo(ng description): Restores the position of a FILE stream.
	This function uses a portable way to save/restore the position
	of a stream and is even supported by Micro-C. The position
	is stored into the \tok{fpos_t} structure passed by reference
	into this function.\par
	On failure
	the program is terminated with the error message: "Cannot access
	file \"%s\""
pr(erequistes): 
re(lated to): Eopen Ecreate Fsetpos Egetpos Eseek
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): none
fi(le): eesetpos.c

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


void Esetpos(FILE * const fp, const fpos_t * const pos)
{	DBG_ENTER("Esetpos", Suppl_error)

	Fsetpos(fp, pos);
	Eerror(fp);

	DBG_EXIT
}
