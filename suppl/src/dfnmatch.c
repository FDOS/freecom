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
lo(ng description): Matches two filenames, if they match each other
	honoring wildcard patterns. Wildcards may appear everywhere, even in
	the drive spec. \tok{.} and \tok{..} components are \em{not} removed, but
	matched literally. The following tokens are recognized:
	\item \tok{?}: any, but one character except dot or space
	\item \tok{*}: any (maybe empty) sequence of characters upto the next dot,
	a delimiter or the end of the spec
	\item \tok{/} and \tok{\\}: delimiter of components, any sequence
	\item \tok{:}: delimiter of components, but doesn't match \tok{/}
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

ob(ject): dfnumatch
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

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dfnmatch(const char *fnam, const char *pattern)
{
	/* Already set by dfnmat2.c */
#ifndef NAME

#ifdef SUPPORT_UNC_PATH
#define NAME "dfnumatch"
#else
#define NAME "dfnmatch"
#endif
#define FCT  dfnmatchcomponent

#endif

	DBG_ENTER(NAME, Suppl_dfn)

	assert(fnam);
	assert(pattern);

	DBG_ARGUMENTS( ("fnam=\"%s\", pattern=\"%s\"", fnam, pattern) )

	DBG_RETURN_BI( dfnmatchEx(fnam, pattern, FCT))
}
