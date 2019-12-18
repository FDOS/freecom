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

	NLS-depended dynamic string functions.

*/

#ifndef __NLS_C_H
#define __NLS_C_H

#include <portable.h>

int toLower(int ch);
int toUpper(int ch);
int toFUpper(int ch);

#define memcmpI MemiCmp
int MemiCmp(const byte *buf1, const byte *buf2, unsigned len);

#define _fmemcmpI _fMemiCmp
#ifdef _MICROC_
int _fMemiCmp(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length);
#else
int _fMemiCmp(const byte far * dest, const byte far * src, unsigned length);
#endif

int StriCmp(const char *s1, const char *s2);
char *StrUpr(char *s);
char *MemUpr(char buf[], unsigned length);
void _fStrUpr(const word dseg, word dofs);
void _fMemUpr(const word dseg, word dofs, unsigned length);

int isodigit(int ch);
#define isodigit(ch)	((ch) >= '0' && (ch) <= '7')
int toxdigit(int ch);

#endif
