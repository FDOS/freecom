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

ob(ject): suppl_log_mk_item
su(bsystem): debug
ty(pe): HL
sy(nopsis): 
sh(ort description): Make a new item of a name log list
he(ader files): 
lo(ng description): 
pr(erequistes): 
va(lue): 
re(lated to): 
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

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>

#include "dynstr.h"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

suppl_log_list_t *suppl_log_mk_item(const char * const s)
{	suppl_log_list_t *h;

	assert(s);
	assert(*s == '+' || *s == '-');

	h = Emalloc(sizeof(suppl_log_list_t) - 1 + strlen(s));
	h->suppl_l_nxt = 0;
	strcpy(&h->suppl_l_name[0], s);

	chkHeap
	return h;
}
