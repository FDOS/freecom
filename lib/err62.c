/* $Id$

	Wrapper for error strings
*/

#include "../config.h"

#include "../include/command.h"
#include "../err_fcts.h"
#include "../strings.h"

void error_context_out_of_memory(void)
{
	displayString(TEXT_ERROR_CONTEXT_OUT_OF_MEMORY);
}
