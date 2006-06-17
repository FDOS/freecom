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

ob(ject): dfnfullpath2
su(bsystem): dfn
ty(pe): H
sh(ort description): Expand a path to a minimal fully-qualified one in Win9x-style
he(ader files): 
lo(ng description): Generates the fully-qualified path of a given file
	spec and removes any dot-components in Win9x-style.\par
	Before processing the passed in path is passed through
	\tok{dfnexpand(path, NULL)}.\newline
	".." that chdir above the root directory are ignored, which is
	very Unix-like, but is not supported by all DOS applications the
	same way.\par
	In Win9x the number of dots specified how many path components to
	remove, e.g.:
	\item "." --> one == this one
	\item ".." --> two == this one and one to the left
	\item "..." --> three, e.g.: "path1\path2\path3\..." becomes "path1\"
	\item a.s.o
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

ob(ject): dfnufullpath2
su(bsystem): dfn
ty(pe): H
sy(nopsis): 
sh(ort description): Expand a filename to a fully-qualified one in Win9x-style
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


#include "initsupl.loc"
#include "dfn.h"

#define OS_WIN32
#ifdef dfnfullpath
#undef dfnfullpath
#endif
#ifdef SUPPORT_UNC_PATH
#define dfnfullpath dfnufullpath2
#else
#define dfnfullpath dfnfullpath2
#endif

#include "dfnfullp.c"
