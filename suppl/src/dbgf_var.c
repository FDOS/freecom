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

ob(ject): suppl_stack1
su(bsystem): debug
ty(pe): L
sy(nopsis): 
sh(ort description): Initial (default) debug stack
he(ader files): 
lo(ng description): Initial debug stack; the default values are:
	\item no files to be logged; defaults to "disable"
	\item no functions to be logged; defaults to "disable"
	\item no classes to be logged; defaults to "disable"
	\item unlimited depth (32767)
	\item not enabled
	\item indent enabled
pr(erequistes): 
va(lue): 
re(lated to): 
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): static
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): suppl_Stack
su(bsystem): debug
ty(pe): 
sy(nopsis): 
sh(ort description): Pointer to the current debug stack item
he(ader files): 
lo(ng description): 
pr(erequistes): 
va(lue): 
re(lated to): suppl_log_change
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): static suppl_log_push suppl_log_pop
wa(rning): 
bu(gs): 
co(mpilers): 

ob: suppl_l_nestlevel
su: debug
sh: current nesting level of functions during logging
re: suppl_log_enter suppl_log_leave
in: static suppl_log_enter suppl_log_leave

ob: suppl_l_fct_enabled
su: debug
sh: status if the function name/class allows logging
re: suppl_log_enter suppl_log_leave
in: static suppl_log_enter suppl_log_leave

ob: suppl_l_openmode
su: debug
sh: how to open the logfile
re: suppl_log_enter
in: static suppl_log_enter suppl_log_close

ob: suppl_l_logfile
su: debug
sh: opened logfile
re: suppl_log_log_header
in: static suppl_log_log_header

ob: suppl_l_count
su: debug
sh: line counter of logfile
re: suppl_log_log_header
in: static suppl_log_log_header

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdio.h>
#endif
#include <limits.h>
#include <portable.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif


suppl_log_stack_t suppl_stack1 = {
	 0					/* no next item */
	,						/* files to be logged */
		{ 0
		  , { '-', '\0' }
		}
	,						/* classes to be logged */
		{ 0
		  , { '-', '\0' }
		}
	,						/* functions to be logged */
		{ 0
		  , { '-', '\0' }
		}
	,INT_MAX				/* maximum nesting level */
	,0						/* logging active? */
	,1						/* indent output? */
	,1						/* 'files' member NOT inherited */
	,1						/* 'classes' member NOT inherited */
	,1						/* 'functions' member NOT inherited */
};

suppl_log_stack_t *suppl_Stack = aS(suppl_stack1);

int suppl_l_nestlevel = 0;				/* current nesting level */
FLAG suppl_l_fct_enabled = 0;			/* function class permits logging */
FILE *suppl_l_logfile = 0;			/* logfile opened? */
char suppl_l_openmode[] = "wt";			/* openmode for the logfile
											first character must be w/a */
long suppl_l_count = 0;					/* line counter */
