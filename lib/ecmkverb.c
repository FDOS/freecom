/* $id$

	Make a string suitable to "%@verbatim" a passed in string.

	Return:
		0: on error, message already displayed
		else: dynamically allocated string
*/

#include "../config.h"

#include <stdlib.h>
#include <string.h>

#include <dynstr.h>

#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"

char *ecMkVerbatimStr(const char * const str)
{	char *verb;
	int i;

	if((verb = estrdup(" ")) == 0)
		return 0;

	do {
		i = 0;
		while(++verb[i] == '\0') {
			verb[i] = ' ' + 1;
			if(!verb[++i]) {		/* end of string --> append a character */
				if(!StrAppChr(verb, ' ' + 1)) {
					free(verb);
					error_out_of_memory();
					return 0;
				}
				break;
			}
		}
	} while(strstr(str, verb));

	return verb;
}
