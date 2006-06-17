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

ob(ject): suppl_dfltEnvSegm
su(bsystem): 
ty(pe): 
sy(nopsis): 
sh(ort description): default environment for SUPPL
he(ader files): 
lo(ng description): All functions of \subsys{env}, that accept a \tok{0]
	as environment segment, first use \tok{env_dfltSeg} first,
	\tok{env_glbSeg} then. The variable \tok{suppl_dfltEnvSegm} contains
	the value read by \tok{env_dfltSeg} and written by \tok{env_setDfltSeg}.
	This variable is also automatically updated by functions, that delete
	or relocate an environment segments.\par
	default: \tok{0}
pr(erequistes): 
va(lue): none
re(lated to): env_dfltSeg env_setDfltSeg
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): static; may be changes any time by the user directly
wa(rning): 
bu(gs): 

 */

#include "initsupl.loc"

#include "environ.h"

word suppl_dfltEnvSegm = 0;
