/*	$id$
	$Locker$	$Name$	$State$

	Add a dynamically duplicated string to a dynamically growing
	array.

	Displays any error messages.

	Return:
		internal error code

	$Log$
	Revision 1.1.2.1  2001/07/08 17:23:43  skaus
	Update #7

 */

#include "../config.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../include/cmdline.h"
#include "../include/ierror.h"
#include "../include/misc.h"
#include "../err_fcts.h"

int addArg(char ***Xarg
	, int * const argc
	, const char *sBeg
	, const char * const sEnd
	, const char * const prefix)
{	char **arg;

	assert(Xarg);
	assert(argc);
	assert(sEnd);
	assert(sBeg);
    assert(sEnd >= sBeg);

	/* add new entry for new argument */
	if((arg = realloc(*Xarg, (*argc + 2) * sizeof(char *))) ==  0
				/* create new entry */
	 || (*Xarg = arg
	     , (arg[(*argc)++] = unquote(sBeg, sEnd)) == 0)) {
	 	error_out_of_memory();
		freep(*Xarg);
		*Xarg = 0;
		*argc = 0;
		return E_NoMem;
	}

	chkHeap
	if(prefix && *prefix) {
		char *p, **q;
		int l1, l2;

		q = &arg[*argc - 1];
		assert(*q);
		if((p = erealloc(*q
		 , (l1 = strlen(prefix)) + (l2 = strlen(*q) + 1))) == 0) {
			freep(*Xarg);
			*Xarg = 0;
			*argc = 0;
			return E_NoMem;
		}
		memmove(&p[l1], p, l2);
		memcpy(p, prefix, l1);
		*q = p;
	}
	chkHeap
	arg[*argc] = 0;
    return E_None;
}
