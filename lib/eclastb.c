/* $id$

	Return the ID of the topmost B context

	Return:
		0: on failure, message already displayed
*/

#include "../config.h"

#include <assert.h>
#include <stdio.h>

#include "../include/context.h"
#include "../err_fcts.h"

static ctxtEC_Batch_t bc;

#pragma argsused
static int lastB(unsigned id, char *buf, void * const arg)
{
	if(*(byte*)buf == EC_TAG_BATCH) {
		if(3 != sscanf(buf + 1, "%u|%lu %lu"
		 , &bc.ec_idFnam, &bc.ec_pos, &bc.ec_lnr)) {
		 	error_context_corrupted();
		 	return -2;
		}
		return 1;
	}
	return 0;
}

ctxtEC_Batch_t *ecLastB(void)
{
	ecEnum(CTXT_TAG_EXEC, 0, lastB, 0);
	return &bc;
}
