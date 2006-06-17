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

	void Efseek(FILE * const fp, int pos, int whence)

	As Fseek() with (int) offset, but terminate program on failure.

	Micro-C only

ob(ject): Efseek
su(bsystem): error
ty(pe): S
sh(ort description): Seek a FILE stream with an (int) offset
lo(ng description): Seeks a FILE streams with an (int) offset. On failure
	the program is terminated with the error message: "Cannot access
	file \"%s\""\par
	\em{Micro-C only}
pr(erequistes): 
re(lated to): Eopen Ecreate Fseek
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): none
fi(le): eefseek.c

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void Efseek(FILE * const fp, int pos, int whence)
{	dword ofs;

	DBG_ENTER("Efseek", Suppl_error)

	longseti(ofs, pos);
	Eseek(fp, ofs, whence);

	chkHeap
	DBG_EXIT
}
#endif
