/* $id$

	Encode a string into an internal format, nothing is quoted except
	the control characters (range 1..31; 0 == EOS).

	Return:
		0: on failure, error message already displayed
		else: pointer to dynamically allocated buffer
*/

#include "../config.h"

#include <assert.h>

#include <dynstr.h>

#include "../include/context.h"
#include "../include/misc.h"

char *esEncode(const char * const str)
{	char *s, *p;
	const char *q;
	int ch;

	assert(str);

		/* At maximum each characters is transformed into two bytes */
	if((p = s = emalloc(2 * strlen(str) + 1)) == 0)
		return 0;

	for(q = str; (ch = *p++ = *q++) != 0;)
		if((unsigned char)ch < 32) {
			p[-1] = ES_CLEAR_HIGH;
			*p++ = ch | 0x80;
		}

	return StrTrim(s);
}
