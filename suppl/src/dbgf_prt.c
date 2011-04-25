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

ob(ject): suppl_log_print_1
su(bsystem): debug
ty(pe): L
sy(nopsis): 
sh(ort description): Test and open one log entry
he(ader files): 
lo(ng description): Tests, if enabled, and, if so, opens one entry in the debug log.\par
	This function dumps the usual prefix (time, entry number and indention)
	and the information class into the logfile and leaves the entry open.
pr(erequistes): inf_class != NULL
va(lue): 0: if logging is disabled for the particular information class
	\item else: otherwise
re(lated to):  suppl_log_log_header suppl_log_log_trailer suppl_log_log_item suppl_log_log_vitem suppl_log_log suppl_log_print_2
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): The suppl_log_log() function became unsuiteable, because there is
	no portable way to implement a macro like DBG_PRINT(), which is called
	with a dynamic parameter list _and_ calls a function with this
	list and one additional argument. Therefore the logging process is
	splitted into two functions, what may lead to problems, if they
	are not called in sequence.
bu(gs): 
co(mpilers): 

ob(ject): suppl_log_print_2
su(bsystem): debug
ty(pe): L
sy(nopsis): 
sh(ort description): Fill and close one log entry
he(ader files): 
lo(ng description): Fills and closes one entry in the debug log.\par
	This function dumps the usual information into the debug logfile and
	closes this entry, thus, it is the \em{back-end} of the suppl_log_print_1()
	function.
pr(erequistes): fmt != NULL
va(lue): none
re(lated to):  suppl_log_log_header suppl_log_log_trailer suppl_log_log_item suppl_log_log_vitem suppl_log_log suppl_log_print_1
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): see suppl_log_print_1()
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

int suppl_log_print_1(suppl_log_csptr_t inf_class)
{	if(suppl_log_class_enabled(inf_class)) {
		suppl_log_log_header();
		suppl_log_log_item("%s: ", inf_class);
		return 1;
	}
	return 0;
}

void suppl_log_print_2(suppl_log_csptr_t fmt, ...)
{	va_list ap;

	assert(fmt);

	va_start(ap, fmt);

	suppl_log_log_vitem(fmt, ap);
	suppl_log_log_trailer();

	va_end(ap);
}
