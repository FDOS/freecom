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

ob(ject): dfnmatchcomponent2
su(bsystem): dfn
ty(pe): 
sh(ort description): Match two filename components using Win32-style pattern match
he(ader files): 
lo(ng description): Matches two filename components, if they match each other
	honoring wildcard patterns. Wildcards may appear everywhere, even in
	the drive spec. \tok{.} and \tok{..} components are \em{not} removed, but
	matched literally. The following tokens are recognized:
	\item \tok{?}: any, but one character
	\item \tok{*}: none or more characters
	\item \tok{/} and \tok{\\}: delimiter of components, any non-empty sequence
	\item \tok{:}: delimiter of components, but doesn't match \tok{/}
	\endlist
	The following components are recognized specifically:
	\item \tok{*}: any filename
	\item \tok{*.*}: any filename with at least one dot in it
	\item \tok{*.}: any filename with no dot in it, or a dot at the very end.
	\endlist
	There are no assumptions how long a component is, how many dots it
	contains or which characters it is constructed of (except for the
	specially recognized characters and '\0'). Dots may start or end
	a component.\par
pr(erequistes): fnam != NULL && pattern != NULL
va(lue): 0: no\item else: yes
re(lated to): dfnumatch2 dfnexpand dfnmatch
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs):
fi(le): dfnmatc2.c

*/


#include "initsupl.loc"
#include "dfn.h"

#define OS_WIN32
#ifdef dfnmatchcomponent
#undef dfnmatchcomponent
#endif
#define dfnmatchcomponent dfnmatchcomponent2

#include "dfnmato1.c"
