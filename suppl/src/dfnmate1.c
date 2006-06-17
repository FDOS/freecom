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

ob(ject): dfnmatchext
su(bsystem): dfn
ty(pe): 
sh(ort description): Match two extensions using DOS-style pattern match
he(ader files): 
lo(ng description): Matches two extensions, if they match each other
	honoring wildcard patterns. The following tokens are recognized:
	\item \tok{?}: any, but one character except dot or space
	\item \tok{*}: any (maybe empty) sequence of characters upto the next dot,
	a delimiter or the end of the spec
	\endlist
	The following components are recognized specifically:\newline
	\item \tok{*} and \tok{*.*}: any filename
	\item \tok{*.}: any filename with no dot in it, or a dot at the very end (meaning
		a filename without an extension)
	\endlist
	There are no assumptions how long a component is
	or which characters it is constructed of (except for the
	specially recognized characters and \tok{'\0'}), but one component should not
	contain more than one dot (except for \tok{..}). Dots may start or end
	a component.\par
	\par The pattern is assumed to start with a dot, the filename is
	skipped to the first extension dot.
pr(erequistes): fnam != NULL && pattern != NULL
va(lue): 0: no\item else: yes
re(lated to): dfnmatch dfnmatchcomponent
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs):
fi(le): dfnmatch.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <ctype.h>
#include "dir.loc"
#include <string.h>
#endif
#include "dfn.loc"

#ifdef OS_DOS
#undef OS_DOS
#endif
#ifndef OS_WIN32		/* passed from DFNMATE2.C */
#define OS_DOS
#endif

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dfnmatchext(const char * const fnam , const char *pattern)
{	char *p;

	DBG_ENTER("dfnmatchext", Suppl_dfn)

	assert(fnam);
	assert(pattern);

	DBG_ARGUMENTS( ("fnam=\"%s\", pattern=\"%s\"", fnam, pattern) )

	/* find extension in filename */
	p = dfnfilenameext(fnam);
	assert(p);
	assert(*p == '\0' || *p == '.');
	if(!*p) {
		/* No dot --> no extension */
		DBG_RETURN_I(*pattern == 0);
	}

	if(*pattern == '.')		/* one dot is optional */
		++pattern;

#ifdef OS_DOS
	/* At most one extension in DOS mode */
	DBG_RETURN_BI(dfnmatchcomponent(p + 1, pattern))
#else
	/* Try each extension as the pattern may include the asterisk */
	do if(dfnmatchcomponent2(p + 1, pattern)) {
		DBG_RETURN_I(1)
	} while(0 != (p = strchr(p + 1, '.')));

	DBG_RETURN_I(0)
#endif
}
