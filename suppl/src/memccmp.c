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

ob(ject): memccpy
su(bsystem): portable
ty(pe): 
sh(ort description): Copy one memory area to another upto a character
lo(ng description): Copies a memory area over another one up to
	including the first occurance of \para{ch} or until \para{len}
	characters have been copied.
pr(erequistes): s1 != NULL && s2 != NULL
va(lue): dst
re(lated to): memcpy
se(condary subsystems): 
bu(gs): 
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

void *memccmp(void *Xs1, const void *Xs2, int ch, size_t len)
{
#ifdef HI_TECH_C
	unsigned char *s1 = Xs1;
	const unsigned char *s2 = Xs2;
#else
	void *oS1 = Xs1;
#define s1 ((unsigned char*)Xs1)
#define s2 ((const unsigned char*)Xs2)
#endif

	assert(s1);
	assert(s2);

	while(len-- && (s1++ = s2++) != ch);

#ifdef HI_TECH_C
	return Xs1;
#else
	return Os1;
#endif
}
#endif
