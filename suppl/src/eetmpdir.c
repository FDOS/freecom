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

	char *Etmpdir(void)

	Return the absolute path of the current temporary directory.
	The path already contains a trailing backslash.
	The returned string is located in the heap and must be
	free()'ed by the user.

	On failure the program is terminated with an appropriated error
	message.

	Return:
		pointer to generated path name


ob(ject): Etmpdir
su(bsystem): error
ty(pe): H
sh(ort description): Return the path of the current temporary directory
lo(ng description): Returns the path of the current temporary directory,
 where the user should create all temporary files in. The returned string
 is duplicated into the heap and must be deallocated by the user. The string
 already contains a trailing backslash.\par
 On failure the program is terminated with an appropriate error message.
pr(erequistes): 
re(lated to): Etmpnam
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): pointer to dynamically allocated buffer, never \tok{NULL}
fi(le): eetmpdir.c

*/

#include "initsupl.loc"

#include <stdio.h>

#include "supplio.loc"
#include "dfn.h"
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *Etmpdir(void)
{	char *p;

	DBG_ENTER("Etmpdir", Suppl_error)

	remove(p = Etmpnam(0));		/* Ftmpnam() creates the file already */
	chkHeap
	DBG_RETURN_BS( Estrdupe(p, dfnfilename(p)))
}
