/* $id$

	Returns the current line number of the currently active batch script
	or "-1"  if none.

*/

#include "../config.h"

#include "../include/command.h"
#include "../include/context.h"
#include "../include/misc.h"

#pragma argsused
char *fct_batchline(char *param)
{	ctxtEC_Batch_t far *bc;

	if((bc  = ecLastB()) != 0) {
		char buf[sizeof(unsigned long) * 8 + 2];
		sprintf(buf, "%lu", bc->ec_lnum);
		return estrdup(buf);
	}
	return estrdup("-1");
}
