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

   SUPPL message implementation

	Each of these functions emit one particular error.

	One can create a library implementing just these functions and
	link it _before_ SUPPL, and one gets an user-specific error handling.

*/

#ifndef __MSGS_H
#define __MSGS_H

#include <portable.h>
#include "msgs_w.loc"

void Esuppl_argMissing(char ch);
void Esuppl_nonArgOption(char ch);
void Esuppl_argOption(char ch);
void Esuppl_lArgOption(const char * const name);
void Esuppl_option(char ch);		/* option not found */
void Esuppl_lOption(const char * const name);	/* option not found */
void Esuppl_longOption(void);
void Esuppl_noMem(void);
void Esuppl_writeFile(const char * const fnam);
void Esuppl_accessFile(const char * const fnam);
void Esuppl_openFile(const char * const fnam);
void Esuppl_createFile(const char * const fnam);
void Esuppl_readFile(const char * const fnam);
void Esuppl_creatTmpFnam(void);
void Esuppl_tmpOpen(const char * const fnam);
void Esuppl_wrCfgFile(const char *fnam);
void Esuppl_ovrLinkcount(const char *fnam);

extern void hlpScreen(void);
extern const char Y_noFNam[];

#endif
