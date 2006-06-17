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

	stpcpy(char *dst, char *src)

	Copy src onto dst

	Return a pointer to the '\0' of dst

   Target compilers: Pacific C v7.51

ob(ject): stpcpy
su(bsystem): portable
ty(pe): 
sh(ort description): Copy a string onto another
lo(ng description): Copies string \tok{src[]} over \tok{dst[]}
	and returns the end of the resulting string
pr(erequistes): \tok{dst != NULL && src != NULL} and \tok{dst[]}
	must have enough space to copy \tok{src[]} to
va(lue): pointer to \tok{'\0'} terminator byte of the resulting
	string
re(lated to): 
se(condary subsystems): 
bu(gs): 
co(mpilers): Pacific HiTech C only

*/

#include "initsupl.loc"

#ifdef COMPILE
#undef COMPILE
#endif

#ifdef _PAC_NOCLIB_
#define COMPILE
#endif
#ifdef __WATCOMC__
#define COMPILE
#endif

#ifdef COMPILE
#include <portable.h>

#include <assert.h>

char *stpcpy(char * const dst, const char * const src)
{	register char *p;
	register const char *q;

	assert(dst);
	assert(src);

	p = dst - 1;
	q = src - 1;
	while(*++p = *++q);
	return p;
}

#endif
