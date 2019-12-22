/*
    DBCS/MBCS feature
*/
#ifndef __MBCS_H
#define __MBCS_H

#ifndef H__PORTABLE_
#include <portable.h>
#endif

/* DBCS support */
int isDbcsLead(unsigned char ch);
int MbLen(const char *s);
char * MbStrchr(const char *s, int c);
char * MbStrrchr(const char *s, int c);
int MbStricmp(const char *s1, const char *s2);
int MbStrnicmp(const char *s1, const char *s2, size_t length);
char * CharPrev(const char *base, const char *s);
#define CharNext(s)    ((char *)(s) + MbLen(s))

#define TEST_DBCS
#undef TEST_DBCS

#if defined TEST_DBCS
int printf(const char * fmt, ...);
#endif

#ifdef _MICROC_
/* not supported: MICROC */
#else

int _fMbLen(const char far *s);

#endif

#endif /* __MBCS_H */

