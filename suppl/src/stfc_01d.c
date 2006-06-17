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
/* DON'T MODIFY THIS FILE!

	This is an auto-generated file (by mkstrfct.pl), don't
	modify because next time the script runs, your modifications
	are lost.

	Defines one of the string functions replacements that accept
	NULL as pointer arguments.
*/

#include "initsupl.loc"

#ifndef _MICROC_ 
#include <string.h>
#endif
#include <portable.h>
#include "suppl.h"
#ifdef SUPPL_STR_REMAP
#undef SUPPL_STR_REMAP
#endif
#include "sstr.h"

size_t Strspn(char const *str, char const *chset)
{
	return str && chset? strspn(str, chset): 0;
}
