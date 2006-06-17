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

ob(ject): suppl_log_match_list
su(bsystem): debug
ty(pe): L
sy(nopsis): 
sh(ort description): Search a name within a log list and return its status
he(ader files): Searches for a name through a log list and return if
	the name is enabled or disabled. If there is no entry of the name
	within the list, the default status is returned.
lo(ng description): 
pr(erequistes): list != NULL
va(lue): 0:	disabled or \tok{str == NULL}
	\item else: enabled
re(lated to): suppl_log_find_list
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
#endif
#include <portable.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int suppl_log_match_list(suppl_log_list_t *list
	, suppl_log_csptr_t str)
{	int flg;

	assert(list);

	if(!str)
		return 0;

	if((flg = suppl_log_find_list(list, str)) == 0)
		/* no explicit entry --> return default */
		flg = list->suppl_l_name[0];

	return flg == '+';
}
