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

ob(ject): dfnexpand
su(bsystem): dfn
ty(pe): H
sh(ort description): Expand a path to a fully-qualified one
he(ader files): 
lo(ng description): Generates the fully-qualified path of a given file
	spec.\par
	If \tok{*fnam == '\0'} meaning that \para{fnam} is an empty
	string, \tok{"."} is used.\newline
	\para{path} is \em{not} expanded itself, meaning\example{|}
	|	dfnexpand("x:foo/bar", "humpf/er")
	is expanded to \tok{"x:humpf/er\\FOO\\BAR"}. Alternatively one
	could say that \para{fnam} is preprocessed by \tok{dfnsqueeze()},
	but \para{path} is not.\newline
	If \para{path} does not contain a drive spec, it is assumed that it matches
	the one of \para{fnam}; see above, too.\newline
	If \tok{\para{path} == NULL}, the current working path is used.
pr(erequistes): fnam != NULL
va(lue): NULL: on error
	\list \tok{ENOMEM}: out of memory
	\item \tok{ENODRV}: Invalid drive while getting cwd
	\item \tok{ERANGE}: Buffer of getdcwd() too small
	\item \tok{ENODEV}: Failed to fetch cwd from drive
	\endlist
	\item else: pointer to dynamically allocated buffer
	with the generated path
re(lated to): dfnuexpand dfnsqueeze dfnpath dfnmerge dfnsplit
se(condary subsystems): 
in(itialized by): 
wa(rning): If the macro \tok{SUPPORT_UNC_PATH} is \em{not} defined
	prior \tok{#include}'ing the header file, this function does \em{not}
	support UNC filenames.
bu(gs): 
fi(le): dfnexpan.c

ob(ject): dfnuexpand
su(bsystem): dfn
ty(pe): H
sy(nopsis): 
sh(ort description): Expand a filename to a fully-qualified one
he(ader files): 
lo(ng description): Expands a filename to a fully-qualified one,
	just as \tok{dfnexpand()}, but supports UNC filenames.
pr(erequistes): fnam != NULL
va(lue): NULL: on error\item else: pointer to dynamically allocated buffer
	with the generated path
re(lated to): dfnexpand dfnsqueeze dfnpath dfnmerge dfnsplit
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

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#endif
#include "dfn.loc"
#include "dynstr.h"
#include "suppl.h"
#include "dir.loc"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfnexpand(const char * const fnam, char * const path)
{	char *h, *p;				/* intermediate pointers */
	char *dr, *pa, *na, *ex;	/* filename components */
	char pathDr, *pathPa;		/* drive & path of 'path' */
	char *dynPath;

#ifdef SUPPORT_UNC_PATH
	DBG_ENTER("dfnuexpand", Suppl_dfn)
#else
	DBG_ENTER("dfnexpand", Suppl_dfn)
#endif

	assert(fnam);

	DBG_ARGUMENTS( ("fnam=\"%s\", path=\"%s\"", fnam, path) )

	chkHeap
	if((h = dfnsqueeze(fnam)) == 0) 
		DBG_RETURN_S( 0)

#ifdef SUPPORT_UNC_PATH
	if(isUNCpath(h)) {			/* UNC paths are always fully-qualified */
		/* check if the trailing '\\' is present to mark the root direc */
		DBG_RETURN_BS((*UNCpath(h) != '\\')? StrAppChr(h, '\\') : h)
	}
#endif

	chkHeap
	if(!*h || h[1] != ':' || h[2] != '\\') {
	/* the spec is not fully-qualified or completely empty */
		pathDr = 0;
		dynPath = 0;
		if((pathPa = path) != 0 && *pathPa) {
			if(pathPa[1] == ':') {	/* path[] has drive spec */
				pathDr = *path;
				if(!*(pathPa += 2)) {
					pathPa = 0;
					goto noPath;
				}
			}
			if(dfndelim(*pathPa) && !pathPa[1])
				++pathPa;		/* Trans "/" || "\\" --> "" */
noPath:;
		}
		chkHeap
		if(dfnsplit(h, &dr, &pa, &na, &ex)) {
			StrFree(h);
			if(dr) {				/* drive specified */
				if(pathDr && toFUpper(pathDr) != *dr)
					/* The specified path is for a different drive */
					pathPa = 0;
			}
			else {					/* drive spec missing */
				if((dr = StrChar(pathDr? pathDr: 'A' + getdisk())) == 0)
					goto errRet;
			}

			if(!pa || (*pa != '\\' && *pa != NUL)) {
			/* no path or a relative one */
				if(!pathPa) {				/* path has no path spec in it */
					if((dynPath = dfnpath(*dr)) == 0)
						goto errRet;
					pathPa = dynPath + 2;
				}

				if((p = dfnmerge(0, 0, pathPa, pa, 0)) == 0)
					goto errRet;
 				StrRepl(pa, p);
			}
			h = dfnmerge(0, dr, pa, na, ex);
		} else
			StrFree(h);

errRet:
		chkHeap
		free(dr);
		free(pa);
		free(na);
		free(ex);
		free(dynPath);
	}

	chkHeap
	DBG_RETURN_S( h)
}
