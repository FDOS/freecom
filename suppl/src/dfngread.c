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

ob(ject): dfnglobread
su(bsystem): dfn
ty(pe): H
sh(ort description): Updates a DFN_GLOB object with the next match
he(ader files): 
lo(ng description): Updates a DFN_GLOB object with the next match.
pr(erequistes): \tok(glob != NULL)
va(lue): 0: failure
	\list \tok{ENOMEM}: out of memory
		\item \tok{EINVAL}: Invalid argument, e.g. \tok{pattern == NULL}
		\item \tok{ENOENT}: No further entry found
	\endlist
	\item else: an entry was found
re(lated to): dfnglobinit
se(condary subsystems): 
in(itialized by): 
wa(rning): The object's data should be access by the various \tok{dfnglob()}
	functions only.
bu(gs): 
fi(le): dfnsearc.c

ob(ject): dfnuread
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

#include "../../config.h"
#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#ifndef _PAC_NOCLIB_
#include "dir.loc"
#endif
#endif
#include "dfn.loc"
#include "dynstr.h"
#include "suppl.h"
#include "eno.loc"

#include "../../include/lfnfuncs.h"

#include "suppldbg.h"

	/* which bits to pass to DOS API? */
#define SEARCH_MASK 0xff

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dfnglobread(DFN_GLOB * const g)
{
	DBG_ENTER("dfnglobread", Suppl_dfn)
	DBG_ARGUMENTS( ("glob=\"%p\"", g) )

	if(!g) {
		eno_set(EINVAL);
		DBG_RETURN_I(0)
	}

	assert(g->dfn_basedir);
	if(g->dfn_flags.dfn_rewind || !g->dfn_data) {
		/* restart file search */
		if(0 == StrChg((char*)g->dfn_data, sizeof(struct ffblk)))
			DBG_RETURN_I(0)

		strcpy(g->dfn_basedir + g->dfn_basediroffset, g->dfn_pattern);
		g->dfn_flags.dfn_rewind = 0;
		g->dfn_flags.dfn_eof = 0;
		if(0 == findfirst(g->dfn_basedir
		 , g->dfn_data
		 , g->dfn_searchattr & SEARCH_MASK))	/* found an entry */
			DBG_RETURN_I(1)
		goto errRet;
	}

	/* continue previous search */
	if(g->dfn_flags.dfn_eof)		/* we hit the end of entries already */
		goto errRet2;

	if(findnext(g->dfn_data) == 0) {
#ifdef FEATURE_LONG_FILENAMES
        lfnfindclose(g->dfn_data);
#endif
		DBG_RETURN_I(1)
    }

errRet:
	g->dfn_flags.dfn_eof = 1;
errRet2:
	eno_set(ENOENT);
	DBG_RETURN_I(0)
}
