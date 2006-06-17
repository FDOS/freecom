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


ob(ject): dfndelim
su(bsystem): dfn
ty(pe): 
sy(nopsis): 
sh(ort description): Test if a character is a path component delimiter
he(ader files): 
lo(ng description): Tests if a character delimits two path components.
	This function does not match the colon delimiting the
	drive specification from other path components.
pr(erequistes): 
va(lue): 0: no, not a component delimiter
	\item 1: yes, is a delimiter
re(lated to): dfndelim2
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

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dfndelim(const int ch)
{	
#define c (unsigned char)ch
	return c == '/' || c == '\\';
}
