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

ob(ject): dfnglobfree
su(bsystem): dfn
ty(pe): H
sh(ort description): Deallocate a DFN_GLOB object
he(ader files): 
lo(ng description): Deallocate a DFN_GLOB object.
re(lated to): dfnglobinit dfnuglobfree
se(condary subsystems): 
in(itialized by): 
wa(rning):
bu(gs): 
fi(le): dfnsearc.c

ob(ject): dfnuglob
su(bsystem): dfn
ty(pe): H
sh(ort description): Deallocate a DFN_GLOB object
he(ader files): 
lo(ng description): As \tok{dfnglobfree()}, but supports UNC paths.
pr(erequistes): 
re(lated to): dfnuglobinit dfnglobfree
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnsearc.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include "dfn.loc"
#include "suppl.h"

#include "suppldbg.h"


#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void dfnglobfree(DFN_GLOB *g)
{
	DBG_ENTER("dfnglobfree", Suppl_dfn)
	DBG_ARGUMENTS( ("glob=\"%p\"", g) )

	if(g) {
		free(g->dfn_basedir);
		free(g->dfn_data);
		free(g);
	}

	DBG_EXIT
}
