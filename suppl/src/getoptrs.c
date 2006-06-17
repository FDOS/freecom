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

ob(ject): getopt_save
su(bsystem): getopt
ty(pe): 
sy(nopsis): 
sh(ort description): Save internal variables of getopt()
he(ader files): 
lo(ng description): Saves all internal variables of any \tok{getopt()}
	function to allow to call a \tok{getopt()} function when another
	\tok{getopt()} cycle is still active. It is even possible to switch
	back and forth two or more \tok{getopt()} cycles.
pr(erequistes): sbuf != NULL
va(lue): none
re(lated to): getopt_restore getopt_init
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 


ob(ject): getopt_restore
su(bsystem): getopt
ty(pe): 
sy(nopsis): 
sh(ort description): Restore internal variables of getopt()
he(ader files): 
lo(ng description): Restores all internal variables of any \tok{getopt()}
	function. See \tok{getopt_save()}.
pr(erequistes): sbuf != NULL
va(lue): none
re(lated to): getopt_save getopt_init
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
#endif
#include <portable.h>
#include "getopt.h"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void getopt_save_(struct getopt_save_t * const sbuf)
{	sbuf->gs_optind = optind;
	sbuf->gs_optcur = optcur;
	sbuf->gs_opterr = opterr;
}

void getopt_restore_(struct getopt_save_t * const sbuf)
{
	optind = sbuf->gs_optind;
	optcur = sbuf->gs_optcur;
	opterr = sbuf->gs_opterr;
}
