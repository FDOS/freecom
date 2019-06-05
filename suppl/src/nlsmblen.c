#include "initsupl.loc"

#include <portable.h>
#include "cntry.h"

#include "suppldbg.h"

int isDbcsLead(unsigned char ch)
{
#if defined(DBCS)
	if (ch >= 0x80) {
		unsigned char far *db = nlsInfo()->dbcsTbl;
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

