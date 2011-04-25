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

ob(ject): suppl_log_log_header
su(bsystem): debug
ty(pe): L
sy(nopsis): 
sh(ort description): Prepare to submit a debug log entry
lo: Prepares to issue a debug log entry into the logfile.\par
	This function dumps the standard information (time, continueous
	number) and the indention. The line is left open right at the
	position where to write the message to.\newline
	If the logfile is closed, it is opened automatically.
he(ader files): 
pr(erequistes): 
va(lue): none
re(lated to): 
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

#include "nls.h"
#include "appname.h"
#include "dynstr.h"
#include "supplio.h"
#include "msgs.h"

#include "syslog.loc"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_log_header(void)
{	char *fnam;
	nlstime t;

	assert(suppl_Stack);

	if(!suppl_log_lock())
		return;

	/* Open the logfile if not done so already */
	if(!suppl_l_logfile) {
		/* If to lock the debug package fails, _item() and _trailer()
			will silently ignore any calls */
		chkHeap
		if((fnam = appNameEx()) == 0)
			Esuppl_noMem();
		fnam = EStrConcat(2, fnam, DBG_EXTENSION);
		suppl_l_logfile = Eopen(fnam, suppl_l_openmode);
		chkHeap
		free(fnam);
	}
	chkHeap

	nlsCurTime(aS(t));

	suppl_log_unlock();

	suppl_log_log_item("%02u:%02u:%02u.%02u ", t.ti_hour, t.ti_minute
	 , t.ti_second, t.ti_hundred);

	if(S(indent)) {			/* indent */
		int i;

		suppl_log_log_item("%9ld:", suppl_l_count);
		for(i = suppl_l_nestlevel; i--; )
			suppl_log_log_item("\t|");
	}
	else {					/* no indention, dump nestlevel numerical */
		suppl_log_log_item("%ld [%d]: ", suppl_l_count, suppl_l_nestlevel);
	}
}
