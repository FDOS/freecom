/*
 *  TYPE.C - type internal command
 *
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/openf.h"
#include "../strings.h"

int cmd_type(char *param)
{
	char buf[256];
	char **argv;
	int argc, opts, ec = E_None;
	FILE *f;

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	/* Because no option was passed into scanCmdline() no
		option can have been processed */
	assert(opts == 0);

	if(!argc) {
		error_req_param_missing();
		ec = E_Useage;
		goto errRet;
	}

	for(argc = 0; argv[argc]; ++argc) {
		if((f = fdevopen(argv[argc], "rt")) == 0) {
			error_sfile_not_found(argv[argc]);
			ec = E_Other;
			break;
		}

		while(fgets(buf, sizeof(buf), f)) {
			if(cbreak) {
				dprintf(("[TYPE: Quit because of ^Break]\n"));
				fclose(f);
				ec = E_CBreak;
				goto errRet;
			}
			fputs(buf, stdout);
		}
		fclose(f);
		if(cbreak) {
			dprintf(("[TYPE: Quit because of ^Break]\n"));
			ec = E_CBreak;
			break;
		}
	}

errRet:
	freep(argv);

	return ec;
}
