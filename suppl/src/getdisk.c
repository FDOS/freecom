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

ob(ject): getdisk
su(bsystem): portable
ty(pe): 
sh(ort description): Return current working (default) drive / disk
lo(ng description): 
pr(erequistes): 
va(lue): always valid: 0 == A:, 1 == B:, ...
re(lated to): getbootdisk setdisk
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Pacific HiTech C [implements return type \tok{(char*)}]

*/

#include "initsupl.loc"

#ifdef _PAC_NOCLIB_
#include <suppl.h>
#include <sys.h>

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int getdisk(void)
{	return *getdrv() - 'A';
}

#endif

