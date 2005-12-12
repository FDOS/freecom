/* $Id$
 * MD & RD handler
 */

#include "../config.h"

#include <assert.h>
#include <dir.h>

#include "../include/misc.h"
#include "../strings.h"
#include "../err_fcts.h"
#include "../include/cmdline.h"


#ifdef FEATURE_LONG_FILENAMES
int lfn_mrc_dir(const char *path, int func)
{
	struct REGPACK r;
	if (checkDriveSupportsLFN(getdisk() + 'A'))
        func = (func >> 8) + 0x7100;
	r.r_ax = func;
	r.r_ds = FP_SEG(path);
	r.r_dx = FP_OFF(path);
	intr(0x21, &r);
	return (r.r_flags & 1) ? -1 : 0;
}

#define mkdir(x) lfn_mrc_dir(x,0x3900)
#define rmdir(x) lfn_mrc_dir(x,0x3A00)
#define chdir(x) lfn_mrc_dir(x,0x3B00)
#endif


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
