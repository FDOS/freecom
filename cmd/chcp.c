/* $Id$
 *  CHCP.C - CHCP command.

 	CHCP [codepage]

 	via DOS-66-01/02

	$Log$
	Revision 1.1  2002/11/12 21:47:16  skaus
	add: CHCP (disabled by default)

 */

#include "../config.h"

#include <assert.h>
#include <dos.h>
#include <ctype.h>

#include <portable.h>

#include "../err_fcts.h"
#include "../include/command.h"
#include "../include/cmdline.h"
#include "../strings.h"

int cmd_chcp(char *param)
{	unsigned curCP, sysCP;

	_AX = 0x6601;
	geninterrupt(0x21);
	curCP = _BX;
	sysCP = _DX;
	if(_CFLAG) {
		error_get_codepage();
		return 1;
	}

	if(!param || !*param) {
		/* display current codepage */
		displayString(TEXT_DISPLAY_CODEPAGE, curCP, sysCP);
	} else {
		char *p;
		unsigned n = 0;

		/* set code page */
		if(!isdigit(*param)) {
			error_syntax(param);
			return 1;
		}

		p = param;
		do  n = n * 10 + (*p - '0');
		while (isdigit(*++p));

		if(*ltrimcl(p)) {
			error_syntax(param);
			return 1;
		}

		_DX = sysCP;
		_BX = curCP;
		_AX = 0x6602;
		geninterrupt(0x21);
		if(_CFLAG) {
			error_set_codepage();
			return 1;
		}
	}
	return 0;
}
