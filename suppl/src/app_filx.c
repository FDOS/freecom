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

ob(ject): appFileEx
su(bsystem): appName
ty(pe): 0F
sy(nopsis): char *appFileEx(void)
sh(ort description): Return the absolute filename of the application
lo(ng description): Returns a pointer to a static buffer that holds the
 absolute filename of the current application.\par
 The function initializes itself with appInitEnvEx.
va: NULL: if appInitEnvEx failed
 \item else: pointer to requested data
re(lated to): appFile appName appInit appPath appInitEnvEx
fi(le): app_filx.c
in(itialized by): appInitEnvEx

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include "appName.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *appFileEx(void)
{	DBG_ENTER("appFileEx", Suppl_appName)
	appInitEnvEx();
	DBG_RETURN_BS(appFile())
}
