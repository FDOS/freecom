/* $Id$

	Purge one entry from the dynamic context that overflow
	the tag's sizemax.

	Return: 0 if one entry had been removed
*/

#include "../config.h"

#include <assert.h>
#include <environ.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

/* Weight two context items which one uses more memory than
	acutally is allowed to do so */
#if 0
static unsigned long weight(ctxt_info_t *info)
{	unsigned long n;

	n = (unsigned long)info->c_sizecur << 16;
	n /= (unsigned)info->c_sizemax;
	return n;
}
#else
#define weight(info)	\
	(((unsigned long)((unsigned)info->c_sizecur) << 16)		\
		/ (unsigned)info->c_sizemax)
#endif

int ctxtSinglePurge(void)
{	int i, heaviest;

	/* Otherwise a removeable entry is removed;
		those are:
			oldest item of history
			oldest item of dirstack */
	heaviest = 0;
	for(i = 0; ++i <= CTXT_LAST_TAG;) {
		ctxt_info_t *p = &ctxt_info[i];

						/* honor inconsitency of redundant info */
		if(p->c_sizecur > p->c_sizemax && p->c_nummin < p->c_nummax) {
			/* not in range -> check it */
			if(!heaviest || weight(p) > weight((&ctxt_info[heaviest])))
				heaviest = i;
		}
	}

	if(heaviest) {
		/* Remove only */
		ctxtGet(1, heaviest, ++ctxt_info[heaviest].c_nummin, 0);
		return E_None;
	}

	return E_NoMem;		/* Failed */
}
