/*
 * CDD & CD handler
 */

#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dir.h>

#include "../include/cmdline.h"
#include "../include/misc.h"
#include "../strings.h"
#include "../err_fcts.h"

int cd_dir(char *param, int cdd, const char * const fctname)
{	char **argv, *dir;
	int argc, opts;
	int rv, freeDir;

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	freeDir = 0;
	rv = 1;

	/* if doing a CD and no parameters given, print out current directory */
	if(argc == 0) {
		if((dir = cwd(0)) != 0) {
			puts(dir);
			freeDir = 1;
			goto okRet;
		}
		goto errRet;
	} else if(argc != 1) {
		error_syntax(0);
		goto errRet;
	}
	else {
		assert(argv[0]);
		dir = strchr(argv[0], '\0');
		/* take off trailing \ if any, but ONLY if dir is not the root dir */
		while(dir > &argv[0][1] && *--dir == '\\' && dir[-1] != ':')
			*dir = '\0';

		dir = argv[0];
#ifdef FEATURE_LAST_DIR
		if(strcmp(dir, "-") == 0) {
			assert(!freeDir);
			/* change to last directory */
			lastDirGet(&dir);
			freeDir = 1;
		}

		lastDirSet();
		if(!dir) 	/* "CD -" without a CD before at all */
			goto okRet;
#endif
		if(*dir && dir[1] == ':') {
			if(cdd) {
				if(changeDrive(*dir) != 0)
					goto errRet;
				if(!dir[2])		/* only change drive */
					goto okRet;
			} else if(!dir[2]) {	/* Real CHDIR displays CWD of
										specified drive */
				assert(freeDir == 0);

				if((dir = cwd(*dir)) != 0) {
					puts(dir);
					freeDir = 1;
					goto okRet;
				}
				goto errRet;
			}
		}
		dprintf(("%s: '%s'\n", fctname, dir));
		if(chdir(dir) != 0) {
			error_dirfct_failed(fctname, dir);
			goto errRet;
		}
	}

okRet:
	rv = 0;
errRet:
	freep(argv);
	if(freeDir)
		free(dir);
	return rv;
}
