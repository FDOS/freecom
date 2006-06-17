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

	FILE *Etmpfile(void)

	Open a scratch (temporary) file; the file is generated via the
	Ftmpnam() call and opened with the Fxopen() call.

	This file must be closed by the Fxclose() function in order to
	remove the file.

	On failure the program is terminated with:
		Cannot create temporary filename (if Ftmpnam() failed), or
		Cannot create temporary file (if Fxopen() failed)

ob(ject): Etmpfile
su(bsystem): error
ty(pe): S
sh(ort description): Create a temporary file
lo(ng description): Creates a new temporary file and opens it in "w+b" mode.
	The stream \em{must} be closed using the \tok{Fxclose()} function
	in order to actually remove the temporary file on close.\par
	The file is placed into the current temporary directory. There are no
	limitations according to change the current working directory or
	change the temporary directory in order to correctly remove the
	temporary file on its close.\par
	On failure the program is terminated with an appropriate error
	message.
pr(erequistes): 
re(lated to): 
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{FILE*} pointer of the created stream, never \tok{NULL}
fi(le): eetmpfil.c

*/

#include "initsupl.loc"

#include "supplio.loc"
#include "msgs.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

FILE *Etmpfile(void)
{	char *fnam;			/* name of the temporary file */
	FILE *f;

	DBG_ENTER("Etmpfile", Suppl_error)

	if((f = F_tmpfile(fnam = Etmpnam(0))) == 0)
		Esuppl_tmpOpen(fnam);

	chkHeap
	DBG_LEAVE( f)
}
