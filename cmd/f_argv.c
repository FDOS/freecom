/* $Id$

	function: argv(first,last)

	Returns a number of arguments as returned by %0, %1 a.s.o

	if "last" (and the comma) is absent, only one argument is returned.
	if "last" < "first", nothing is returned.

	If a number is prefixed by a plus sign, the number is taken
	relative to the current shiftlevel, otherwise the indexes are
	absolute ones.

	A missing "first" is replaced by the current shiftlevel;
	a missing "last" (behind the comma) is replaced by the
	maximum number of arguments.

	If the argument is absent or empty (neither a comma),
	all arguments are returned ignoring the shiftlevel.

	Return:
		0: on error (error message already displayed)
		else: pointer to dynamically allocated buffer
*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <dynstr.h>
#include <suppl.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/cmdline.h"
#include "../include/misc.h"
#include "../err_fcts.h"

static void fetchNumber(const char * const line, unsigned * const num)
{	const char *p;
	int disp;

	assert(num);
	assert(line);

	disp = 0;
	if(*(p = line) == '+') {
		++p;
		disp = gflag_shiftlevel;
	}

	if(is_num(p, num) && !addu(num, disp))
		return;

	*num = (unsigned)-1;
	error_invalid_number(line);
	return;
}

char *fct_argv(char *param)
{	unsigned first, last;
	char *p, *q;
	char *argv[2];	

	if(splitFctLine(2, argv, param) != E_None)
		return 0;

	last = (unsigned)~0;
	if(argv[0]) {
		fetchNumber(argv[0], &first);
		if(argv[1])
			fetchNumber(argv[1], &last);
	} else
		first = 1;

	if(addu(&first, gflag_base_shiftlevel))
		return 0;
	if(addu(&last, gflag_base_shiftlevel))
		last = (unsigned)-1;

	if(last > CTXT_INFO(CTXT_TAG_ARG, nummax))
		last = CTXT_INFO(CTXT_TAG_ARG, nummax);

	if(first > last)
		return 0;

	for(p = 0; first <= last; ++first) {
		q  = 0;
		if(ctxtGet(0, CTXT_TAG_ARG, first, &q) > 1) {
			myfree(p);
			return 0;
		}
		if((p && !StrAppChr(p, ' ')) || !StrCat(p, q)) {
			error_out_of_memory();
			myfree(p);
			return 0;
		}
	}

	return p;
}
