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

ob(ject): MemFUpr
su(bsystem): nls
ty(pe): 
sh(ort description): Upercases a memory area using DOS NLS
he(ader files): 
lo(ng description): Upercases a memory area using DOS NLS information
	for filenames. \tok{s[]} gets overwritten.
pr(erequistes): toUpper
va(lue): \para{buf}
re(lated to): MemiCmp
se(condary subsystems):
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dmemupr.c

*/

#include "initsupl.loc"
#include <portable.h>
#include "nls_f.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void *MemFUpr(void * const buf, unsigned length)
{	char *p;

	DBG_ENTER("MemFUpr", Suppl_nls)

	if((p = buf) != 0)
		while(length--)
			*p = toFUpper(*p), ++p;

	DBG_RETURN_S( buf)
}
