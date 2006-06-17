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

	FILE *Ecreate(const char * const fnam, const char * const mode);

	Open the file as Fopen(), but terminate the program on failure

ob(ject): Ecreate
su(bsystem): error
ty(pe): S
sh(ort description): Create a new stream and open a FILE pointer
lo(ng description): Wrapps \tok{fopen()}. On failure the program is
	terminated with the error message: "Cannot create file \"%s\""
pr(erequistes): 
re(lated to): Eopen Eclose Eread Ewrite Fopen Fxopen
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): a valid FILE pointer
fi(le): eecreate.c

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

FILE *Ecreate(const char * const fnam, const char * const mode)
{	FILE *f;

	DBG_ENTER("Ecreate", Suppl_error)

	if((f = Fopen(fnam, mode)) == 0)
		Esuppl_createFile(fnam);

	chkHeap
	return f;
}
