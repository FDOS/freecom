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

ob(ject): dfnmatchEx
su(bsystem): dfn
ty(pe): 
sh(ort description): Match two filenames using specified-style pattern match
he(ader files): 
lo(ng description): Matches two filenames, if they match each other
	honoring wildcard patterns. Wildcards may appear everywhere, even in
	the drive spec. \tok{.} and \tok{..} components are \em{not} removed, but
	matched literally. The tokens recognized depend on the
	actual match function passed.\par
	There are no assumptions how long a component is
	or which characters it is constructed of (except for the
	specially recognized characters and \tok{'\0'}), but one component should not
	contain more than one dot (except for \tok{..}). Dots may start or end
	a component.\par
	Unless the macro \tok{SUPPORT_UNC_PATH} is defined prior \tok{#include}'ing
	the header file, UNC paths are not supported. Though, the only difference
	is that it is not checked if both filenames are UNC paths or both are not.
pr(erequistes): fnam != NULL && pattern != NULL
va(lue): 0: no\item else: yes
re(lated to): dfnumatch dfnmatch
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): One component may not exceed the length of \tok{DFN_FILENAME_BUFFER_LENGTH} bytes.
fi(le): dfnmatch.c

ob(ject): dfnumatchEx
su(bsystem): dfn
ty(pe): 
sh(ort description): Match two filenames using DOS-style pattern match
he(ader files): 
lo(ng description): As \tok{dfnmatch()}, but supports UNC paths.
pr(erequistes): fnam != NULL && pattern != NULL
va(lue): 0: no\item else: yes
re(lated to): dfnumatch dfnmatch
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): One component may not exceed the length of \tok{DFN_FILENAME_BUFFER_LENGTH} bytes.
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
#ifndef OS_WIN32		/* passed from DFNMATC2.C */
#define OS_DOS
#endif

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#define isPathDelim dfndelim

/*
 *	Copy the next path component and return the break character.
 */
static int copyComp(char *dst, const char **src)
{	int ch;
	const char *p;

	assert(dst);
	assert(src && *src);

	p = *src;
	while((ch = (*dst++ = *p++)) != ':' && ch && !isPathDelim(ch));
	if(ch) dst[-1] = '\0';
	*src = p;
	if(ch == '\\') ch = '/';

	return ch;
}

/*
 *	Break the filename into path components, then let them
 *	be matched individually
 */
int dfnmatchEx(const char *fnam
	, const char *pattern
	, int (*matchfct)(const char *, const char *))
{	char n1[DFN_FILENAME_BUFFER_LENGTH], n2[DFN_FILENAME_BUFFER_LENGTH];
	int c1, c2;

#ifdef SUPPORT_UNC_PATH
#define NAME "dfnumatchEx"
#else
#define NAME "dfnmatchEx"
#endif

	DBG_ENTER(NAME, Suppl_dfn)

	assert(fnam);
	assert(pattern);
	assert(matchfct);

	DBG_ARGUMENTS( ("fnam=\"%s\", pattern=\"%s\"", fnam, pattern) )

#ifdef SUPPORT_UNC_PATHS
	if(isUNCpath(fnam) != isUNCpath(pattern))
		DBG_RETURN_I( 0)
#endif

	/* 1. Make sure both names start or don't start with a path delimiter */
	if(isPathDelim(*fnam)) {
		if(!isPathDelim(*pattern))
			DBG_RETURN_I( 0)
		while(isPathDelim(*++fnam));
		while(isPathDelim(*++pattern));
	}
	else if(isPathDelim(*pattern))
		DBG_RETURN_I( 0)
	/* Both names point to a non-path delimiter character */

	/* 2. break up the next path component */
	do {
		c1 = copyComp(n1, &fnam);
		c2 = copyComp(n2, &pattern);

		if(c1 != c2 || !(*matchfct)(n1, n2))
			DBG_RETURN_I( 0)

		if(c1 == '/') {		/* slash / backslash */
			/* skip multiple path component delimiters */
			while(isPathDelim(*fnam)) ++fnam;
			while(isPathDelim(*pattern)) ++pattern;
		}
	} while(c1);

	DBG_RETURN_I( 1)
}
