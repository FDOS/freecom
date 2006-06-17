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

ob(ject): dfnfilenameext
su(bsystem): dfn
ty(pe): 
sh(ort description): Return a pointer to the extension of a filename component
he(ader files): 
lo(ng description): Returns the pointer to the extension of a filename component, the
	passed in file spec must not contain any path component.\par
pr(erequistes): 
va(lue): NULL: \para{fnam} == NULL
	\item \tok{*fnam} == '\0'}: no extension
	\item else: pointer to the first dot of the filename
re(lated to): dfnextension
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

char *dfnfilenameext(const char * const fnam)
{	const char *p;

	DBG_ENTER("dfnfilenameext", Suppl_dfn)

	if(!fnam)	DBG_RETURN_S( 0)

	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	p = fnam;
	while(*p == '.') 	/* Leading dots are no extension marker */
		++p;

	assert(*p != '.');
	while(*p && *++p != '.');

	DBG_RETURN_S( (char*)p )
}
