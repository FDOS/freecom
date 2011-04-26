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
#include "../include/lfnfuncs.h"


#ifdef FEATURE_LONG_FILENAMES
int lfn_mrc_dir( const char *path, int func )
{
	struct REGPACK r;
    r.r_ax = func;
    r.r_dx = FP_OFF( path );
	r.r_ds = FP_SEG( path );
    intr( 0x21, &r );
    if( func == 0x7139 && ( ( r.r_flags & 1 ) || r.r_ax == 0x7100 ) ) {
        r.r_ax = func << 8;
        intr( 0x21, &r );
    }
    return( -( r.r_flags & 1 ) );
}

#endif


#ifdef FEATURE_LONG_FILENAMES
int mk_rd_dir( char *param, int lfnfunc, char *fctname )
#else
int mk_rd_dir(char *param, int (*func) (const char *), char *fctname)
#endif
{	char **argv;
	int argc, opts;
	int rv;

#ifndef FEATURE_LONG_FILENAMES
	assert(func);
#endif

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	if(argc != 1) {
		error_syntax(0);
		rv = 1;
	} else {
		cutBackslash(argv[0]);

		dprintf(("%s: '%s'\n", fctname, argv[0]));
#ifdef FEATURE_LONG_FILENAMES
        if((rv = lfn_mrc_dir( getshortfilename( argv[0] ), lfnfunc )) != 0)
#else
		if((rv = func(argv[0])) != 0)
#endif
			error_dirfct_failed(fctname, argv[0]);
	}

	freep(argv);
	return rv;
}
