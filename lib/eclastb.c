/* $id$

	Return the ID of the topmost B context

	Return:
		0: on failure, message already displayed
*/

#include "../config.h"

#include <assert.h>
#include <stdio.h>

#include "../include/context.h"
#include "../include/ierror.h"
#include "../err_fcts.h"

static ctxtEC_Batch_t bc;

#pragma argsused
static int lastB(unsigned id, char *ctxt, void * const arg)
{
	if(*(byte*)ctxt == EC_TAG_BATCH) {
		if(ecScanArg(ctxt, 3, "%u|%lu %lu"
		 , &bc.ec_idFnam, &bc.ec_pos, &bc.ec_lnr) != E_None)
		 	return -2;
		return 1;
	}
	return 0;
}

ctxtEC_Batch_t *ecLastB(void)
{
	if(ecEnum(CTXT_TAG_EXEC, 0, lastB, 0) == 1)
		return &bc;
	return 0;
}
