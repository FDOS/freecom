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

	Declarations for the DOS filename functions.

*/

#ifndef __DFN_H
#define __DFN_H

#include <supl_def.h>
#ifndef __PORTABLE_H
#include <portable.h>
#endif
#include <nls_f.h>
#ifdef _TC_LATER_
#include <direct.h>
#endif

/* To support UNC paths, all functions are macro'ed from
	dfn*()   --->     dfnu*()
*/
#ifdef SUPPORT_UNC_PATH
#define dfnmerge dfnumerge
#define dfnsplit dfnusplit
#define dfnexpand dfnuexpand
#define dfnsqueeze dfnusqueeze
#define dfnmatch dfnumatch
#endif

#define DFN_FILE		0x4000
#define DFN_LFN			0x2000
#define DFN_DIRECTORY	0x0010
#define DFN_LABEL		0x0008
#define DFN_WRITE		0x0001
#define DFN_HIDDEN		0x0002
#define DFN_SYSTEM		0x0004
/* Derived attributes */
#define DFN_READ		DFN_FILE	/* every file can be read */

char *dfnexpand(const char * const fname, char * const path);
char *dfnmerge(char *fname, const char * const dr, const char * const path
 , const char * const name, const char * const ext);
int dfnsplit(char * const fname, char ** const dr, char ** const path, char ** const name, char ** const ext);
char *dfnsqueeze(const char * const fname);
char *dfnpath(int drive);
char *dfntruename(const char * const fn);
char *dfnsearch(char * const fname, char *path, char *ext);
int dfnmatch(char *fnam, char *pattern);
int isUNCpath(const char * const fname);
char *UNCpath(const char * const fname);
int dfnwrdir(const char * const fnam);
int dfnstat(const char * const fnam);
int dfnmkfile(const char * const fnam);
void dfn_tmpnam(char * const fnam);
char *dfnmktmp(const char * const path, const char * const ext);
char *dfnfilename(const char * const fnam);
char *dfnbakname(const char * const fnam);
int dfnfreplace(const char * const newname, const char * const oldname);
int dfndelim(const int ch);
int dfndelim2(const int ch);

#endif
