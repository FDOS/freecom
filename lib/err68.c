/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_redirect_to_file
void error_redirect_to_file(const char * const fnam)
{	displayError(TEXT_ERROR_REDIRECT_TO_FILE, fnam);
}
