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

ob(ject): memicmp
su(bsystem): portable
ty(pe): 
sh(ort description): Case-insensitive compare of two memory areas
lo(ng description): Compares case-insensitively two memory
	areas. The characters are mangled by \tok{<ctype.h>}'s \tok{toupper()}
	function, thus, only the plain Latin letters \tok{'a'}..\tok{'z'} are
	recognized.\par
	The return valus is compareble to:\example{|}
	|	memcmp(memupr(s1, length), memupr(s2, length), length)
pr(erequistes): s1 != NULL && s2 != NULL
va(lue): <0: upcased s1 < upcased s2
	\item =0: upcased s1 == upcased s2
	\item >0: upcased s1 > upcased s2
re(lated to): memcmp
se(condary subsystems): 
bu(gs): 
wa: Only the plain Latin letters \tok{'a'}..\tok{'z'} are mangled.
co(mpilers): Micro-C and Pacific HiTech C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#define COMPILE
#else
#ifdef _PAC_NOCLIB_
#define COMPILE
#endif
#endif

#ifdef COMPILE

#include <assert.h>
#include <ctype.h>

int memicmp(const void *Xs1, const void *Xs2, unsigned len)
{	int result;

#ifdef HI_TECH_C
	const unsigned char *s1 = Xs1;
	const unsigned char *s2 = Xs2;
#else
#define s1 ((const unsigned char*)Xs1)
#define s2 ((const unsigned char*)Xs2)
#endif

	assert(s1);
	assert(s2);

	result = 0;
	while(len-- && !(result = toupper(*s2++) - toupper(*s1++)));
	return result;
}
#endif
