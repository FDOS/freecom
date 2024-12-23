/* $Id$
 * RD / RMDIR - makes a call to directory_handler to do its work
 */

#include "../config.h"

#include <io.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/lfnfuncs.h"

int recursive_rmdir(const char * path, int optRecursiveMode, int optQuiet)
{
	/* optQuiet effects parsing showing errors, but no effect on recursive call */
	(void) optQuiet;
	
	if (optRecursiveMode) {
		return 0;
	} else {
		return rmdir(path);
	}
}

int cmd_rmdir(char *param)
{
	return mk_rd_dir(param, recursive_rmdir, "RMDIR");
}
