/*	$id$
	$Locker$	$Name$	$State$

	Display all the internal status pages optionally into a file

	$Log$
	Revision 1.1.2.5  2001/08/02 21:51:56  skaus
	Update #13

	Revision 1.1.2.4  2001/07/25 20:17:28  skaus
	Update #12
	
	Revision 1.1.2.1  2001/07/16 20:28:38  skaus
	Update #9
	
 */

#include "../config.h"

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../strings.h"
#include "../err_fcts.h"

int cmd_dispStatus(char *param)
{	FILE *oldOut;
	char **argv;
	int argc, opts, newStream;
	char *time, *date;

	chkHeap
	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	chkHeap
	/* Because no option was passed into scanCmdline() no
		option can have been processed */
	assert(opts == 0);

	time = date = 0;
	newStream = 0;
	if(argc) {
		/* exchange the current output stream by the given file */
		assert(argv[0]);
		if(strcmp(argv[0], "-") != 0) {
			oldOut = XoutStream;
			newStream = 1;
			if((XoutStream = fopen(argv[0], "at")) == 0) {
				error_redirect_to_file(argv[0]);
				goto errRet;
			}
		}
	}

	chkHeap
	if((time = curTime()) == 0
	 || (date = curDateLong()) == 0)
		goto errRet;

	chkHeap
	displayString(TEXT_DISP_STATUS_HDR, date, time);
	if(argc > 1)
		displayString(TEXT_DISP_STATUS_COMMAND, param);

	chkHeap
	displayString(TEXT_DISP_STATUS_MEMORY);
	chkHeap
	cmd_memory(0);
	chkHeap
	displayString(TEXT_DISP_STATUS_ALIAS);
	chkHeap
	cmd_alias(0);
	chkHeap
	displayString(TEXT_DISP_STATUS_HISTORY);
	chkHeap
	cmd_history(0);
	chkHeap
	displayString(TEXT_DISP_STATUS_NLS);
	chkHeap
	cmd_dispNLS(0);
	chkHeap
	displayString(TEXT_DISP_STATUS_FLAGS);
	chkHeap
	cmd_dispFlags(0);
	chkHeap
	displayString(TEXT_DISP_STATUS_IVARS);
	chkHeap
	cmd_dispIVars(0);
	chkHeap
	displayString(TEXT_DISP_STATUS_ARGS, gflag_base_shiftlevel, gflag_shiftlevel);
	chkHeap
	cmd_dispArgs(0);
	chkHeap
	displayString(TEXT_DISP_STATUS_EXEC);
	chkHeap
	cmd_dispEC(0);
	chkHeap

	displayString(TEXT_DISP_STATUS_FOOTER);
	chkHeap

errRet:
	chkHeap
	if(newStream) {
		fclose(XoutStream);
		XoutStream = oldOut;
	}

	chkHeap
	myfree(time);
	chkHeap
	myfree(date);
	chkHeap
	freep(argv);
	chkHeap
	return 0;
}
