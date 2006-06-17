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

	char *Ftmpdir(void)

	Return the absolute path of the current temporary directory.
	The path already contains a trailing backslash.
	The returned string is located in the heap and must be
	free()'ed by the user.

	Return:
		NULL: no writeble directory found
		else: pointer to generated path name


ob(ject): Ftmpdir
su(bsystem): supplio
ty(pe): 
sh(ort description): Return the absolute path of the temporary directory
lo(ng description): Returns the absolute path of the currently effective
	temporary directory. The returned directory already contains one
	trailing backslash and is always fully-qualified.
	\par The actual algorithm to find the directory is explained
	at \tok{Ftmpnam()}.
pr(erequistes): 
va(lue): \tok{NULL}: on failure\item else: pointer to the dynamically
	allocated buffer holding the temporary directory
re(lated to): Ftmpnam
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

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

char *Ftmpdir(void)
{	char *p;

	DBG_ENTER("Ftmpdir", Suppl_supplio)

	if((p = Ftmpnam(0)) == 0)
		DBG_RETURN_S( 0)

	remove(p);			/* Ftmpnam() creates the file already */
	chkHeap
	DBG_RETURN_BS( StrDupe(p, dfnfilename(p)))
}
