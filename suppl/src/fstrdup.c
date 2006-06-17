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

ob(ject): _fdupstr
su(bsystem): farmem
ty(pe): 
sh(ort description): Duplicate a far string into local heap
lo(ng description): Duplicates the far string into the local,
	usually near heap
pr(erequistes): the source pointer must not be \tok{NULL}
va(lue): NULL: on failure\item else: pointer to the string in the
	heap
re(lated to): strdup
se(condary subsystems): portable
in(itialized by): 
wa(rning): This function cannot be named \tok{_fstrdup}, because this
	would interfere with the implementation of string functions of
	compilers supporting far poiners and a far heap.
bu(gs): 
co(mpilers):

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>
#include "dynstr.h"
#include "fmemory.h"

#include "suppldbg.h"

char *_fdupstr(unsigned const segm, unsigned const ofs)
{	size_t len;
	char *p;

	if(!(p = malloc(len = _fstrlen(MK_FP(segm, ofs)) + 1)))
		return 0;
	copy_seg(TO_FP(p), segm, ofs, len);

	return p;
}

#else

#include <stdlib.h>
#include <string.h>
#include <portable.h>
#include "fmemory.h"

#include "suppldbg.h"

char *_fdupstr(const char far * const s)
{	size_t len;
	char *p;

	chkHeap
	if((p = malloc(len = _fstrlen(s) + 1)) == 0)
		return 0;
	_fmemcpy(TO_FP(p), s, len);

	chkHeap
	return p;
}

#endif
