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

ob(ject): stpcat
su(bsystem): portable
ty(pe): 
sh(ort description): Append a string to another
lo(ng description): Appends a string to another one and return
	the end of the constructed string.
pr(erequistes): \tok{dst != NULL && src != NULL} and \tok{dst[]}
	must have enough space to append the string \tok{src[]}.
va(lue): pointer to the \tok{'\0'} terminator byte of the
	resulting string in \tok{dst[]}
re(lated to): strcpy
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#include <portable.h>
#ifndef _MICROC_
#include <string.h>
#endif
#include "suppl.h"

#include "suppldbg.h"

char *stpcat(char * const dst, const char * const src)
{	assert(dst);
	assert(src);

	return stpcpy(strchr(dst, NUL), src);
}
