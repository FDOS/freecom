/*	$id$
	$Locker$	$Name$	$State$

	Display the internal execution context

	$Log$
	Revision 1.1.2.6  2001/08/02 21:51:56  skaus
	Update #13

	Revision 1.1.2.5  2001/07/30 00:45:17  skaus
	Update #13 / Beta 27: plain dynamic context
	
	Revision 1.1.2.4  2001/07/25 20:17:28  skaus
	Update #12
	
	Revision 1.1.2.1  2001/07/16 20:28:38  skaus
	Update #9
	
 */

#include "../config.h"

#include <dynstr.h>
#include <fmemory.h>
#include <suppl.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../strings.h"
#include "../err_fcts.h"

#pragma argsused
static int view(unsigned id, char *buf, void * const arg)
{	char far*p;
	ecTag_t tag;
#define cnt (*(unsigned*)arg)

	p = ctxtAddress(CTXT_TAG_EXEC, id);
	assert(p);
	++cnt;
	if((tag = *buf++) > EC_LAST_TAG)
		displayString(TEXT_EC_HDR_UNKNOWN, cnt, p, tag);
	else
		displayString(TEXT_EC_HDR, cnt, p, ecNames[tag]);

	chkHeap
	switch(tag) {
	case EC_TAG_BATCH:		/* Batch context */
		{	unsigned long pos, lnr;
			unsigned idFnam;
			char *fnam;

			if(3 != sscanf(buf, "%u|%lu %lu", &idFnam, &pos, &lnr)) {
				error_context_corrupted();
				return 1;
			}
			if((fnam = ecString(idFnam)) == 0)
				return 1;
			displayString(TEXT_EC_DISP_BATCH, pos, lnr, fnam);
		}
		break;
	case EC_TAG_FOR_FIRST:	/* FOR primary context */
		{	unsigned idPattern, idVarname, idCmd;
			char *varname, *cmd, *pattern, *p;

			if(3 != sscanf(buf, "%u %u|%u", &idVarname, &idCmd, &idPattern)) {
				error_context_corrupted();
				return 1;
			}
			if((cmd = ecString(idCmd)) == 0
			 || (varname = ecString(idVarname)) == 0)
				return 1;
			for(pattern = 0; idPattern > idCmd; --idPattern) {
				if((p = ecString(idPattern)) == 0) {
					myfree(pattern);
					return 1;
				}
				unregStr(p);
				chkPtr(p);
				if(!StrAppChr(pattern, ' ') || !StrCat(pattern, p)) {
				 	error_out_of_memory();
				 	myfree(pattern);
				 	myfree(p);
				 	return 1;
				}
				myfree(p);
			}
			chkRegStr(varname);
			chkRegStr(cmd);
			displayString(TEXT_EC_DISP_FOR1, varname, pattern + 1, cmd);
			myfree(pattern);
		}
		break;
	case EC_TAG_FOR_NEXT:	/* FOR secondary context */
		{	unsigned idPrefix, idVarname, idCmd;
			char *varname, *cmd, *prefix;

			if(3 != sscanf(buf, "%*u %u %u %u"
			 , &idVarname, &idCmd, &idPrefix)) {
				error_context_corrupted();
				return 1;
			}
			if((cmd = ecString(idCmd)) == 0
			 || (varname = ecString(idVarname)) == 0
			 || (prefix = ecString(idPrefix)) == 0)
				return 1;

			displayString(TEXT_EC_DISP_FOR2, varname, prefix, cmd);
			chkRegStr(varname);
			chkRegStr(cmd);
			chkRegStr(prefix);
		}
		break;
	case EC_TAG_COMMAND:	/* command */
		{	int mode;

			if(1 != sscanf(buf, "%u", &mode)
			 || 0 == (buf = strchr(buf, ' '))
			 || !*++buf) {
				error_context_corrupted();
				return 1;
			}

			displayString(TEXT_EC_DISP_CMD_1, mode);
			if(mode & EC_CMD_IGNORE_EXIT)
				displayString(TEXT_EC_DISP_CMD_IGNORE_EXIT);
			if(mode & EC_CMD_FORCE_INTERNAL)
				displayString(TEXT_EC_DISP_CMD_FORCE_INTERNAL);
			if(mode & EC_CMD_SILENT)
				displayString(TEXT_EC_DISP_CMD_SILENT);
			if(mode & EC_CMD_NO_TRACE)
				displayString(TEXT_EC_DISP_CMD_NO_TRACE);
			displayString(TEXT_EC_DISP_CMD_2, buf);
		}
		break;
	case EC_TAG_SET_STRING:
	case EC_TAG_INTERACTIVE:
		{	unsigned id;

			if(1 != sscanf(buf, "%u", &id)) {
				error_context_corrupted();
				return 1;
			}
			displayString(TEXT_EC_DISP_SET_STRING, id);
		}
		break;
	}

#undef cnt
	return 0;
}

#pragma argsused
int cmd_dispEC(char *param)
{	unsigned cnt;

	assert(ctxtSegm);

	cnt = 0;
	ecEnum(CTXT_TAG_EXEC, 0, view, &cnt);

	return 1;
}
