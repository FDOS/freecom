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

ob(ject): dfnglobinit
su(bsystem): dfn
ty(pe): H
sh(ort description): Creates a DFN_GLOB object
he(ader files): 
lo(ng description): Creates a DFN_GLOB object and roots its search
	data. The object is created in dynamic memory and must be
	deallocated by \tok{dfnglobfree()}.\par
	The \para{flags} parameter is a bitfield of the following defines:
	\list \tok{DFN_HIDDEN}: includes hidden files in search
	\list \tok{DFN_SYSTEM}: includes system files in search
	\list \tok{DFN_LABEL}: includes volume labels in search
	\list \tok{DFN_DIRECTORY}: includes volume labels in search
	\endlist
pr(erequistes): \tok(pattern != NULL)
va(lue): NULL: failure
	\list \tok{ENOMEM}: out of memory
		\item \tok{EINVAL}: Invalid argument, e.g. \tok{pattern == NULL}
	\endlist
	\item else: pointer to dynamically allocated object
re(lated to): dfnuglobinit dfnglobfree dfnglobread dfnglobfilename
se(condary subsystems): 
in(itialized by): 
wa(rning): The object's data should be access by the various \tok{dfnglob()}
	functions only.
bu(gs): 
fi(le): dfnsearc.c

ob(ject): dfnuglob
su(bsystem): dfn
ty(pe): H
sh(ort description): Creates a DFN_GLOB object
he(ader files): 
lo(ng description): As \tok{dfnglobinit()}, but supports UNC paths.
pr(erequistes): 
va(lue): NULL: failure
	\list \tok{ENOMEM}: out of memory
		\item \tok{EINVAL}: Invalid argument, e.g. \tok{pattern == NULL}
	\endlist
	\item else: pointer to dynamically allocated object
re(lated to): dfnuglobinit dfnglobfree dfnglobread dfnglobfilename
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
#endif
#include "dfn.loc"
#include "suppl.h"
#include "eno.loc"

#include "suppldbg.h"


#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

DFN_GLOB *dfnglobinit(const char * const pattern, unsigned flags)
{	const char *p;
	DFN_GLOB *g;

	DBG_ENTER("dfnglobinit", Suppl_dfn)
	DBG_ARGUMENTS( ("pattern=\"%s\", flags=0x%04x", pattern, flags) )

	if(!pattern || !*pattern) {
		eno_set(EINVAL);
		DBG_RETURN_P(0)
	}

	p = dfnfilename(pattern);
	if(!*p) {
		eno_set(ENOENT);
		DBG_RETURN_P(0)
	}

	if(0 == (g = eno_malloc(sizeof(DFN_GLOB) + strlen(p)))) {
		DBG_RETURN_P(0)
	}

	strcpy(g->dfn_pattern, p);		/* store the pattern */
	g->dfn_searchattr = flags;
	g->dfn_flags.dfn_rewind = 1;
	g->dfn_flags.dfn_eof = 1;
	g->dfn_data = 0;
	{	size_t len;
		size_t len2 = strlen(p);
		if(len2 < 12)
			len2 = 12;
		g->dfn_basediroffset = len = (size_t)(p - pattern);
		if(0 == (g->dfn_basedir
		 = eno_malloc(g->dfn_basedirlen = len + len2 + 1)))
		 	goto errRet;
		if(len)
			memcpy(g->dfn_basedir, pattern, len);
		g->dfn_basedir[len] = 0;
	}

	DBG_RETURN_P(g)

errRet:
	dfnglobfree(g);
	DBG_RETURN_P(0)
}
