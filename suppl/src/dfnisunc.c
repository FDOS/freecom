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

ob(ject): isUNCpath
su(bsystem): dfn
ty(pe): 
sh(ort description): Check if a path is an UNC path spec
he(ader files): 
lo(ng description): Check if a path is an UNC path spec. These ones are preceeded
	by two backslashs. UNC path specs are \em{always} fully-qualified by
	definition (probably the extension of the file is missing).
pr(erequistes): 
va(lue): 0: no or \tok{fnam == NULL}\item else: yes
re(lated to): UNCpath
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnisunc.c

*/

#include "initsupl.loc"

#include "dfn.loc"
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int isUNCpath(const char * const fnam)
{	DBG_ENTER("isUNCpath", Suppl_dfn)
	DBG_ARGUMENTS( ("fnam\"%s\"", fnam) )
	DBG_RETURN_BI( fnam && *fnam == '\\' && fnam[1] == '\\')
}
