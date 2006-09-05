/* $Id$

	Scan status from context
	Assumes that the entries are in "init" state!!

*/

#include "../config.h"

#include <assert.h>
#include <ctype.h>

#include "environ.h"
#include "../include/context.h"

static int scan (void * arg, word segm, word ofs) {
        Context_Tag tag;
	ctxt_info_t *info;

        (void)arg;
	assert(segm);
	assert(ofs != (word)-1);

redo:
	if((tag = peekb(segm, ofs), ctxtIsInfoTag(tag))) {
		info = &CTXT_INFO_STRUCT(tag);
		if(peekb(segm, ofs + 1) == '=' && peekb(segm, ofs + 4) == 0) {
			/* status entry */
			info->c_sizemax = peekw(segm, ofs + 2) & ~0x8001;
		} else {		/* normal entry */
			char far *p = MK_FP(segm, ofs + 1);
			unsigned num;

			for(num = 0; isxdigit(*p); ++p) {
				num <<= 8;
				if(isdigit(*p))
					num |= *p & 0xf;
				else {
					assert(islower(*p));
					num |= *p - 'a' + 10;
				}
			}
			if(*p == '=') {
				info->c_sizecur += env_varlen(segm, ofs);
				if(num < info->c_nummin)
					info->c_nummin = num;
				if(num > info->c_nummax)
					info->c_nummax = num;
			} else {
				/* Invalid entry -> remove to make room for useful stuff */
				env_subVarOffset(segm, ofs);
				goto redo;
			}
		}
	} else if(tag > ' ') {			/* Alias */
		CTXT_INFO(CTXT_TAG_ALIAS, sizecur) += env_varlen(segm ,ofs);
	}

	return 0;		/* proceed */
}

void ctxtScanStatus(void)
{
	assert(ctxt);

	env_forAll(ctxt, scan, 0);
}
