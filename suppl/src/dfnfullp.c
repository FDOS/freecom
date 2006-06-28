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

ob(ject): dfnfullpath
su(bsystem): dfn
ty(pe): H
sh(ort description): Expand a path to a minimal fully-qualified one
he(ader files): 
lo(ng description): Generates the fully-qualified path of a given file
	spec and removes any ".." and "." components.\par
	Before processing the passed in path is passed through
	\tok{dfnexpand(path, NULL)}.\newline
	".." that chdir above the root directory are ignored, which is
	very Unix-like, but is not supported by all DOS applications the
	same way.
pr(erequistes): fnam != NULL
va(lue): NULL: on error\item else: pointer to dynamically allocated buffer
	with the generated path; the path may end with a backslash, which means
	that the passed-in filename must match a directory
re(lated to): dfnuexpand dfnsqueeze dfnpath dfnmerge dfnsplit
se(condary subsystems): 
in(itialized by): 
wa(rning): If the macro \tok{SUPPORT_UNC_PATH} is \em{not} defined
	prior \tok{#include}'ing the header file, this function does \em{not}
	support UNC filenames.
bu(gs): 
fi(le): dfnexpan.c

ob(ject): dfnufullpath
su(bsystem): dfn
ty(pe): H
sy(nopsis): 
sh(ort description): Expand a filename to a fully-qualified one
he(ader files): 
lo(ng description): Expands a filename to a fully-qualified one,
	just as \tok{dfnfullpath()}, but supports UNC filenames.
pr(erequistes): fnam != NULL
va(lue): NULL: on error\item else: pointer to dynamically allocated buffer
	with the generated path; the path may end with a backslash, which means
	that the passed-in filename must match a directory
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

#include "../../config.h"
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

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfnfullpath(const char * const fnam)
{	char *fullpath;
	char *paths;
	char *q;				/* intermediate pointers */
	char *nxtDelim;
	int cnt;

#ifdef OS_WIN32
#ifdef SUPPORT_UNC_PATH
	DBG_ENTER("dfnufullpath2", Suppl_dfn)
#else
	DBG_ENTER("dfnfullpath2", Suppl_dfn)
#endif
#else
#ifdef SUPPORT_UNC_PATH
	DBG_ENTER("dfnufullpath", Suppl_dfn)
#else
	DBG_ENTER("dfnfullpath", Suppl_dfn)
#endif
#endif

	assert(fnam);

	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	chkHeap
	if((fullpath = dfnexpand(fnam, 0)) == 0)
		DBG_RETURN_S( 0)

	assert(*fullpath);

#ifdef SUPPORT_UNC_PATH
	if(isUNCpath(fullpath)) paths = UNCpath(fullpath);
	else
#endif
	{
		assert(*fullpath && fullpath[1] == ':');
		paths = &fullpath[2];
	}

	chkHeap
	assert(*paths == '\\');
	q = paths;
	while(*q == '\\' && *++q) {
		/* Check for special directories */
		if((nxtDelim = strchr(q, '\\')) == 0)
			nxtDelim = strchr(q, '\0');
		if((cnt = strspn(q, ".")) == nxtDelim - q
#ifndef FEATURE_LONG_FILENAMES
		 && cnt < 3		/* DOS limits to "." and ".." */
#endif
		) {
			/* all dots --> special directory */
			/* Relocate "q" (cnt-1) path components to the left */
			while(--q > paths && --cnt) {
				/* search next '\' to the left */
				while(*--q != '\\');
				++q;
			}
			if(*nxtDelim) {
				/* some components left, move them up */
				/* Note: *nxtDelim == *q == '\\', but by copying over
					this single character one needs not implement a
					special handling if nxtDelim[1] == '\0' */
				memmove(q, nxtDelim, strlen(nxtDelim) + 1);
			} else {
				/* this was the last component, but because this was
					a special directory, the information must be preserved
					that it is a directory --> append trailing backslash */
				q[1] = '\0';
			}
		} else {
			q = nxtDelim;
		}
	}

	chkHeap
	DBG_RETURN_S( StrTrim(fullpath))
}
