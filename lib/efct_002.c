/*	$Id$

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_no_cwd
void error_no_cwd(int drive)
{	displayError(TEXT_ERROR_CWD_FAILED, drvNum(drive) + 'A');
}
