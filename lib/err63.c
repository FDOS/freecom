/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_out_of_dos_memory
void error_out_of_dos_memory(void)
{	displayError(TEXT_ERROR_OUT_OF_DOS_MEMORY);
}
