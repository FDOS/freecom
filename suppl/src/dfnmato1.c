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

ob(ject): dfnmatchcomponent
su(bsystem): dfn
ty(pe): 
sh(ort description): Match two components using DOS-style pattern match
he(ader files): 
lo(ng description): Matches two components, if they match each other
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
pr(erequistes): fnam != NULL && pattern != NULL
va(lue): 0: no\item else: yes
re(lated to): dfnumatch dfnmatch dfnmatchext
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
#ifndef OS_WIN32		/* passed from DFNMATC2.C */
#define OS_DOS
#endif

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

/*
 *	Match a filename component against a pattern
 */
int dfnmatchcomponent(const char *fnam, const char *pattern)
{	int pch, ch;

#ifdef OS_DOS
#define NAME "dfnmatchcomponent"
#else
#define NAME "dfnmatchcomponent2"
#endif
	DBG_ENTER(NAME, Suppl_dfn)

	assert(fnam);
	assert(pattern);

	DBG_ARGUMENTS( ("fnam=\"%s\", pattern=\"%s\"", fnam, pattern) )

	while((pch = *pattern++) != '\0') {
		ch = *fnam++;
		switch(pch) {
		case '?':		/* any character except dot in DOS mode */
#ifdef OS_DOS
			if(ch == '.')
				DBG_RETURN_I( 0 )
#endif
			if(!ch)
				DBG_RETURN_I( 0 )
			break;
		case '*':
#ifdef OS_DOS
			/* skip any character upto the next dot or '\0' */
			while(*pattern && *pattern != '.')
				++pattern;
			if(ch && ch != '.')
				while(*fnam && *fnam != '.')
					++fnam;
#else
			/* an asterisk in Win32 does act more Unix-stylish,
				let's use a recursive algorithm */
			/* collapse multiple asterisks, they have no meaning
				anyway */
			while((pch = *pattern) == '*') ++pattern;
			if(!pch) 	/* asterisk at the end of name --> always match */
				DBG_RETURN_I( 1 )

			--fnam;			/* was advanced above */
			if(pch == '.') {		/* often used patterns */
				if(!*++pattern)	/* any sequence, no dot in it */
								/* or end in a dot */
					DBG_RETURN_BI( strchr(fnam, '.') == 0
					 || (*fnam && ((char*)strchr(fnam, '\0'))[-1] == '.'))

				/* optimization: advance to the next dot within 'fnam'.
					Because '.' is not case-depend, it's easy. */
				while((fnam = strchr(fnam, '.')) != 0)
					if(dfnmatchcomponent(++fnam, pattern))
						DBG_RETURN_I( 1 )
				DBG_RETURN_I( 0 )
			}

			if(!ch)
				DBG_RETURN_I( 0 )

			/* Now try the remaining pattern against the 'fnam' with
				none to N characters strip from the beginning of fnam. */
			/* This routine is not that much called, therefore no
				special optimization are implemented */
			do {
				if(dfnmatchcomponent(fnam, pattern))
					DBG_RETURN_I( 1 )
			} while(*++fnam);
			DBG_RETURN_I( 0 )
#endif
			/** Warning: Unreachable code **/
#ifndef __TURBOC__
			break;
#endif
		case '.':		/* one trailing dot is ignored */
			if(!*pattern)
				goto ret;
			/* Fall through */

		default:
			if(toupper(ch) != toupper(pch))
				DBG_RETURN_I( 0 )
		}
	}

ret:		/* ignore one trailing dot */
	DBG_RETURN_BI( *fnam == '\0' || (*fnam == '.' && fnam[1] == '\0') )
}
