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

	void Eclose(FILE *fp)

	Close the file as Fclose(), but terminate the program on failure

ob(ject): Eclose
su(bsystem): error
ty(pe): S
sh(ort description): Close a FILE stream
lo(ng description): Closes a FILE stream, but tests if the error flag is
	set. If the error flag is true, the program is terminated with the
	message: "Cannot access file \"%s\""
pr(erequistes): 
re(lated to): Eopen Ecreate Eerror Fclose Fxclose
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): none
fi(le): eeclose.c

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

void Eclose(FILE *fp)
{	
	DBG_ENTER("Eclose", Suppl_error)

	Eerror(fp);
	fclose(fp);

	chkHeap
	DBG_EXIT
}
