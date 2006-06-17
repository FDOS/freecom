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

ob(ject): suppl_log_exit
su(bsystem): debug
ty(pe): 
sh(ort description): Terminate the application with a message onto screen
he(ader files): 
lo(ng description): Terminates the application logging the passed in
	string onto the screen, then terminates the application with
	DBG_EXIT_CODE. Used bu DBG_TERMINATE() if logging is disabled.
pr(erequistes): 
va(lue): none
re(lated to): suppl_log_terminate
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dbgf_chg.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <process.h>
#include <conio.h>
#endif
#include <portable.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_exit(suppl_log_csptr_t loginfo)
{	if(loginfo) {
		cputs("\r\n");
		cputs(loginfo);
		cputs("\r\n");
	}
	exit(DBG_EXIT_CODE);
}
