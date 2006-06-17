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

ob(ject): dfnbakname
su(bsystem): dfn
ty(pe): H
sh(ort description): Return the backup-name of a file
he(ader files): 
lo(ng description): Constructs the filename of a backup of a given file.
pr(erequistes): 
va(lue): NULL: on out-of-memory conditions (ENOMEM)\item else: pointer to dynamically
	allocated buffer with the filename
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): The generated filename maybe identical with the passed in name, if
	that one was to "preferred" backup name. This is usually the case,
	if the passed-in name ends in ".BAK".
bu(gs): 
fi(le): dfnbnam.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#endif
#include "eno.loc"
#include "dfn.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfnbakname(const char * const fnam)
{	char *p, *q, *h;

	DBG_ENTER("dfnbakname", Suppl_dfn)

	assert(fnam);

	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	/* Allocate 5 bytes more to be absolutely sure that
		the ".BAK" can be appended */
	chkHeap
	if((p = eno_malloc(strlen(fnam) + 5)) == 0)
		DBG_RETURN_S( 0)

	chkHeap
	if((h = strrchr(q = dfnfilename(strcpy(p, fnam)), '.')) == 0)
		h = strchr(q, '\0');

	strcpy(h, ".BAK");

	chkHeap
	DBG_RETURN_S( p)
}
