/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_context_length
void error_context_length(unsigned long islen, unsigned maxlen)
{	displayError(TEXT_ERROR_CONTEXT_LENGTH, islen, maxlen);
}
