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

ob(ject): env_resizeCtrl
su(bsystem): 
ty(pe): 
sy(nopsis): 
sh(ort description): Controls how the \subsys{env} internally works
he(ader files): 
lo(ng description): This variable contains a \tok{'|'} combination of the
	following macros, defined in ENVIRON.H:
	\item \tok{ENV_SHRINKBELOW}: allows to resize the environment segment
		below its last used byte (will destroy environment,
		is necessary to delete env)
	\item \tok{ENV_ALLOWMOVE}: allows to relocate environment segment
		if to grow it fails (meaning to move it into another memory
		segment)
	\item \tok{ENV_USEUMB}: when the environment segment is relocated, search
		in the UMBs first
	\item \tok{ENV_FIRSTFIT}: when the environment segment is relocated,
		 use the memory allocation strategy \em{First Fit}
	\item \tok{ENV_LASTFIT}: when the environment segment is relocated,
		use the memory allocation strategy \em{Last Fit}
	\item If both \tok{ENV_FIRSTFIT} and \tok{ENV_LASTFIT} are set,
		the behaviour is undefined
	\item If neither \tok{ENV_FIRSTFIT} nor \tok{ENV_LASTFIT} are set,
			 \em{Best Fit} is used
	\nolist
	The default settings are: deny shrinking & moving, use UMBs, best fit
pr(erequistes): 
va(lue): none
re(lated to): env_resize env_setsize
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

unsigned env_resizeCtrl = ENV_USEUMB;	/* control flags */
