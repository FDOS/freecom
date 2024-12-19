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

ob(ject): dfnpath
su(bsystem): dfn
ty(pe): H
sh(ort description): Return the fully-qualified current working path of a drive
he(ader files): 
lo(ng description): Returns the fully-qualified filename of the current
	working directory of a given drive. The drive can be specified as
	number (\tok{0}: currently working drive, \tok{1}..\tok{32}: A: .. Z: 
	and special Netware search drives), upper-case letters
	(\tok{'A'}..\tok{'Z'}) and lower-case letters (\tok{'a'}..\tok{'z'}).\par
	The path contains the drive specification "?:" and does not have a
	trailing backslash, unless it is the root directory.
pr(erequistes): 
va(lue): NULL: failure; errno is assigned:
	\list \tok{ENOMEM}: out of memory
	\item \tok{ENODEV}: Invalid drive while getting cwd
	\item \tok{ERANGE}: Buffer of getdcwd() too small
	\item \tok{ENODEV}: Failed to fetch cwd from drive
	\endlist
	\item else: pointer to
	dynamically allocted string of absolute pathname
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnpath.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <ctype.h>
#endif
#if defined(__GNUC__)
#include <direct.h>
#endif
#include "dfn.loc"
#include "dynstr.h"
#include "suppl.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfnpath(int drive)
{	char *h;

	DBG_ENTER("dfnpath", Suppl_dfn)
	DBG_ARGUMENTS( ("drive=%u ('%c')", drive, drive < 32? '.': drive) )

	if(isupper(drive)) drive -= 'A' - 1;
	else if(islower(drive)) drive -= 'a' - 1;
	else if((unsigned)drive > 32) {
		eno_set( ENODEV);
		DBG_RETURN_S( 0)
	}

	if(0 != (h = StrTrim(_getdcwd(drive, 0, DFN_FILENAME_BUFFER_LENGTH))))
		DBG_RETURN_S(h)

	eno_set(ENODEV);
	DBG_RETURN_S(0)
}
