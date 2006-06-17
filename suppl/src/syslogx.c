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


ob: syslogo
su(bsystem): syslog
ty(pe): 
sh(ort description): Dump one entry into the log file
lo(ng description): Dumps one entry onto the log file. If the
	log file has not been opened so far, it is opened via the
	\tok{openlog(NULL, 0, 0)} call.
pr(erequistes): 
va(lue): none
re(lated to): syslog openlog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include "syslog.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void syslogo( int priority, char *format, ...)
{	va_list ap;

	if(!syslog_opencount)		/* not opened */
		openlog(0, 0, 0);

	va_start(ap, format);
	syslogv(priority, format, ap);
}
