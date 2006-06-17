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

ob(ject): Ftmpfile
su(bsystem): supplio
ty(pe): 
sh(ort description): Open a scratch (temporary) file
lo(ng description): Creates a scratch (temporary & empty) file within the
	currently effective temporary directory. Its name is generated
	by the \tok{Ftmpnam()} function and it is created with the
	Fxopen() function, so it should be closed with Fxclose() in
	order to be removed.
	\par The file is opened in read&write binary mode.
pr(erequistes): 
va(lue): NULL: on failure\item else: pointer to stream / \tok{(FILE*)}
	pointer
re(lated to): Ftmpnam
se(condary subsystems): 
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

FILE *Ftmpfile(void)
{
	DBG_ENTER("Ftmpfile", Suppl_supplio)
	chkHeap
	DBG_RETURN_BP( F_tmpfile(Ftmpnam(0)))
}
