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

	DOS NLS implementation -- "Country" information DOS-65-XX
	See DOC\CNTRY.H

	These functions provide an easy access to the information
	stored in the DOS NLS, but does not actually offer any
	functions to transform/compare etc.

*/

#ifndef __CNTRY_H
#define __CNTRY_H

#ifndef __PORTABLE_H
#include <portable.h>
#endif
#include "suppl.h"

/* All information from Ralf Brown's interrupt list v5.6 */
struct Country_ {		/* local copy of country information */
	/* pointers to tables */
	fartbl uppercaseTbl;	/* uppercase for "normal" characters */
	fartbl lowercaseTbl;	/* lowercase for "normal" chars (maybe NULL) */
	fartbl fupcaseTbl;		/* uppercase for filename characters */
	fartbl collateTbl;		/* collating table */
#if defined(DBCS)
	fartbl dbcsTbl;			/* DBCS table */
#endif

	fartbl illegalChars;	/* illegal characters in filenames */
	int illegalLen;			/* illegal characters in filenames */

	/* length of the tables
		If length <= 0x80 the table does not contain values for
		the indexs 0..0x7F, but the pointers are adjusted that index 0x80
		really points to the first entry of the table. */
	int uppercaseLen;	/* uppercase for "normal" characters */
	int lowercaseLen;	/* lowercase for "normal" chars (maybe NULL) */
	int fupcaseLen;		/* uppercase for filename characters */
	int collateLen;		/* collating table */

	/* Extended country information */
	word country;		/* currently active DOS country code */
	word charset;		/* currently active DOS codepage */
	word datefmt;		/* format of date: 0/1/2 == USA/Europe/Japan */
	byte curSymbol[6];	/* ASCIZ currency symbol */
	byte thousendsSep[3];	/* ASCIZ decimal separator */
	byte decimalSep[3];	/* ASCIZ decimal separator */
	byte dateSep[3];	/* ASCIZ date separator */
	byte timeSep[3];	/* ASCIZ time separator */
	byte listSep[3];	/* ASCIZ list separator */
	byte curFormat;		/*	bit 0: 0/1 == preceed/follow value
							bit 1: separate cur sym from value by one space
							bit 2: cur sym replaces decimal point */
	byte precision;		/* number of digits after decimal point */
	byte timefmt;		/* bit 0: 0/1 == 12/24 hour clock */

	/* valid filename characters */
	byte inclFirst, inclLast;	/* permissible character range */
	byte exclFirst, exclLast;	/* excluded character range */

	byte initialized;
};

#ifdef _MICROC_
#define Country struct Country_
#else /*#	!(defined(_MICROC_)) */
typedef struct Country_ Country;
#endif /*#	defined(_MICROC_) */

Country *nlsInfo(void);
/*
 *	Return a pointer to a filled copy of above structure
 *
 *	During the life-time of a program the information is retrieved once.
 *
 *	Return: Always != NULL, but maybe ome fields are NULL.
 */

Country *nlsNewInfo(void);
/*
 *	As nlsInfo(), but causes to reread the information cached by
 *	nlsInfo().
 */

#endif /*#	!defined(__CNTRY_H) */
