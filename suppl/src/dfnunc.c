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

ob(ject): UNCpath
su(bsystem): dfn
ty(pe): 
sh(ort description): Return the first character not part of UNC drive
he(ader files): 
lo(ng description): Returns the first character, which is not part of the
	UNC drive specification.
pr(erequistes): fnam != NULL
va(lue): NULL: no UNC path passed in
	\item "": whole \para{fnam} is UNC drive spec (of passed-in filename)
	\item else: first character not part of UNC drive
re(lated to): isUNCpath
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnunc.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include "dfn.loc"
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *UNCpath(const char * const fnam)
{	char *h;

	DBG_ENTER("UNCpath", Suppl_dfn)
	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	assert(fnam);

	if(!isUNCpath(fnam))
		DBG_RETURN_S( 0)

	if((h = strchr(fnam + 2, '\\')) != 0
	 && (h = strchr(h + 1, '\\')) != 0)
	 	DBG_RETURN_S( h)

	DBG_RETURN_BS( strchr(fnam, '\0'))
}
