/* $Id$
 * MD / MKDIR - makes a call to directory_handler to do its work
 */

#include "../config.h"

#include <dir.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/lfnfuncs.h"

int cmd_mkdir(char *param)
{
#ifdef FEATURE_LONG_FILENAMES
    return mk_rd_dir( param, ( !__supportlfns ) ? 0x3900 : 0x7139, "MKDIR" );
#else
	return mk_rd_dir(param, mkdir, "MKDIR");
#endif
}
