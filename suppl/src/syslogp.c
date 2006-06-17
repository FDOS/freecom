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

ob: syslog_print
su(bsystem): syslog
ty(pe): L
sh(ort description): Dump one line into the logfile
lo(ng description): Dumps one line into the logfile and uses the
	\tok{printf()}-style to specify what to dump.
pr(erequistes): fmt != NULL
va(lue): none
re(lated to): syslog syslog_eol
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <stdarg.h>
#include "syslog.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void syslog_print(const char * const fmt, ...)
{	va_list ap;

	va_start(ap, fmt);

	syslog_vprint(fmt, ap);

	va_end(ap);
}
