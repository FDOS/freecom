/* $Id$

	Internal command ARG

	ARG [ shift_level arg0 { arg } ]

 */


#include "../config.h"

#include <assert.h>
#include <stdlib.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"
#include "../strings.h"

int cmd_arg(char *param)
{
	char **argv;
	int argc, opts, ec = E_None;

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	/* Because no option was passed into scanCmdline() no
		option can have been processed */
	assert(opts == 0);

		/* Reset arguments */
	ctxtClear(CTXT_TAG_ARG);
	ctxtFlags.f_shiftlevel = 0;

	if(argc > 1) {
		ctxtFlags.f_shiftlevel = atoi(argv[0]);
		for(argc = 1
		 ; argv[argc]
		    && (ec = ctxtSet(CTXT_TAG_ARG, argc, argv[argc])) == E_None
		 ; ++argc);
	}

	freep(argv);
	return ec;
}
