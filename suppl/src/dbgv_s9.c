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

ob(ject): Suppl_inifile1
su(bsystem): debug
ty(pe): L0
sy(nopsis): 
sh(ort description): Name of \subsys{inifile/1} passed to DBG_ENTER() macros
he(ader files): 
lo(ng description): Contains the name of \subsys{inifile/1} passed to
	DBG_ENTER() macros in order to prevent that every function
	creates its own copy of the word into its data segment.
pr(erequistes): 
va(lue): "inifile/1"
re(lated to): DBG_ENTER
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): static
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

const char Suppl_inifile1[] = "inifile/1";
