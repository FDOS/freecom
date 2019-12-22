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
/* This file was automatically generated from SSTR.SRC */
/* $Id$
   $Locker$	$Name$	$State$

	Secure string functions.

	They accept NULLs as string/memory pointers.

*/

#ifndef __SSTR_H
#define __SSTR_H

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>

#define Strcmpi	Stricmp
#define Strncmpi	Strnicmp
#define Memcmpi	Memicmp

#ifdef DBCS
# include "mbcs.h"
#endif



/* Generated string function replacements */
void *Memccpy(void *dst, void const *src, int ch, size_t length);
void *Memchr(void const *s, int ch, size_t length);
int Memcmp(char const *s1, char const *s2, size_t length);
void *Memcpy(void *dst, void const *src, size_t length);
int Memicmp(char const *s1, char const *s2, size_t length);
void *Memmove(void *dst, void const *src, size_t length);
void *Memset(void *s, int ch, size_t length);
char *Stpcat(char *dst, char const *src);
char *Stpcpy(char *dst, char const *src);
char *Strcat(char *dst, char const *src);
char *Strchr(char const *s, int ch);
int Strcmp(char const *s1, char const *s2);
int Strcoll(char const *s1, char const *s2);
char *Strcpy(char *dst, char const *src);
size_t Strcspn(char const *str, char const *chset);
char *Strdup(char const *s);
int Stricmp(char const *s1, char const *s2);
size_t Strlen(char const *s);
char *Strlwr(char *s);
char *Strncat(char *dst, char const *src, size_t length);
int Strncmp(char const *s1, char const *s2, size_t length);
char *Strncpy(char *dst, char const *src, size_t length);
int Strnicmp(char const *s1, char const *s2, size_t length);
char *Strnset(char *s, int ch, size_t length);
char *Strpbrk(char const *str, char const *chset);
char *Strrchr(char const *s, int ch);
char *Strrev(char *s);
char *Strset(char *s, int ch);
size_t Strspn(char const *str, char const *chset);
char *Strstr(char const *str, char const *substr);
char *Strtok(char *str, char const *delim);
char *Strupr(char *s);
char *Strend(char *s);


#ifdef SUPPL_STR_REMAP
	/*Remap standard function names to replacements */

#define strupr Strupr
#define stricmp Stricmp
#define memmove Memmove
#define stpcpy Stpcpy
#define strspn Strspn
#define strchr Strchr
#define strnset Strnset
#define strcoll Strcoll
#define memicmp Memicmp
#define strnicmp Strnicmp
#define stpcat Stpcat
#define memccpy Memccpy
#define strcpy Strcpy
#define strcspn Strcspn
#define memcpy Memcpy
#define strlwr Strlwr
#define strncpy Strncpy
#define strlen Strlen
#define strstr Strstr
#define strrev Strrev
#define memset Memset
#define memchr Memchr
#define strcmp Strcmp
#define memcmp Memcmp
#define strncat Strncat
#define strcat Strcat
#define strdup Strdup
#define strtok Strtok
#define strncmp Strncmp
#define strrchr Strrchr
#define strset Strset
#define strpbrk Strpbrk
#define strend Strend
#endif	/* defined(SUPPL_STR_REMAP) */

#ifdef DBCS
# if 1
#  undef Stricmp
#  undef stricmp
#  define Stricmp MbStricmp
#  define stricmp MbStricmp
# endif
# if 1
#  undef Strnicmp
#  undef strnicmp
#  define Strnicmp MbStrnicmp
#  define strnicmp MbStrnicmp
# endif
# if 1
#  undef Strchr
#  undef strchr
#  define Strchr MbStrchr
#  define strchr MbStrchr
# endif
# if 1
#  undef Strrchr
#  undef strrchr
#  define Strrchr MbStrrchr
#  define strrchr MbStrrchr
# endif
#endif /* DBCS */

#endif
