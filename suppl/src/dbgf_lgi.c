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

ob(ject): suppl_log_log_item
su(bsystem): debug
ty(pe): L
sy(nopsis): 
sh(ort description): Submit a message into the debug log file
he(ader files): 
pr(erequistes): 
va(lue): none
re(lated to): suppl_log_log_header suppl_log_log_vitem suppl_log_log_trailer
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): suppl_log_log_vitem
su(bsystem): debug
ty(pe): L
sy(nopsis): 
sh(ort description): Submit a message into the debug log file
he(ader files): 
pr(erequistes): 
va(lue): none
re(lated to): suppl_log_log_header suppl_log_log_item suppl_log_log_trailer

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>

#include "syslog.loc"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_log_item(suppl_log_csptr_t fmt, ...)
{	va_list ap;

	va_start(ap, fmt);
	suppl_log_log_vitem(fmt, ap);
	va_end(ap);
}

void suppl_log_log_vitem(suppl_log_csptr_t fmt, va_list ap)
{
	assert(fmt);
	if(suppl_l_logfile && suppl_log_lock()) {
		suppl_log_unlock();
		vfprintf(suppl_l_logfile, fmt, ap);
		chkHeap
	}
}
