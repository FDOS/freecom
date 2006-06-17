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

	char *Etmpnam(char *ext)

	Create a filename of a temporary file with the extension 'ext'.
	The file is tried to be created in:
		%TEMP%
		%TMP%
		%TEMPDIR%
		%TMPDIR%
		%CURRENT_DRIVE%\TEMP
		%CURRENT_DRIVE%\TMP
		%BOOT_DRIVE%\TEMP
		%BOOT_DRIVE%\TMP
		.\
		%BOOT_DRIVE%\

	On failure the program is terminated with the error message
	"Cannot create temporary filename"

	Return:
		pointer to generated filename, which is overwritten the
			next time this function is called or Ftmpnam().


ob(ject): Etmpnam
su(bsystem): error
ty(pe): H
sh(ort description): Create the name of a temporary file
lo(ng description): Creates the name of a temporary file with a given
	extension in the current temporary directory. (See \tok{Ftmpnam}
	for details how the name of the temporary directory is determined.)\par
	On failure the program is terminated with an appropriate error message.
pr(erequistes): 
re(lated to): 
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): pointer to filename located within a static buffer that is
	overwritten the next time \tok{Etmpnam()} or \tok{Ftmpnam()}
	is called, never \tok{NULL}
fi(le): eetmpnam.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif

#include "supplio.loc"
#include "msgs.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *Etmpnam(const char * const ext)
{	char *p;

	DBG_ENTER("Etmpnam", Suppl_error)

	if((p = Ftmpnam(ext)) == 0)
		Esuppl_creatTmpFnam();

	chkHeap
	DBG_RETURN_S( p)
}
