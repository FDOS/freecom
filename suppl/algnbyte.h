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
/* one byte alignment */
/* do not use with Micro-C ! */

#if defined(__WATCOMC__)
#if __WATCOMC__ > 1000
#pragma pack(__push,1);
#else
#pragma pack(1);
#endif
#elif defined(_MSC_VER) || defined(_QC)
    #pragma pack(1)
#elif defined(__ZTC__)
    #pragma ZTC align 1
#elif defined(__TURBOC__) && (__TURBOC__ > 0x202)
    #pragma option -a-
#endif
