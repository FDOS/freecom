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

ob(ject): suppl_log_pop
su(bsystem): debug
ty(pe): 
sh(ort description): Discard the current state of the logging functions
he(ader files): 
lo(ng description): The state of the logging functions is organized within
	a stack. The topmost state is the "current" state under which control
	the logging functions perform their work. This function discards the
	current state and restores the previous one. The function will not
	discard the last state.
pr(erequistes): more than one state within the status-stack
va(lue): none
re(lated to): suppl_log_change, suppl_log_push
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dbgf_pop.c

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

void suppl_log_pop(void)
{	
	if(suppl_log_lock()) {
		if(suppl_Stack && S(nxt)) {
			/* OK more than one entry */
			suppl_log_stack_t *p;

			chkHeap
			if(S(ownClasses))
				suppl_log_del_list(S(classes));
			if(S(ownFunctions))
				suppl_log_del_list(S(functions));
			if(S(ownFiles))
				suppl_log_del_list(S(files));

			chkHeap
			p = suppl_Stack;
			suppl_Stack = S(nxt);
			free(p);
			chkHeap
		}
		suppl_log_unlock();
	}
}
