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

ob(ject): app_file
su(bsystem): appName
ty(pe): 0FL
sy(nopsis): char *app_file
sh(ort description): Dynamically buffered filename of application
he(ader files): appname.loc
va:  NULL: not initialized so far
	\item else: pointer to dynamically duplicated filename
re(lated to): appPath appFile appInit appName appInitEnv
fi(le): app_vars.c
in(itialized by): static appInit appInitEx appInitEnv appInitEnvEx appNameEx
 appFileEx appPathEx

ob(ject): app_fnam
su(bsystem): appName
ty(pe): 0FL
sy(nopsis): char *app_fnam
sh(ort description): Pointer to backslash before filename within app_file
he(ader files): appname.loc
va:  undefined: if not initialized (app_file == NULL)
re(lated to): appPath appFile appInit appName appInitEnv
fi(le): app_vars.c
in(itialized by): appInit appInitEx appInitEnv appInitEnvEx appNameEx
 appFileEx appPathEx

ob(ject): app_fext
su(bsystem): appName
ty(pe): 0FL
sy(nopsis): char *app_fext
sh(ort description): Pointer to dot of filename portion within app_file
he(ader files): appname.loc
va:  NULL: if file has no extension
re(lated to): appPath appFile appInit appName appInitEnv
fi(le): app_vars.c
in(itialized by): appInit appInitEx appInitEnv appInitEnvEx appNameEx
 appFileEx appPathEx

ob(ject): app_fnam
su(bsystem): appName
ty(pe): 0FL
sy(nopsis): char *app_fnam
sh(ort description): Pointer to backslash before filename within app_file
he(ader files): appname.loc
va:  undefined: if not initialized (app_file == NULL)
re(lated to): appPath appFile appInit appName appInitEnv
fi(le): app_vars.c
in(itialized by): appInit appInitEx appInitEnv appInitEnvEx appNameEx
 appFileEx appPathEx

*/

#include "initsupl.loc"

#include "appname.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *app_file = 0			/* indicate: not initialized */
	, *app_fext, *app_fnam;
