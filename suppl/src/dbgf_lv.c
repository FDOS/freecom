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

ob(ject): suppl_log_leave
su(bsystem): debug
ty(pe): 
sy(nopsis): 
sh(ort description): Leave a function and reconfigure logging status
he(ader files): 
pr(erequistes): local_data != NULL
va(lue): none, but
	\item the current nest level is resetted to the original value
	\item the function-depend enable status is re-evaluated
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

void suppl_log_leave(suppl_log_local_data_t *local_data)
{
	assert(local_data);
	assert(local_data->suppl_l_fctname);

	if(suppl_log_class_enabled("fct"))
		/* Create the log entries when leaving a function */
		suppl_log_log("<%s()", local_data->suppl_l_fctname);

	if(local_data->suppl_l_prv_nestlevel != suppl_l_nestlevel - 1)
		suppl_log_log("Missing %u DBG_LEAVE macro(s)"
		 , suppl_l_nestlevel - 1 - local_data->suppl_l_prv_nestlevel);

	/* restore the former settings */
	suppl_l_nestlevel = local_data->suppl_l_prv_nestlevel;
	suppl_l_fct_enabled = local_data->suppl_l_prv_fct_enabled;
	chkHeap
}
