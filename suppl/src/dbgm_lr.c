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

ob(ject): suppl_log_realloc
su(bsystem): debug
ty(pe): 
sh(ort description): Write a "realloc()" action into the syslog
he(ader files): 
lo(ng description): This function is part of the memory allocation
	monitoring functions and writes a formatted \tok{<pointer_new> = realloc(<pointer_old>, <bytes>)}
	information into the syslog.
	These entries can be verified later by an external program.
pr(erequistes): 
va(lue): none
re(lated to): suppl_dbg_change syslog
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dbgm_lr.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>
#include "syslog.h"

#include "msgs.loc"
#include "msgs.lng"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_realloc(void const * const newpoi, const void * const oldpoi
	, const size_t size)
{
	syslog(0, "realloc(0x%p, %u) = 0x%p; %s", oldpoi, size, newpoi
	 , newpoi? I_success: I_failure);
}
