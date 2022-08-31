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

ob(ject): _fstrcpy
su(bsystem): farmem
ty(pe): 
sh(ort description): Copy a far string onto another
lo(ng description): Copies a far string onto another one. The destination
	string must have at least \tok{_fstrlen()+1} bytes in size.
pr(erequistes): pointers must not be \tok{NULL}
va(lue): none
re(lated to): strcpy
se(condary subsystems): portable
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"
#include "fmemory.loc"

#ifdef COMPILE
char far *_fstrcpy(fargDecl(char, dst), fargDecl(const char, src))
{
	return (char far *)_fmemcpy(fargPass(dst), fargPass(src), _fstrlen1(src));
}
#endif
