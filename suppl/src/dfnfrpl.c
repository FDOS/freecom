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

ob(ject): dfnfreplace
su(bsystem): dfn
ty(pe): H
sh(ort description): Replace a file with another one and make a backup
he(ader files): 
lo(ng description): \para{newname}'s backup file is deleted, if it exists.
	Then \para{newname} is renamed into its backup filename, if it exists.
	Then \para{oldname} is renamed into \para{newname}.\par
	The name of the backup file is created with \tok{dfnbakname()}.\par
	UNC paths are supported.
pr(erequistes): 
va(lue): \endlist integer SUPPL error code\newline
	\item ESUPPL_OK: on success
		\item ESUPPL_NOMEM: on out-of-memory error
		\item ESUPPL_FACCESS: on access error (rename/remove failed)
re(lated to): dfnbakname
se(condary subsystems): 
in(itialized by): 
wa(rning): The file cannot be renamed across filesystems.
bu(gs): On failure, the "to" / "newname" file is probably deleted, but the
	"from" / "oldname" file has not be renamed. This might lead to inconsistencies or even to loose data unless handled correctly.
fi(le): dfnfrpl.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#ifdef __WATCOMC__
#include <io.h>
#endif
#include <stdio.h>
#include "errcodes.h"
#include "dfn.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dfnfreplace(const char * const newname, const char * const oldname)
{	char *bak;

	DBG_ENTER("dfnfreplace", Suppl_dfn)

	assert(newname);
	assert(oldname);

	DBG_ARGUMENTS( ("to:\"%s\", from=\"%s\"", newname, oldname) )

	chkHeap
	if((bak = dfnbakname(newname)) != 0) {
		DBG_INFO( ("bakup=\"%s\"", bak) )
		unlink(bak);		/* delete the backup file, if present */
		rename(newname, bak);	/* backup current file, if present */
		chkHeap
		free(bak);
		chkHeap
		if(!dfnstat(newname)) {
			/* file doesn't exist (either renamed or not present at all) */
			rename(oldname, newname);
			if(dfnstat(newname))	/* rename succeded */
				DBG_RETURN_I( ESUPPL_OK)
		}
	}
	else
		DBG_RETURN_I( ESUPPL_NOMEM)

	chkHeap
	DBG_RETURN_I( ESUPPL_FACCESS)
}
