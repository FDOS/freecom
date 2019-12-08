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

ob: toFUpper
su(bsystem): nls
ty(pe): 
sh(ort description): Upcases a character for filenames
lo(ng description): Transforms a character to uppercase according the
	DOS NLS table for filename characters. If no such table is
	available, the standard C \tok{toupper()} function is used.
pr(erequistes): 
va(lue): upcased character in the range: 0..255
re(lated to): nlsInfo toUpper
se(condary subsystems): 
bu(gs): 
co(mpilers): 

ob: toUpper
su(bsystem): nls
ty(pe): 
sh(ort description): Upcases a character
lo(ng description): Transforms a character to uppercase according the
	DOS NLS table for characters. If no such table is
	available, the standard C \tok{toupper()} function is used.
pr(erequistes): 
va(lue): upcased character in the range: 0..255
re(lated to): nlsInfo toFUpper toLower
se(condary subsystems): 
bu(gs): 
co(mpilers): 

ob: toLower
su(bsystem): nls
ty(pe): 
sh(ort description): Lowercases a character for filenames
lo(ng description): Transforms a character to lowercase according the
	DOS NLS table for characters. If no such table is
	available, the standard C \tok{tolower()} function is used.
pr(erequistes): 
va(lue): lowercased character in the range: 0..255
re(lated to): nlsInfo toUpper
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <ctype.h>
#endif
#include <portable.h>
#include "suppl.h"
#include "dynstr.h"
#include "cntry.h"
#include "fmemory.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif


static int toMTrans(int ch, int len, fartbl iM(*)tbl, int (*fct)(int))
{
	/* If len <= 0x80, the table has been filled only with
		the values from 0x80..0xFF */
	ch &= 0xFF;
	return (len <= 0x80 && ch < 0x80 && !tbl)
		? (byte)fct(ch)		/* this is the standard case */
		: dpeekb(tbl, ch);
}

#undef toupper
int toFUpper(int ch)
{	return toMTrans(ch, nlsInfo()->fupcaseLen, nlsInfo()->fupcaseTbl
		, toupper);
}

int toUpper(int ch)
{	return toMTrans(ch, nlsInfo()->uppercaseLen, nlsInfo()->uppercaseTbl
		, toupper);
}

#undef tolower
#if 0
int toLower(int ch)
{	return toMTrans(ch, nlsInfo()->lowercaseLen, nlsInfo()->lowercaseTbl
		, tolower);
}
#endif
