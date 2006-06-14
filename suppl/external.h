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
#ifndef __EXTERNAL_H
#define __EXTERNAL_H

extern char *ARGV[];
extern int ARGC;
extern unsigned PSP;
extern unsigned _AX_, _BX_, _CX_, _DX_, _DI_, _SI_;
extern unsigned char Longreg[];
extern unsigned IOB_size;

#define fileno(a) ((a)->FILE_handle)
#endif
