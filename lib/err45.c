/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_no_rw_device
void error_no_rw_device(const char * const devname)
{	displayError(TEXT_ERROR_NORW_DEVICE, devname);
}
