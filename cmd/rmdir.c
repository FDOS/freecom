/* $Id$
 * RD / RMDIR - makes a call to directory_handler to do its work
 */

#include "../config.h"

#include <io.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/lfnfuncs.h"

int cmd_rmdir(char *param)
{
	return mk_rd_dir(param, rmdir, "RMDIR");
}
