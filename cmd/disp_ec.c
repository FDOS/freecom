/*	$id$
	$Locker$	$Name$	$State$

	Display the internal execution context

	$Log$
	Revision 1.1.2.4  2001/07/25 20:17:28  skaus
	Update #12

	Revision 1.1.2.1  2001/07/16 20:28:38  skaus
	Update #9
	
 */

#include "../config.h"

#include <fmemory.h>
#include <suppl.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../strings.h"
#include "../err_fcts.h"

#pragma argsused
int cmd_dispEC(char *param)
{	ctxtEC_t far *ec;
	unsigned len, cnt;
	ecTag_t tag;
	void far*limit;

	assert(ctxtMain);
	
		/* absolute bottom of dynamic context */
	for(cnt = 0, ec = ctxtExecContext
	 ; ec && FP_SEG(ec) < nxtMCB(FP_SEG(ctxtMCB))
	 ; ec = _fnormalize(limit)) {
		++cnt;
		len = ec->ctxt_length;
		limit = (byte far*)ec + len + sizeof(*ec);
	 	if((tag = ec->ctxt_type) > EC_LAST_TAG)
	 		displayString(TEXT_EC_HDR_UNKNOWN, cnt, ec, tag, len);
	 	else
	 		displayString(TEXT_EC_HDR, cnt, ec, ecNames[tag], len);

			/* Does this context overflows the allocated bytes within
				the execution context stack? */
		if(addu(&len, FP_OFF(ec) + sizeof(*ec))
		 || (nxtMCB(FP_SEG(ctxtMCB)) - FP_SEG(ec)) * 16 < ec->ctxt_length) {
		  	error_context_corrupted();
		  	return 0;
		}

		chkHeap
		switch(tag) {
		case EC_TAG_BATCH:		/* Batch context */
			{	ctxtEC_Batch_t far*bc;

				bc = ecData(ec, ctxtEC_Batch_t);
				if(ec->ctxt_length < sizeof(ctxtEC_Batch_t)
				 || _fstrchr(bc->ec_fname, '\0') + 1 != limit) {
					error_context_corrupted();
					return 0;
				}
				displayString(TEXT_EC_DISP_BATCH
				 , bc->ec_pos
				 , bc->ec_lnum
				 , bc->ec_fname);
			}
			break;
		case EC_TAG_FOR_FIRST:	/* FOR primary context */
			{	char far*pattern;
				char far*varname;
				char far*cmd;

				pattern = ecData(ec, char);
				varname = _fstrchr(pattern, '\0') + 1;
				cmd = _fstrchr(varname, '\0') + 1;
				if(varname >= limit || cmd >= limit
				 || _fstrchr(cmd, '\0') + 1 != limit) {
					error_context_corrupted();
					return 0;
				}
				displayString(TEXT_EC_DISP_FOR1, varname, pattern, cmd);
			}
			break;
		case EC_TAG_FOR_NEXT:	/* FOR secondary context */
			{	ctxtEC_For_t far*fc;

				fc = ecData(ec, ctxtEC_For_t);
				if(ec->ctxt_length < sizeof(ctxtEC_For_t)
				 || _fstrchr(fc->ec_prefix, '\0') + 1 != limit) {
					error_context_corrupted();
					return 0;
				}
				displayString(TEXT_EC_DISP_FOR2
				 , MK_FP(ctxtMain, fc->ec_varname)
				 , fc->ec_prefix
				 , MK_FP(ctxtMain, fc->ec_cmd));
			}
			break;
		case EC_TAG_COMMAND:	/* command */
			{	ctxtEC_Cmd_t far*cc;

				cc = ecData(ec, ctxtEC_Cmd_t);
				displayString(TEXT_EC_DISP_CMD_1, cc->ec_flags);
				if(cc->ec_flags & EC_CMD_IGNORE_EXIT)
					displayString(TEXT_EC_DISP_CMD_IGNORE_EXIT);
				if(cc->ec_flags & EC_CMD_FORCE_INTERNAL)
					displayString(TEXT_EC_DISP_CMD_FORCE_INTERNAL);
				if(cc->ec_flags & EC_CMD_SILENT)
					displayString(TEXT_EC_DISP_CMD_SILENT);
				if(cc->ec_flags & EC_CMD_NO_TRACE)
					displayString(TEXT_EC_DISP_CMD_NO_TRACE);
				displayString(TEXT_EC_DISP_CMD_2, cc->ec_cmd);
			}
			break;
		}
	}
	displayString(TEXT_EC_FOOTER, ec);

	return 1;
}
