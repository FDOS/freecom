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
int memcmpFI(const byte *buf1, const byte *buf2, unsigned len);
#ifdef _MICROC_
int _fmemcmpFI(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length);
#else
int _fmemcmpFI(const byte far * dest, const byte * src, unsigned length);
#endif

#endif
