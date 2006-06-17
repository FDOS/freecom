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

ob(ject): setdisk
su(bsystem): portable
ty(pe): 
sh(ort description): Set the current working drive / disk
lo(ng description): Sets the current working drive.\par
	\tok{0} == A:, \tok{1} == B: ...
pr(erequistes): 
va(lue): total number of available drives
re(lated to): getdisk getbootdisk
se(condary subsystems): 
wa: To verify that the functions succeeds check the current
	working disk with \tok{getdisk()}, e.g.\example{|}
	| setdisk(dr);
	| if(getdisk() != dr)
	|		printf("Failed to change to drive %c:", 'A' + dr);
co(mpilers): Pacific HiTech C only

*/

#include "initsupl.loc"

#ifdef _PAC_NOCLIB_
#include <sys.h>
#include <string.h>
#include <portable.h>

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int setdisk(int drv)
{	char buf[3];

	memcpy(buf,"A:", 3);	/* don't modify string constant */
	*buf += drv;
	return chdrv(buf);
}

#endif
