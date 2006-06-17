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

ob(ject): dfnfilename
su(bsystem): dfn
ty(pe): 
sh(ort description): Return a pointer to the filename component
he(ader files): 
lo(ng description): Returns the pointer to the filename component of the
	passed in file spec.\par
	The "filename component" is the very last component of the file spec
	and it need not reference to the regular file within the filesystem.\newline
	If \para{fnam} does not have a filename portion, a pointer to \tok{'\0'}
	is returned.\newline
	UNC paths are supported in so far as they don't cause problems with
	this function. But a \tok{"\\\\server\\share"} string will return
	a pointer to \tok{"share"} and for \tok{"\\\\server"} \tok{"server"}
	is returned.
pr(erequistes): 
va(lue): NULL: \para{fnam} == NULL\item else: pointer to filename
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnfnam.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include "dfn.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfnfilename(const char * const fnam)
{	const char *p;

	DBG_ENTER("dfnfilename", Suppl_dfn)

	if(!fnam)	DBG_RETURN_S( 0)

	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	p = strchr(fnam, '\0');
	while(--p >= fnam && !dfndelim2(*p));

	DBG_RETURN_S( (char*)p + 1)
}
