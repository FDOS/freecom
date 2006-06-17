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

ob: syslog_eol
su(bsystem): syslog
ty(pe): L
sh(ort description): Dump a newline into the logfile, if necessary
lo(ng description): Dumps a newline character into the logfile,
	if the line \para{format} is not terminated by one.
pr(erequistes): format != NULL
va(lue): none
re(lated to): syslog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include "syslog.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void syslog_eol(const char * const format)
{	DBG_ENTER("syslog_eol", Suppl_syslog)

	assert(format);

	if(!*format || strchr(format, '\0')[-1] != '\n') {
		DBG_STRING("Append newline")
		syslog_vprint("\n", 0);
	}

	DBG_EXIT
}
