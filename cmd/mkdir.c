/* $Id$
 * MD / MKDIR - makes a call to directory_handler to do its work
 */

#include "../config.h"

#include <sys/stat.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/lfnfuncs.h"

int recursive_mkdir(const char * path, int optRecursiveMode, int optQuiet)
{
	if (optRecursiveMode) {
		return 0;
	} else {
		return mkdir(path);
	}
}

int cmd_mkdir(char *param)
{
	return mk_rd_dir(param, recursive_mkdir, "MKDIR");
}


