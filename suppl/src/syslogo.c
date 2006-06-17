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

ob: openlog
su(bsystem): syslog
ty(pe): 
sh(ort description): Open the logfile
lo(ng description): Opens the logfile, hence, enable logging.\par
	The subsystem keeps track how often the logfile is opened and
	closed. The first time the logfile is opened, it is opened
	in \em{overwrite} mode (\tok{"wt"} is passed to \tok{fopen()}).
	Each time \tok{openlog()} is called, an internal counter is
	incremented. Each time \tok{closelog()} is called, this counter
	is decremented. If the counters falls to zero, the logfile
	is closed, hence, disable logging. Any call to \tok{closelog()}
	is ignored when the counter is zero. The next time \tok{openlog()}
	is called, the logfile is opened in \em{append} mode (\tok{"at"}
	is passed to \tok{fopen()}) to preserve the log messages generated
	so far.\par
	Any call to \tok{openlog()} directly or indirectly from within
	\tok{openlog()} is ignored.\par
	On error, such as out-of-memory conditions, the program is terminated
	with an appropriate error message.\par
	The parameters are preserved, eventually duplicated into the heap,
	to be used by \tok{syslog()} later on.
pr(erequistes): 
va(lue): none
re(lated to): closelog syslog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include "syslog.loc"
#include "dynstr.h"
#include "appname.h"
#include "supplio.h"
#include "msgs.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void openlog(const char * const ident, int option, int  facility)
{	static FLAG8 recursive = 0;			/* to prevent recursive calls
											to openlog() when using
											syslogo() */
	DBG_ENTER("openlog", Suppl_syslog)

		/* "++semaphore" is considered an atomic operation by
			many programs; it's native C's best approximation
			for the P(semaphore) operation */
	if(++recursive == 1		/* not recursively called */
	 && ++syslog_opencount == 1) {	/* not opened til now */
	 	/* When these statements are executed, no logfile has
	 		been opened so far. That means that they can be logged
	 		onto STDERR only. */
		chkHeap
		syslog_options = option;
		syslog_facility = facility;
		syslog_ident = Estrdup(ident);
		if((syslog_fnam = appNameEx()) == 0)
			Esuppl_noMem();
		syslog_fnam = SYSLOG_FNAM(syslog_fnam);
		syslog_logfile = Eopen(syslog_fnam, syslog_openmode);
		chkHeap
	}

	--recursive;

	DBG_EXIT
}
