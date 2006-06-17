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

ob(ject): closelog
su(bsystem): syslog
ty(pe): 
sh(ort description): Close the log file
lo(ng description): Closes the logfile. see \tok{openlog()} for more
	information
pr(erequistes): 
va(lue): none
re(lated to): openlog syslog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include "syslog.loc"
#include "dynstr.h"
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void closelog(void)
{	DBG_ENTER("closelog", Suppl_syslog)
	DBG_ARGUMENTS( ("open count=%u", syslog_opencount) )

	if(!syslog_opencount || --syslog_opencount) {	/* not or still opened */
		DBG_EXIT
	}

	DBG_STRING("Closing logfile")

	Fclose(syslog_logfile);
	chkHeap
	StrFree(syslog_fnam);
	StrFree(syslog_ident);
	syslog_logfile = 0;
	*syslog_openmode = 'a';		/* next openlog() will append rather than
									overwrite */

	chkHeap
	DBG_EXIT
}
