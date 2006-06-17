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

	Suppl_ExtAttr *F_xopen(char *fnam, char *mode)

	Open a file as Fopen(); create extended attributes and chain them
	into the list of extended attributes.

	If the returned FILE* pointer is closed by fclose() rather than
	Fxclose(), the extended attributes remain in memory an could
	return invalid information, if used, if the compiler re-uses
	FILE* pointers, e.g. Borland C.

ob(ject): F_xopen
su(bsystem): supplio
ty(pe): L
sy(nopsis): 
sh(ort description): Open a file in extended mode
he(ader files): 
lo(ng description): Opens a file as \tok{Fopen()}, but preserve extended
	attributes about the file, especially the fully-qualified filename.\par
	This function is actually the back-end function of \tok{Fxopen()}.
pr(erequistes): 
va(lue): \tok{NULL}: on failure
	\item else: pointer to extended stream buffer
re(lated to): Fopen Fxopen Fxclose
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
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
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

Suppl_ExtAttr *F_xopen(const char * const fnam, const char * const mode)
{	Suppl_ExtAttr *p;

	DBG_ENTER("F_xopen", Suppl_supplio)

	assert(fnam);
	assert(mode);

	DBG_ARGUMENTS( ("fnam=\"%s\", mode=\"%s\"", fnam, mode) )

	chkHeap
	if((p = eno_malloc(sizeof(Suppl_ExtAttr))) == 0) {
		DBG_STRING("Failed to allocate extended structure")
		goto err1;
	}

	chkHeap
	if((p->sea_fnam = dfnexpand(fnam, 0)) == 0) {
		DBG_STRING("Failed to dfnexpand() filename")
		goto err2;
	}

	chkHeap
	if((p->sea_fp = Fyopen(p->sea_fnam, mode)) != 0) {
		p->sea_nxt = suppl_dfltExtAttr.sea_nxt;
		suppl_dfltExtAttr.sea_nxt = p;
		p->sea_istmp = 0;
		DBG_RETURN_P( p)
	}

	free(p->sea_fnam);
err2:
	free(p);
err1:
	chkHeap
	DBG_RETURN_P( 0)
}
