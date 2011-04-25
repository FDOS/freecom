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

ob(ject): suppl_log_push
su(bsystem): debug
ty(pe): 
sh(ort description): Duplicates the current state of the logging functions
he(ader files): 
lo(ng description): The state of the logging function is organized within
	a stack, the topmost item controls the behaviour of the logging
	functions. This function duplicates the topmost item, thus, preserves the
	current state to be restored later.
pr(erequistes): 
va(lue): none
re(lated to): suppl_log_change, suppl_log_pop
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dbgf_psh.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_push(void)
{	
	if(suppl_log_lock()) {
		suppl_log_stack_t *p;

		assert(suppl_Stack);

		memcpy(p = Emalloc(sizeof(suppl_log_stack_t))
		 , suppl_Stack, sizeof(suppl_log_stack_t));
		p->suppl_l_nxt = suppl_Stack;
		suppl_Stack = p;

		S(ownClasses) = S(ownFunctions) = S(ownFiles) = 0;
		suppl_log_unlock();
		chkHeap
	}
}
