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

ob: syslog
su(bsystem): syslog
ty(pe): 
sh(ort description): Dump one entry into the log file
lo(ng description): Dumps one entry into the log file.\par
	The contents of the entry is specified as with the \tok{printf()}
	function. An entry currently looks like this:\newline
	(f/l: )(hh:mm:ss.hh )(ident )([pid]): user_entry\newline
	The '()'s surround one item, which can be absent.\newline
	'f' is substituted by the effective \para{facility}, if one of
	facility and log level are non-zero.\newline
	'l' is substituted by the effective \para{log level}, if one of
	facility and log level are non-zero.\newline
	'hh:mm:ss.hh' is substituted by the current time, if the
	flag \tok{LOG_NOTIME} has \em{not} been passed to \tok{openlog()}.\newline
	'ident' is substituted by the \para{ident} parameter passed to
	\tok{openlog}, if non-zero.\newline
	'pid' is substituted by the current active PID (PSP), if the
	flag \tok{LOG_PID} has been passed to \tok{openlog}.\newline
	'user_entry' is the string that results from interpreting the
	\para{format} string.\par
	If \tok{openlog} has not been called so far, this function
	returns immediately, doing nothing at all.\newline
  If priority has no facility, it defaults to the facility set
  by \tok{openlog()}.\newline
  If priority has no level, it defaults to \tok{LOG_DEBUG}.
pr(erequistes): 
va(lue): none
re(lated to): syslogv openlog closelog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

ob: syslogv
su(bsystem): syslog
ty(pe): 
sh(ort description): Dump one entry into the log file
lo(ng description): Dumps one entry into the log file; as \tok{syslog},
	but the arguments are passed using a \tok{<stdarg.h>} pointer.
pr(erequistes): 
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
#include "appname.h"
#include "nls.h"
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void syslogv( int priority, char *format, va_list ap)
{	int facility;
	nlstime t;

	if(!syslog_opencount		/* not opened */
	 || !format) return;		/* nothing to log */

	if((facility = FACILITY(priority)) == 0)
		facility = syslog_facility;

	/* dump prefix */
	if(facility || LEVEL(priority))
		syslog_print("%d/%d: ", facility, LEVEL(priority));
	if((syslog_options & LOG_NOTIME) == 0) {			/* time if req */
		nlsCurTime(aS(t));
		syslog_print("%02u:%02u:%02u.%02u ", t.ti_hour, t.ti_minute
		 , t.ti_second, t.ti_hundred);
	}
	if(syslog_ident)
		syslog_print("%s ", syslog_ident);
	if(syslog_options & LOG_PID)
		syslog_print("[%u] ", _psp);
	syslog_print(": ");

	/* Now print the message itself */
	syslog_vprint(format, ap);

		/* make sure there is a '\n' at the end of the line */
	syslog_eol(format);
	chkHeap
#ifdef SYSLOG_ALWAYS_FLUSHLOG
	fcommit(syslog_logfile);
#endif
}

void syslog( int priority, char *format, ...)
{	va_list ap;

	va_start(ap, format);
	syslogv(priority, format, ap);
}
