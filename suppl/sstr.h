/* This file was automatically generated from SSTR.SRC */
/* $Id$
   $Locker$	$Name$	$State$

	Secure string functions.

	They accept NULLs as string/memory pointers.

   $Log$
   Revision 1.1  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 1.1  1999/01/18 05:32:29  ska
   Initial revision


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

#define stricmp Stricmp
#define strdup Strdup
#define strcoll Strcoll
#define stpcpy Stpcpy
#define strset Strset
#define strcpy Strcpy
#define strrev Strrev
#define strtok Strtok
#define strncmp Strncmp
#define strrchr Strrchr
#define strpbrk Strpbrk
#define memicmp Memicmp
#define strspn Strspn
#define memcpy Memcpy
#define memset Memset
#define strcspn Strcspn
#define strlwr Strlwr
#define memccpy Memccpy
#define stpcat Stpcat
#define strend Strend
#define strupr Strupr
#define strnicmp Strnicmp
#define strcat Strcat
#define strstr Strstr
#define strnset Strnset
#define strncpy Strncpy
#define memmove Memmove
#define strchr Strchr
#define strlen Strlen
#define strcmp Strcmp
#define memchr Memchr
#define strncat Strncat
#define memcmp Memcmp
#endif	/* defined(SUPPL_STR_REMAP) */

#endif
