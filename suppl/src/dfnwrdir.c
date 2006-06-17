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

ob(ject): dfnwrdir
su(bsystem): dfn
ty(pe): H
sh(ort description): Probe if a directory is writtable
he(ader files): 
lo(ng description): Probes if one can create new files in a directory.\par
	If \para{fnam} ends in a colon ':', the current working directory
	of that drive is tested and not the root directory.\par
	This function actively tries to create a new file in this directory,
	because in DOS there is no way to really passively check if a
	directory is writable or not.
pr(erequistes): 
va(lue): 0: on success\item else: on failure, maybe out-of-memory or
	directory is not writeable; ENOMEM, OS error code
re(lated to): dfnstat
se(condary subsystems): 
in(itialized by): 
wa(rning): People with WORM drives don't like functions like that very much,
	probing around on such drives.
bu(gs): 
fi(le): dfnwrdir.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <stdio.h>
#include "dfn.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dfnwrdir(const char * const fnam)
{	char *p;			/* temporary filename */

	DBG_ENTER("dfnwrdir", Suppl_dfn)
	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	assert(fnam);

	chkHeap
	if((p = dfnmktmp(fnam, 0)) == 0)		/* failed --> error */
		DBG_RETURN_I( 1)

	remove(p);			/* dfnmktmp() creates the file */
	free(p);

	chkHeap
	DBG_RETURN_I( 0)
}
