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

	Declarations for NLS depended DOS filename functions.

*/

#ifndef __NLS_F_H
#define __NLS_F_H

#include <portable.h>

int toFUpper(int ch);

#define memcmpFI MemFCmp
int MemFCmp(const void *buf1, const void *buf2, unsigned len);

#define _fmemcmpFI _fMemFCmp
#ifdef _MICROC_
int _fMemFCmp(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length);
#else
int _fMemFCmp(const void far * dest, const void far * src, unsigned length);
#endif

char *StrFUpr(char *s);
void *MemFUpr(void * const buf, unsigned length);

#endif
