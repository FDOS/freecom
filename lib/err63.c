/* $Id$

	Wrapper for error strings
*/

#include "../config.h"

#include "../include/command.h"
#include "../err_fcts.h"
#include "../strings.h"

void error_no_context_after_swap(void)
{
	displayString(TEXT_ERROR_CONTEXT_AFTER_SWAP);
}
