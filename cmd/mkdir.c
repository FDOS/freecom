/* $Id$
 * MD / MKDIR - makes a call to directory_handler to do its work
 */

#include "../config.h"

#include <sys/stat.h>
#include <string.h>

#include "suppl.h"
#include "dfn.h"

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/lfnfuncs.h"

int recursive_mkdir(const char * path, int optRecursiveMode, int optQuiet)
{
	if (optRecursiveMode) {
		char fullname[MAXPATH];
		char *p = fullname;
		int flag_not_done = 0;
		int ret;
		strcpy(fullname, path); /* so we can modify in place */
		dprintf(("fullpath = %s\n", fullname));
		do {
			while (*p && ((*p != '\\') && (*p != '/'))) {
				p++;
			}
			flag_not_done = *p; /* == 0 when end of path found, nonzero if \ or / */
			*p = '\0';
			dprintf(("mkdir(%s)\n", fullname));
			/* validate if path exists and is a directory */
			ret = dfnstat(fullname);
			if (ret) { /* found, verify it is a directory */
				if (!(ret & DFN_DIRECTORY)) {
					/* path component exists but not a directory */
					ret = E_Other;
				} else {
					ret = 0;  /* no error */
				}
			} else { /* not found (or other error) so attempt to make directory */
				ret = mkdir(fullname);
			}
			if (ret) {
				if (!optQuiet) {
					/* TODO show error message */
				}
				return ret;
			}
			if (flag_not_done)
				*p++ = '\\';
		} while(flag_not_done);		
		return ret;
	} else {
		return mkdir(path);
	}
}

int cmd_mkdir(char *param)
{
	return mk_rd_dir(param, recursive_mkdir, "MKDIR");
}


