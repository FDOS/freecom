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

ob(ject): Ftmpnam
su(bsystem): supplio
ty(pe): 
sh(ort description): Generate a temporary file
lo(ng description): Generates the name for a temporary file with the
	specified extension and located
	within the currently effective temporary directory and creates
	this file.\par
	To create the file prevents that the same filename is generated
	by multiple programs or multiple calls to Ftmpnam(), because
	the call to the system function to create a new, non-existant
	file is the actual atomic probe, whether or not the generated filename
	is already in use.\newline
	Upon successful return the file has been closed and has no unusual
	attributes set, meaning only the 'archive' attribute is set,
	the others are cleared.\par
	The currently effective temporary directory is determined by the
	following sequence:
		\enum %TEMP%
		\enum %TMP%
		\enum %TEMPDIR%
		\enum %TMPDIR%
		\enum <current_drive>\TEMP
		\enum <current_drive>\TMP
		\enum <boot_drive>\TEMP
		\enum <boot_drive>\TMP
		\enum .\
		\enum <boot_drive>\
	\endlist
	The first directory, in which a file can be created, is considered
	the temp directory.\newline
	This function also makes sure that the path is fully-qualified.\par
	The environment variables are retreived by env_fetch().
	\par If \tok{ext == NULL || *ext == '\0'}, no extension is appended.
pr(erequistes): 
va(lue): NULL: on failure\item else: pointer to a static buffer, which contents
	is overwritten the next time this function is called
re(lated to): Ftmpfile dfnmktmp Fxopen
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif

#include "supplio.loc"
#include "environ.h"
#include "dfn.h"
#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *Ftmpnam(const char * const ext)
{	
	static char sTemp[] = "?:\\TEMP";
	static char sTmp[] = "?:\\TMP";
	static char sBoot[] = "?:\\";
	static char *last = 0;

	DBG_ENTER("Ftmpnam", Suppl_supplio)
	DBG_ARGUMENTS( ("ext=\"%s\"", ext) )

	free(last);

/* Micro-C cannot handle the next in a big if()
	statement */

	chkHeap
	if((last = dfnmktmp(env_fetch(0, "TEMP"), ext)) != 0)
		DBG_RETURN_S( last)
	if((last = dfnmktmp(env_fetch(0, "TMP"), ext)) != 0)
		DBG_RETURN_S( last)
	if((last = dfnmktmp(env_fetch(0, "TEMPDIR"), ext)) != 0)
		DBG_RETURN_S( last)
	if((last = dfnmktmp(env_fetch(0, "TMPDIR"), ext)) != 0)
		DBG_RETURN_S( last)
	if((last = dfnmktmp(sTemp + 2, ext)) != 0)
		DBG_RETURN_S( last)
	if((last = dfnmktmp(sTmp + 2, ext)) != 0)
		DBG_RETURN_S( last)
	sTemp[0] = sTmp[0] = sBoot[0] = 'A' - 1 + getbootdisk();
	if((last = dfnmktmp(sTemp, ext)) != 0)
		DBG_RETURN_S( last)
	if((last = dfnmktmp(sTmp, ext)) != 0)
		DBG_RETURN_S( last)
	if((last = dfnmktmp(".", ext)) != 0)
		DBG_RETURN_S( last)

	chkHeap
	DBG_RETURN_BS( last = dfnmktmp(sBoot, ext))
}
