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

ob(ject): dfnglobfullfilename
su(bsystem): dfn
ty(pe): H
sh(ort description): Return the full filename of the matched entry
he(ader files): 
lo(ng description): Returns the filename with the same path component
	prepended
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

ob(ject): dfnufullfilename
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

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfnglobfullfilename(DFN_GLOB * const g)
{
	struct ffblk *ff;
	int len;

	DBG_ENTER("dfnglobfullfilename", Suppl_dfn)
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

	assert(g->dfn_basedirlen > g->dfn_basediroffset);
	if((len = strlen(ff->ff_name))
	 >= g->dfn_basedirlen - g->dfn_basediroffset) {
			/* grow the buffer */
		char *p;

		if(0 == (p = eno_realloc(g->dfn_basedir
		 , g->dfn_basedirlen = g->dfn_basediroffset + len + 1)))
			DBG_RETURN_S(0)

		g->dfn_basedir = p;
	}

	memcpy(g->dfn_basedir + g->dfn_basediroffset, ff->ff_name, len + 1);
	DBG_RETURN_S(g->dfn_basedir)
}
