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

ob(ject): isodigit
su(bsystem): portable
ty(pe): UA
sh(ort description): Test if a character is an octal digit
pr(erequistes): 
va(lue): 0: \para{ch} is not an octal digit\item else: is one
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): Unless \tok{#undef isodigit} is specified, the argument
	is evaluated twice.
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#undef isodigit
int isodigit(int ch)
{	return ch >= '0' && ch <= '7';
}
