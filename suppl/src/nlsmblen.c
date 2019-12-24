#include "initsupl.loc"

#include <portable.h>
#include "cntry.h"
#include "nls_c.h"

#include "suppldbg.h"

int isDbcsLead(unsigned char ch)
{
#if defined(DBCS)
	if (ch >= 0x80) {
		unsigned char far *db = (unsigned char far *)(nlsInfo()->dbcsTbl);
		if (db) while(db[1]) {
			if (*db <= ch && ch <= db[1])
				return 1;
			db += 2;
		}
	}
#endif
	return 0;
}

#if defined(DBCS)
# define tmpl_mblen(s) \
	if ((s) && (unsigned char)(*(s)) >= 0x20 && (unsigned char)((s)[1]) >= 0x20) { \
		return isDbcsLead(*(s)) ? 2 : 1; \
	} \
	return 1;
#else
# define tmpl_mblen(s) (void)(s); return 1;
#endif

int MbLen(const char *s)
{
	tmpl_mblen(s);
}

int _fMbLen(const char far *s)
{
	tmpl_mblen(s);
}

#ifdef TEST_DBCS
char * MbStrchr2(const char *s, int c);
char * MbStrchr(const char *s, int c)
{
    char *d;
    printf("strchr(\"%s\", '%c' (%u))\n", s, c, c);
    d = MbStrchr2(s,c);
    printf("result: \"%s\"\n", d ? d : "(null)");
    return d;
}
# define MbStrchr MbStrchr2
#endif
char * MbStrchr(const char *s, int c)
{
	while(1) {
		if (*s == (char)c) return (char *)s;
		if (*s == '\0') break;
		s += MbLen(s);
	}
	return NULL;
}

char * MbStrrchr(const char *s, int c)
{
	const char *p = NULL;
	while(1) {
		if (*s == (char)c) p = s;
		if (*s == '\0') break;
		s += MbLen(s);
	}
	return (char *)p;
}

char * CharPrev(const char *base, const char *s)
{
	const char *p = base;
	while(p < s) {
		p += MbLen(p);
	}
	return (char *)p;
}

int MbStricmp(const char *s1, const char *s2)
{
	int d;
	while(1) {
		int c1, c2;
		int n1, n2;
		n1 = MbLen(s1);
		n2 = MbLen(s2);
		c1 = (n1 == 1) ? toUpper(*s1) : *s1;
		c2 = (n2 == 1) ? toUpper(*s2) : *s2;
		d = c1 - c2;
		if (d != 0 || (*s1 == '\0' /* && *s2 == '\0' */)) break;
		s1 += n1;
		s2 += n2;
	}
	return d;
}

int MbStrnicmp(const char *s1, const char *s2, size_t length)
{
	int d = 0;
	while(length--) {
		int c1, c2;
		int n1, n2;
		n1 = MbLen(s1);
		n2 = MbLen(s2);
		c1 = (n1 == 1) ? toUpper(*s1) : *s1;
		c2 = (n2 == 1) ? toUpper(*s2) : *s2;
		d = c1 - c2;
		if (d != 0 || (*s1 == '\0' /* && *s2 == '\0' */)) break;
		s1 += n1;
		s2 += n2;
	}
	return d;
}

