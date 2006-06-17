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

	FILE *Fxopen(char *fnam, char *mode)

	Open a file as Fopen(); create extended attributes and chain them
	into the list of extended attributes.

	If the returned FILE* pointer is closed by fclose() rather than
	Fxclose(), the extended attributes remain in memory an could
	return invalid information, if used, if the compiler re-uses
	FILE* pointers, e.g. Borland C.

ob(ject): Fxopen
su(bsystem): supplio
ty(pe): 
sh(ort description): Open a file and keep extended attributes
lo(ng description): Opens a file like \tok{Fopen()}, but creates
	an extended attribute structure associated to the stream. The
	attributes are preset with:
	\enum \tok{fp}: opened stream / \tok{(FILE*)} pointer
	\enum \tok{fnam}: fully-qualified filename
	\enum \tok{istmp}: \tok{0} -- not a temorary file
	\endlist
pr(erequistes): fnam != NULL && mode != NULL
va(lue): NULL: on failure\item else: stream / \tok{(FILE*)} pointer
re(lated to): Fxclose Fopen
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#include <assert.h>
#ifndef _MICROC_
#include <stdlib.h>
#endif
#include "supplio.loc"
#include "dfn.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

FILE *Fxopen(const char * const fnam, const char * const mode)
{	Suppl_ExtAttr *p;

	assert(fnam);
	assert(mode);

	return (p = F_xopen(fnam, mode)) == 0? 0: p->sea_fp;
}
