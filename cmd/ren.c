/*
 * REN.C - rename command
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"

int cmd_rename(char *param)
{	char **argv;
	int argc, opts, ec = E_None;

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	if(argc < 2) {
		error_req_param_missing();
		ec = E_Useage;
	}
	else if(argc > 2) {
		error_too_many_parameters(param);
		ec = E_Useage;
	}
	else if(rename(argv[0], argv[1]) != 0) {
		perror("rename");
		ec = E_Other;
	}

	freep(argv);
	return ec;
}
