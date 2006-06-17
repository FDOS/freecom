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
/* $Id$
   $Locker$	$Name$	$State$

	Supplemental string functions.

ob(ject): utoa10
su(bsystem): misc
ty(pe): M
sh(ort description): Transform an unsigned decimal number into ASCII format
lo(ng description): Transforms an unsigned decimal number into its ASCII
	representation.
pr(erequistes): buf != NULL
va(lue): none
re(lated to): itoa10
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): itoa10
su(bsystem): misc
ty(pe): M
sh(ort description): Transform a signed decimal number into ASCII format
lo(ng description): Transforms a signed decimal number into its ASCII
	representation.
pr(erequistes): buf != NULL
va(lue): none
re(lated to): utoa10
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#ifndef __STR_H
#define __STR_H

#ifndef _MICROC_
#include <stdlib.h>
#include <string.h>
#endif
#include <portable.h>

char *skipws(const char *s);
char *skipwd(const char *s);
char *singleword(char *s);

/* void itoa10(int num, char * const buf); */
/* void utoa10(int num, char * const buf); */
#ifndef _MICROC_
#define itoa10(num,buf)	itoa((num), (buf), 10)
#define utoa10(num,buf) ultoa((unsigned)(num), (buf), 10)
#else
	/* negative radix --> signed */
#define itoa10(num,buf)	itoa((num), (buf), -10)
	/* positive radix --> unsigned */
#define utoa10(num,buf)	itoa((num), (buf), 10)
#endif

int strnum(const char * const s, int * const num, char ** const endp);
int strbool(char *s);

/* memzero() fills a block of memory with 0; used throughout
	SUPPL where a larger amount of values of data types: (int), (short),
	(char) and pointers are to initialize with 0 or NULL.
	It is unportable, but efficient on portable systems; because of this
	macro places, where to port on failure, are easy to locate. */
/* void memzero(void *poi, size_t length); */
#define memzero(poi,length)	memset((poi), 0, (length))
#define memzerobj(object)	memzero(&object,sizeof(object))
#define memzerostrct(structure)	memzero(aS(structure),sizeof(structure))

#endif
