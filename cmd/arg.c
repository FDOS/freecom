/* $Id$

	Internal command ARG

	ARG [ base_shiftlevel first_item_to_remove shift_level ]

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
	int argc, opts;
	unsigned to_del, base, shiftlevel;

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	/* Because no option was passed into scanCmdline() no
		option can have been processed */
	assert(opts == 0);

	shiftlevel = base = to_del = 0;		/* for argc == 0 */
	if(argc == 0 
	 || argc == 3 && is_num(argv[0], &base) && is_num(argv[1], &to_del)
	     && is_num(argv[2], &shiftlevel) && base <= to_del) {
	 		if(!to_del)
	 			ctxtClear(CTXT_TAG_ARG);
	 		else {
	 			while(to_del <= CTXT_INFO(CTXT_TAG_ARG, nummax))
	 				ctxtPop(CTXT_TAG_ARG, (char*)0);
	 		}
			F(shiftlevel) = shiftlevel;
			F(base_shiftlevel) = base;
			freep(argv);
			return 0;
	}

	error_syntax(0);
	freep(argv);
	return 1;
}
