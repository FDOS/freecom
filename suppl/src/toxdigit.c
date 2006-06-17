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

ob: toxdigit
su(bsystem): misc
ty(pe): 
sh(ort description): Transform a hexadecimal digit into a number
lo(ng description): Transforms a hexadecimal digit (ASCII character)
	into the corresponding integer number:
	\item \tok{'0'}..\tok{'9'}: 0..9
	\item \tok{'a'}..\tok{'z'}: 10..36
	\item \tok{'A'}..\tok{'Z'}: 10..36
	\endlist
	\em{Note:} This function supports all letters, thus, can be
	used to transform a valid digit from ASCII --> integer for
	all radixs through 2..37.
pr(erequistes): character must be a valid ASCII digit for the
	requested range
va(lue): integer representation of the digit in the range: 0..36
re(lated to): isxdigit isodigit
se(condary subsystems): 
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

int toxdigit(int ch)
{	return ch <= '9'? ch - '0': ch <= 'Z'? ch - 'A' + 10: ch - 'a' + 10;
}
