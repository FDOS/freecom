/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_ctty_excluded
void error_ctty_excluded(void)
{	displayError(TEXT_CTTY_NOTIMPLEMENTED);
}
