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

	Declarations for the application name/path etc. functions.
	For details refer to DOC\DETAILS.TEX

*/

#ifndef __APPNAME_H
#define __APPNAME_H

#ifndef __PORTABLE_H
#include <portable.h>
#endif
#ifndef __DFN_H
#include "dfn.h"
#endif

char *appName(void);
char *appPath(void);
char *appFile(void);
char *appNameEx(void);
char *appPathEx(void);
char *appFileEx(void);
int appInit(const char * const fnam);
int appInitEx(const char * const fnam);
int appInitEnv(void);
int appInitEnvEx(void);

#endif
