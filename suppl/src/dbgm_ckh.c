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

ob(ject): suppl_realloc
su(bsystem): debug
ty(pe): 
sh(ort description): Perform a monitored \tok{realloc()} action
he(ader files): 
lo(ng description): If memory monitoring is enabled (SUPPL_LOG_MEMORY), the
	standard \tok{realloc()} function is replaced by this function via a
	macro. This function calls \tok{suppl_log_realloc()} to write the stats
	into the syslog.
pr(erequistes): 
va(lue): Return value of monitored \tok{realloc()}
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dbgm_sr.c

*/

#include "initsupl.loc"

#if __TURBOC__ > 0x201

#include <alloc.h>
#include <conio.h>
#include <stdlib.h>

#include "suppldbg.h"

#define MAX_SEGMENTS 128

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_testHeap(void)
{
	if(heapcheck() != _HEAPOK) {
		cputs("\r\nHeap corrupted\r\nPress any key to terminate application\r\n");
		getch();
		DBG_TERMINATE("Heap corrupted");
	}
}

#endif
