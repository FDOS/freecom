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

ob(ject): getopt_init
su(bsystem): getopt
ty(pe): 
sy(nopsis): 
sh(ort description): Initialize internal variables of getopt()
he(ader files): 
lo(ng description): Initializes all internal variables of any \tok{getopt()}
	function to allow to call a \tok{getopt()} function when another
	\tok{getopt()} cycle is still active. This function should be called
	before a second \tok{getopt()} cycle is started.
pr(erequistes): 
va(lue): none
re(lated to): getopt_restore getopt_save
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): \tok{optind} is initialized with 1 (one), which means that the
	first argument processed by \tok{getopt()} is \tok{argv[1]}. This is
	required to support the \tok{argv[]} array passed in by \tok{main()}.
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>
#include "getopt.h"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void getopt_init(void)
{
	optind = 1;
	optcur = 0;
	opterr = 1;
}
