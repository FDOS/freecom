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

ob: syslog_vprint
su(bsystem): syslog
ty(pe): L
sh(ort description): Dump a string into the log file
lo(ng description): Dumps a string into the log file. The arguments
	are passed via a \tok{<stdarg.h>} pointer and, if the
	flag \tok{LOG_PERROR} has been passed to \tok{openlog},
	issues the same string into the \tok{stderr} device.\par
	On write error the program is terminated with an appropriate
	error message.
pr(erequistes): fmt != NULL
va(lue): none
re(lated to): syslog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <stdarg.h>
#include <stdio.h>
#include "syslog.loc"
#include "msgs.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif
void syslog_vprint(const char * const fmt, va_list ap)
{
	assert(fmt);

	if(syslog_logfile && vfprintf(syslog_logfile, fmt, ap) == EOF)
		Esuppl_writeFile(syslog_fnam);

	if((syslog_options & LOG_PERROR) != 0)
		vfprintf(stderr, fmt, ap);
}
