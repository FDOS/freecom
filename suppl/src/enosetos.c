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

	Assigns a DOS error code to _doserrno and errno.

ob(ject): eno_setOSerror
su(bsystem): error
ty(pe): HL
sy(nopsis): 
sh(ort description): Assign a DOS error code to _doserrno and errno
he(ader files): 
lo(ng description): Assigns a DOS error code to _doserrno and
	recodes the value into the corresponding errno equivalent, which
	is storred, too.
pr(erequistes): 
va(lue): \tok{errnr}
re(lated to): eno_set
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
#include "eno.loc"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#ifdef __TURBOC__
extern char _dosErrorToSV [];
#elif __WATCOMC__
extern  int __set_errno_dos(unsigned int err);
#else
#error "Unsupported compiler!"
#endif

int eno_setOSerror(int errnr)
{
#ifdef __TURBOC__
	int eno;
#endif

	DBG_ENTER("eno_setOSerror", Suppl_error)
	DBG_ARGUMENTS( ("errnr=%u", errnr) )

#ifdef __TURBOC__
	if((unsigned)errnr > 88) {
		DBG_INFO( ("Errnr exceeds range") )
		eno = EFAULT;
	} else {
		eno = _dosErrorToSV[errnr];
	}

	errno = eno;

	DBG_RETURN_BI(_doserrno = errnr)
#elif __WATCOMC__
	 __set_errno_dos(errnr);
	 DBG_RETURN_I( errnr)
#endif
}
