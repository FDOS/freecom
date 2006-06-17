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

	char *dfnmktmp(const char * const path, const char * const ext)

	Create a temporary file. path is duplicated into dynamic memory and
	is transformed into an absolute path.
	A filename (eight characters) is appended and, if ext != NULL,
	this extension as well.


	Return:
		NULL: on failure, e.g. malloc() failed or cannot create file
			or path == NULL

ob(ject): dfnmktmp
su(bsystem): dfn
ty(pe): H
sh(ort description): Create a temporary file in a given path
he(ader files): 
lo(ng description): Creates a temporary file in a given path with a given
	extension. The path must already exist. This function adds an eight
	byte randomized filename and appends the optionally given extension. 
	The file is created with as a zero-length regular file without any special
	attribute set.
pr(erequistes): \para{path} != NULL
va(lue): NULL: out of memory (ENOMEM), file creation failed (e.g. no such path) (OS error code),
		invalid argument (EINVAL), e.g. \tok{\para{path} == 0}
	\item else: pointer to dynamically stored, created filename
re(lated to): dfnmkfile
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnmktmp.c

*/

#include "initsupl.loc"

#include <assert.h>
#ifndef _MICROC_
#include <stdlib.h>
#include <string.h>
#else
#include <file.h>
#endif
#include <stdio.h>

#include "suppl.h"
#include "dfn.loc"
#include "dynstr.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfnmktmp(const char * const path, const char * const ext)
{	char *p;			/* temporary filename */
	char *q;
	const char *e;
	int err;

	DBG_ENTER("dfnmktmp", Suppl_dfn)
	DBG_ARGUMENTS( ("path=\"%s\", extension=\"%s\"", path, ext) )

	if(!path) {
		eno_set(EINVAL);
		DBG_RETURN_S( 0)
	}

	if(ext)
		e = *ext == '.'? ext + 1: ext;
	else e = "";

		/* Create an absolute filename */
		/* Then Extend the dynamic buffer to contain the extension
			and the randomized filename */
	chkHeap
	if((p = dfnexpand(path, 0)) == 0
	 || StrChg(p, strlen(p) + strlen(e) + 11) == 0)
		DBG_RETURN_S( 0)

	chkHeap
	if(!dfndelim2((q = strchr(p, '\0'))[-1]))
		*q++ = '\\';
	if(*e) {
		q[8] = '.';
		strcpy(q + 9, e);
	}
	else q[8] = '\0';

	do {
		dfn_tmpnam(q);		/* put the next temporary name in there */
		chkHeap
		if((err = dfnmkfile(p)) == 0)		/* success --> return it */
			DBG_RETURN_S( p)
	} while(err == 0x50);	/* already exists --> retry */

	eno_setOSerror(err);
	chkHeap
	DBG_RETURN_S( 0)			/* failed to create, but don't exist --> error */
}
