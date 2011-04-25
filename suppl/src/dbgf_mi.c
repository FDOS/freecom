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

ob(ject): suppl_log_match_item
su(bsystem): debug
ty(pe): L
sy(nopsis): 
sh(ort description): Test if the item matches the requested string
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

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int suppl_log_match_item(suppl_log_list_t *item
	, suppl_log_csptr_t s)
{	assert(item);
	assert(s);
	assert(*s);

	if(memcmp(&item->suppl_l_name[1], s, strlen(&item->suppl_l_name[1])))
		return 0;						/* no match */

	return item->suppl_l_name[0];		/* return the enable/disable status */
}
