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

ob(ject): dfnsplit
su(bsystem): dfn
ty(pe): H
sh(ort description): Split a given filename into its components
he(ader files): 
lo(ng description): Splits a given filename into its components, the drive,
	path, filename and extension. Each component may be missing, in which
	case \tok{NULL} is assigned to the particular result pointer.\par
	If one of the passed-in pointers is \tok{NULL}, this component is
	not passed back and, hence, not saved into dynamic memory.\newline
	On native FAT drives (no UNC filename), only the single drive letter,
	but not the colon is assigned to \tok{*\para{drive}}.\newline
	(\tok{dfnusplit()} only:) It is assumed that the UNC paths start
	with two components that have the meaning of "drive", its: \tok{"\\\\server\\share"}.
	\par Unless the macro \tok{SUPPORT_UNC_PATH} was defined prior \tok{#include}'ing
	the header file, this function does not support UNC paths.
pr(erequistes): fnam != NULL
va(lue): 0: on failure \item else: on success, \tok{*\para{drive}}, \tok{*\para{path}}, \tok{*\para{name}} and \tok{*\para{ext}} have been assigned with
	the dynamically allocated component
re(lated to): dfnusplit dfnmerge
se(condary subsystems): 
in(itialized by): 
wa(rning): The passed in filename is not preprocessed in any way, neither
	"." components are removed nor the path fully-qualified.\newline
	If the passed in filename is an UNC path and the UNC support is enabled,
	\tok{*\para{drive}} is not just a single letter!
bu(gs): 
fi(le): dfnsplit.c
ex:\example{|}
 | 		char *drive, *path, *name, *ext;
 |
 | 		if(!dfnsplit("d:\\Path1*./Path2.dgf.575\\....mOrk.vOm.Ork"
 | 		 , &drive, &path, &name, &ext))
 | 		 	abort("memory full");
 | 	-results in:
 | 		drive = "d"
 | 		path = "\\Path1*.\\Path2.dgf.575"
 | 		name = "....mOrk.vOm"
 | 		ext = "Ork"
 |
 | 		if(!dfnsplit("\\any.name", &drive, &path, &name, &ext))
 | 		 	abort("memory full");
 | 	-results in:
 | 		drive = NULL
 | 		path = "\\"
 | 		name = "any"
 | 		ext = "name"
 |
 | 		if(!dfnsplit("d:\\Path1*./Path2.dgf.575\\....mOrk.vOm.Ork"
 | 		 , &drive, NULL, &name, NULL))
 | 		 	abort("memory full");
 | 	-results in:
 | 		drive = "d"
 | 		path = <unchanged>
 | 		name = "....mOrk.vOm"
 | 		ext = <unchanged>

ob(ject): dfnusplit
su(bsystem): dfn
ty(pe): H
sh(ort description): Split a given filename into its components
he(ader files): 
lo(ng description): As \tok{dfnsplit()}, but supports UNC paths.
pr(erequistes): fnam != NULL
va(lue): 0: on failure \item else: on success, \tok{*\para{drive}}, \tok{*\para{path}}, \tok{*\para{name}} and \tok{*\para{ext}} have been assigned with
	the dynamically allocated component
re(lated to): dfnusplit dfnmerge
se(condary subsystems): 
in(itialized by): 
wa(rning): The passed in filename is not preprocessed in any way, neither
	"." components are removed nor the path fully-qualified.\newline
	If the passed in filename is an UNC path and the UNC support is enabled,
	\tok{*\para{drive}} is not just a single letter!
bu(gs): 
fi(le): dfnsplit.c

*/

#include "initsupl.loc"
#include "../../config.h"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#include <dos.h>

#endif
#include "dfn.loc"
#include "dynstr.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dfnsplit(const char * const fnam, char ** const dr, char ** const path
	, char ** const name, char ** const ext)
{	const char *p, *h;
	char *z;
	int ch;

#ifdef SUPPORT_UNC_PATH
	DBG_ENTER("dfnusplit", Suppl_dfn)
#else
	DBG_ENTER("dfnsplit", Suppl_dfn)
#endif

	assert(fnam);

	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

#define iff(var,value) if(var) *(var) = (value)
	iff(dr, 0);
	iff(path, 0);
	iff(name, 0);
	iff(ext, 0);
	if((p = fnam) == 0) DBG_RETURN_I( !NUL)

	chkHeap
#ifdef SUPPORT_UNC_PATH
	if(isUNCpath(p)) {					/* UNC drive */
		h = UNCpath(p);					/* h := end of UNC drive */
		if(dr) {
			chkHeap
#ifdef FARDATA
			assert((h - p) < 0x10000l);
#endif
			if((*dr = *h? StrLeft(p, (size_t)(h - p)): eno_strdup(p)) == 0)
				DBG_RETURN_I( NUL)
			DBG_RETURN_VALUES( ("dr=\"%s\"", *dr) )
			if(!*h) {		/* no further portion, but UNC paths are always
								fully-qualified -> path := root_directory */
				DBG_RETURN_BI((*path = eno_strdup("")) != 0)
			}
			p = h;			/* points to the '\\' marking the root direc */
		}
	}
	else			/* check for non-UNC drive spec */
#endif
	if(p[1] == ':') {	/* drive spec */
		chkHeap
		if(dr) {
			if((*dr = StrChar(*p)) == 0)
				DBG_RETURN_I( NUL)
		}
		p += 2;
	}

	chkHeap
	if(*p) {	/* there are characters left */
	/* determine last path component */
		h = dfnfilename(p);
		chkHeap
		if(h > p) { 	/* there is a backslash => a path */
			if(path) {
				if((*path = z = eno_malloc(1 + (size_t)(h - p))) == 0)
					DBG_RETURN_I( NUL)
				/* because malloc() before squeeze => if multiple '\\'s 
					some memory is wasted */
				/* while copying squeeze multiple '\\'s */
				/* strip trailing backslash */
				chkHeap
				do {
					if(dfndelim(ch = *p++)) {	/* squeeze */
						ch = '\\';
						while(dfndelim(*p++));
						--p;
					}
					if(p >= h) break;
					*z++ = ch;
				} while(1);
				if(z == *path)		/* root directory */
					*z++ = '\\';
				*z = NUL;
				chkHeap
				DBG_RETURN_VALUES( ("path=\"%s\"", *path) )
			}
			p = h;	/* skip path */
		}
	/* check for special names starting with a dot '.' */
		if(*(h = p) == '.')  while(*++h == '.');
		if((h = strrchr(h, '.')) == 0) 	/* no extension at all */
		 	DBG_RETURN_BI( name? (*name = eno_strdup(p)) != 0: !NUL)
	/* copy the name */
		chkHeap
		if(name) {
			if((*name = StrLeft(p, (size_t)(h - p))) == 0)
				DBG_RETURN_I( NUL)
			chkHeap
			DBG_RETURN_VALUES( ("name=\"%s\"", *name) )
		}
	/* copy the extension */
		if(ext) {
			if((*ext = eno_strdup(h + 1)) == 0)
				DBG_RETURN_I(NUL)
			chkHeap
			DBG_RETURN_VALUES( ("ext=\"%s\"", *ext) )
		}
	}
	chkHeap
	DBG_RETURN_I( !NUL)
}

