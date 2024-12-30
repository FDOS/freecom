/* $Id$
 * MD & RD handler
 */

#include "../config.h"

#include <assert.h>
#include <io.h>

#include "../include/misc.h"
#include "../strings.h"
#include "../err_fcts.h"
#include "../include/command.h"
#include "../include/cmdline.h"
#include "../include/lfnfuncs.h"

static int optRecursiveMode, optQuiet;

#define COMMAND_EXTENSIONS
#ifdef COMMAND_EXTENSIONS
optScanFct(opt_md_rd)
{
  (void)arg;
  switch(ch) {
  case 'S': return optScanBool(optRecursiveMode);  /* rmdir */
  case 'P': return optScanBool(optRecursiveMode);  /* mkdir */
  case 'Q': return optScanBool(optQuiet);          /* don't display error and don't prompt to remove files/directories */
  }
  optErr();
  return E_Useage;
}

#define SCANNER_FCT opt_md_rd
#else
#define SCANNER_FCT 0
#endif

int mk_rd_dir(char *param, int (*func) (const char *, int optRecursiveMode, int optQuiet), char *fctname)
{	char **argv;
	int argc, opts;
	int rv;

	assert(func);

	/* initialize options */
	optRecursiveMode = optQuiet = 0;

	if((argv = scanCmdline(param, SCANNER_FCT, 0, &argc, &opts)) == 0)
		return E_Useage;

	if(argc != 1) {
		error_syntax(0);
		rv = 1;
	} else {
		cutBackslash(argv[0]);

		dprintf(("%s: '%s'\n", fctname, argv[0]));
		if((rv = func(argv[0], optRecursiveMode, optQuiet)) != 0) {
			if (!optQuiet) error_dirfct_failed(fctname, argv[0]);
		}
	}

	freep(argv);
	return rv;
}
