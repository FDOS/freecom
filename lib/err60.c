/* $Id$

	Wrapper for error strings
*/

#include "../config.h"

#include "../include/command.h"
#include "../err_fcts.h"
#include "../strings.h"

void error_no_alias_name(const char name[])
{
  displayString(TEXT_ALIAS_INVALID_NAME, name);
}
