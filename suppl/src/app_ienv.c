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

ob(ject): appInitEnv
su(bsystem): appName
ty: 
sy(nopsis): int appInitEnv(void)
sh(ort description): Initialize the \subsys{appName} from environment
lo(ng description): Initializes the \subsys(appName} from the name of the
 application stored within the environment by the system. The name
 is duplicated into the heap.\par
 This function can be called more than once and refreshes the internal
 buffer each time.
va: integer SUPPL error code
re(lated to): appFile appName appInit appPath appInitEnvEx
fi(le): app_ienv.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include "appname.loc"

#include "environ.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int appInitEnv(void)
{	DBG_ENTER("appInitEnv", Suppl_appName)
	DBG_RETURN_BI(appInit_(env_strdup(0, 0)))
}
