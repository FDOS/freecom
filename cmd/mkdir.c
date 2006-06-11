/* $Id$
 * MD / MKDIR - makes a call to directory_handler to do its work
 */

#include "../config.h"

#include <dir.h>

#include "../include/command.h"
#include "../include/misc.h"

int cmd_mkdir(char *param)
{
#ifdef FEATURE_LONG_FILENAMES
    return mk_rd_dir( param, 0x39, "MKDIR" );
#else
	return mk_rd_dir(param, mkdir, "MKDIR");
#endif
}
