/* $Id$

	Display an out-of-memory condition.
	Also: Set the doCancel flag in order to break all running
	stuff.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/context.h"
#include "../include/message.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_out_of_memory
void error_out_of_memory(void)
{	displayError(TEXT_ERROR_OUT_OF_MEMORY);
	doCancel = 1;
}
