/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_init_fully_qualified
void error_init_fully_qualified(const char * const s)
{	displayError(TEXT_INIT_FULLY_QUALIFIED, s);
}
