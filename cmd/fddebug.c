/*
 *  FDDEBUG.C - verify command.
 *
 *  Comments: Turn on/off the debug flag
 *		& set the logfile
 */

#include "../config.h"

#include <assert.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dynstr.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../strings.h"

extern char *dbg_logname2;

int cmd_fddebug(char *param)
{
	switch(onoffStr(param)) {
  	default: {
  		FILE *f;
  		char *p;
  		int duplicate, channel;

		p = trimcl(param);

		if(*p == '!') {
			myfree(dbg_trace);
			dbg_trace = p[1]? estrdup(p + 1): 0;
			break;
		}
		if((duplicate = *p == '+') != 0)
			++p;
  		if((p = estrdup(p)) == 0)
  			return 1;

  		if(stricmp(p, "stderr") == 0) {
  			f = stderr;
  			channel = 1;
  		} else if(stricmp(p, "stdout") == 0) {
  			f = stdout;
  			channel = 1;
  		} else if((f = fopen(p, "at")) == 0) {
  			error_open_file(p);
  			return 2;
  		} else
			channel = 2;
		if(!duplicate || channel == 2) {
			fclose(dbg_logfile2);
			dbg_logfile2 = 0;
			chkPtr(dbg_logname2);
			StrFree(dbg_logname2);
		}
		if(!duplicate || channel == 1)
			dbg_logfile = 0;
		if(channel == 2) {
			dbg_logfile2 = f;
			dbg_logname2 = p;
		} else {
			dbg_logfile = f;
			myfree(p);
		}
		/* FALL THROUGH */
	}
  	case OO_On:		fddebug = 1;	break;
	case OO_Null:	case OO_Empty:
		displayString(TEXT_MSG_FDDEBUG_STATE, fddebug ? D_ON : D_OFF);
		if(dbg_logfile)
			displayString(TEXT_MSG_FDDEBUG_TARGET
			 , dbg_logfile == stderr? "stderr": "stdout");
		if(dbg_logfile2) {
			assert(dbg_logname2);
			displayString(TEXT_MSG_FDDEBUG_TARGET, dbg_logname2);
		}
		if(dbg_trace)
			displayString(TEXT_MSG_FDDEBUG_TRACING, dbg_trace);
		break;
  	case OO_Off:	fddebug = 0;	break;
	}
  return 0;
}
