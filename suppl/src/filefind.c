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

ob(ject): findfirst
su(bsystem): portable
ty(pe): 
sh(ort description): Initiate a file search loop
lo(ng description): Initiates a file search loop.\par
	This function returns the first entry matching the specified
	pattern within a directory and initializes the structure \para{ff}
	that subsequent calls to \tok{findnext()} will enumerate all
	entries matching the pattern.
	\example{|}
	|struct ffblk ff;
	|int done;
	|
	|done = findfirst(pattern, aF(ff), attributes);
	|while(!done) {
	|	// do something with the values of the structure ff
	|	done = findnext(aF(ff));
	|}
pr(erequistes): ff != NULL && pattern != NULL
va(lue): 0: on success\item else: OS error code (usually interpreted as
	"no more matching entries")
re(lated to): findnext
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Pacific HiTech C only

ob(ject): findnext
su(bsystem): portable
ty(pe): 
sh(ort description): Continue a directory enumeration loop
lo(ng description): Continues a directory enumeration loop;
	for more details see function \tok{findfirst()}.
pr(erequistes): ff != NULL
va(lue): 0: on success\item else: OS error code (usually interpreted as
	"no more matching entries")
re(lated to): findfirst
se(condary subsystems): 
in(itialized by): findfirst
wa(rning): 
bu(gs): 
co(mpilers): Pacific HiTech C only

*/

#include "initsupl.loc"

#ifdef _PAC_NOCLIB_
#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int findfirst(const char * const pattern, struct ffblk *ff, int attrib)
{	void far *dta;
	IREGS r;
	int rv;

	/* DOS uses the DTA to store the internal search data to.
		The usual implementation of find*() keeps the first
		21 bytes of the DTA or simply hope that no DOS will use
		more than 21 byte and point the DTA directly on the
		search data buffer. */
	dta = getdta();
	setdta((void far*)ff);
	r.r_dx = FP_OFF(pattern);
	r.r_ds = FP_SEG(pattern);
	r.r_cx = attrib;
	r.r_ax = 0x4e00;
	rv = invokeDOS(&r);

	setdta(dta);

	return rv;
}
	
int findnext(struct ffblk *ff)
{	void far *dta;
	IREGS r;
	int rv;

	dta = getdta();
	setdta((void far*)ff);
	r.r_ax = 0x4f00;
	rv = invokeDOS(&r);

	setdta(dta);

	return rv;
}

#endif
