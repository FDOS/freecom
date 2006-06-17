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

ob(ject): dfnglobfilename
su(bsystem): dfn
ty(pe): H
sh(ort description): Return the filename of the matched entry
he(ader files): 
lo(ng description): Returns the filename without any path component
	of the matched entry within \tok{glob}. The value is overwritten
	the next time \tok{dfnglobfree()} or \tok{dfnglobread()} is called.
pr(erequistes): \tok(glob != NULL)
va(lue): 0: failure
	\list \tok{ENOMEM}: out of memory
		\item \tok{EINVAL}: Invalid argument, e.g. \tok{pattern == NULL}
		\item \tok{ENOENT}: No entry matched
	\endlist
	\item else: pointer to a static buffer with the filename
re(lated to): dfnglobinit
se(condary subsystems): 
in(itialized by): 
wa(rning):
bu(gs): 
fi(le): dfnsearc.c

ob(ject): dfnufilename
su(bsystem): dfn
ty(pe): H
sh(ort description): Updates a DFN_GLOB object with the next match
he(ader files): 
lo(ng description): As \tok{dfnglobread()}, but supports UNC paths.
pr(erequistes): 
va(lue): 0: failure
	\list \tok{ENOMEM}: out of memory
		\item \tok{EINVAL}: Invalid argument, e.g. \tok{pattern == NULL}
		\item \tok{ENOENT}: No further entry found
	\endlist
	\item else: an entry was found
re(lated to): dfnuglobinit dfnglobread
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnsearc.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#ifndef _PAC_NOCLIB_
#include "dir.loc"
#endif
#endif
#include "dfn.loc"
#include "suppl.h"
#include "eno.loc"

#include "suppldbg.h"

	/* which bits to pass to DOS API? */
#define SEARCH_MASK 0xff

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfnglobfilename(DFN_GLOB * const g)
{
	struct ffblk *ff;

	DBG_ENTER("dfnglobfilename", Suppl_dfn)
	DBG_ARGUMENTS( ("glob=\"%p\"", g) )

	if(!g) {
		eno_set(EINVAL);
		DBG_RETURN_S(0)
	}

	assert(g->dfn_basedir);
	if(g->dfn_flags.dfn_eof) {
		eno_set(ENOENT);
		DBG_RETURN_S(0)
	}

	assert(g->dfn_data);
	ff = g->dfn_data;

	DBG_RETURN_S(ff->ff_name)
}
