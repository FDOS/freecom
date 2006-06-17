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

ob(ject): suppl_log_log
su(bsystem): debug
ty(pe): _
sy(nopsis): 
sh(ort description): Write one log entry
he(ader files): 
lo(ng description): Writes one entry into the debug log.\par
	The entry is prefixed by the time and, if enabled, indented
	the necessary depth. Then a printf()-like command is executed
	and one newline chatacter is appended.
pr(erequistes): fmt != NULL
va(lue): none
re(lated to):  suppl_log_log_header suppl_log_log_trailer suppl_log_log_item suppl_log_log_vitem
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

void suppl_log_log(suppl_log_csptr_t fmt, ...)
{	va_list ap;

	assert(fmt);

	va_start(ap, fmt);

	suppl_log_log_header();
	suppl_log_log_vitem(fmt, ap);
	suppl_log_log_trailer();

	va_end(ap);
}
