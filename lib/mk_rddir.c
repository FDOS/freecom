/*
 * MD & RD handler
 */

#include "../config.h"

#include <assert.h>
#include <dir.h>

#include "../include/misc.h"
#include "../strings.h"
#include "../err_fcts.h"
#include "../include/cmdline.h"

int mk_rd_dir(char *param, int (*func) (const char *), char *fctname)
{	char **argv;
	int argc, opts;
	int rv;

	assert(func);

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	if(argc != 1) {
		error_syntax(0);
		rv = 1;
	} else {
		cutBackslash(argv[0]);

		dprintf(("%s: '%s'\n", fctname, argv[0]));
		if((rv = func(argv[0])) != 0)
			error_dirfct_failed(fctname, argv[0]);
	}

	freep(argv);
	return rv;
}
