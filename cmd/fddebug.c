/* $Id$
 *  FDDEBUG.C - verify command.
 *
 *  Comments: Turn on/off the debug flag
 *
 * 30-Mar-2000 (John P Price <linux-guru@gcfl.net>)
 *   started.
 *
 * 2001/02/16 ska
 * add: redirect output into file
 */

#include "../config.h"

#include <assert.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../strings.h"

extern FILE *dbg_logfile;
extern char *dbg_logname;

int cmd_fddebug(char *param)
{
	switch(onoffStr(param)) {
  	default: {
  		FILE *f;
  		char *p;

  		if((p = strdup(trimcl(param))) == 0) {
  			error_out_of_memory();
  			return 1;
  		}

  		if(stricmp(param, "stderr") == 0) f = stderr;
  		else if(stricmp(param, "stdout") == 0) f = stdout;
  		else if((f = fopen(param, "at")) == 0) {
  			error_open_file(param);
  			return 2;
  		}
		if(dbg_logfile != stderr && dbg_logfile != stdout)
			fclose(dbg_logfile);
		dbg_logfile = f;
		free(dbg_logname);
		dbg_logname = p;
		/* FALL THROUGH */
	}
  	case OO_On:		fddebug = 1;	break;
	case OO_Null:	case OO_Empty:
		displayString(TEXT_MSG_FDDEBUG_STATE, fddebug ? D_ON : D_OFF);
		displayString(TEXT_MSG_FDDEBUG_TARGET
		 , dbg_logname? dbg_logname: "stdout");
		break;
  	case OO_Off:	fddebug = 0;	break;
	}
  return 0;
}
