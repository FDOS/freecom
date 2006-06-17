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

ob(ject): suppl_log_close
su(bsystem): debug
ty(pe): L
sy(nopsis): 
sh(ort description): Close the logfile
lo(ng description): Closes the logfile, its data and directory information,
	so that if it asynchroneously terminates, every item written so far
	really is in the logfile. If something is written into the logfile,
	the logfile is opened again and the new entries are appended to it.
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

#include <stdio.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_close(void)
{	if(suppl_l_logfile) {
		fclose(suppl_l_logfile);
		suppl_l_logfile = 0;		/* identify that no LOG file is open */
		*suppl_l_openmode = 'a';	/* Don't overwrite next open */
	}
}
