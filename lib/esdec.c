/* $id$

	Decode a string from the internal format.

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include "../include/context.h"

void esDecode(void * const dst, const char * const src, int maxlen)
{	const char *p;
	char *q;
	int ch;

	assert(src);
	assert(dst);

	p = src;
	q = dst;
	while(maxlen-- && (ch = *q++ = *p++) != 0) switch(ch) {
	case ES_CLEAR_HIGH:
		if(*p)
			q[-1] = *p++ & 0x7f;
		else --q;
		break;
	case ES_SET_HIGH:
		if(*p)
			q[-1] = *p++ | 0x80;
		else --q;
		break;
	case ES_STRING_DELIM:
	case ES_PAD_BYTE:
		--q;
		break;
	}
}
