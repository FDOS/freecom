/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/message.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_context_out_of_memory
void error_context_out_of_memory(void)
{	displayError(TEXT_ERROR_CONTEXT_OUT_OF_MEMORY);
}
