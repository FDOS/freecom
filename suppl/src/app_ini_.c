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

ob(ject): appInit_
su(bsystem): appName
ty: L
sy(nopsis): int appInit_(char * const fnam)
sh(ort description): Initialize the \subsys{appName} from a filename
lo(ng description): Initializes the \subsys(appName} from passed filename.\par
 This function can be called more than once and refreshes the internal
 buffer each time.
pr: The passed filename must be an absolute one and a valid pointer into
 the heap.
va: integer SUPPL error code
re(lated to): appFile appName appInitEnv appPath appInit
fi(le): app_ini_.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include "errcodes.h"
#include "appname.loc"
#include "dynstr.h"
#include "dfn.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int appInit_(char * const fnam)
{	DBG_ENTER("appInit_", Suppl_appName)

	if(!fnam)			DBG_RETURN_I(ESUPPL_INIT)

	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	chkHeap

	StrRepl(app_file, fnam); chkHeap

	app_fnam = dfnfilename(app_file) - 1; chkHeap
	if(app_fnam <= app_file || !*app_fnam) {
		StrFree(app_file);			/* indicate: not-initialized */
									/* as app_fnam is invalid */
		chkHeap;
		DBG_RETURN_I(ESUPPL_FPATH)
	}
	app_fext = strrchr(app_fnam, '.');
	DBG_RETURN_I(ESUPPL_OK);
}
