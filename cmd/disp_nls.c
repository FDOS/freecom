/* $id$

	@ internal command to diplsay the NLS information currently
	in effect.

*/

#include "../config.h"

#include "../include/command.h"
#include "../include/nls.h"
#include "../strings.h"

#pragma argsused
int cmd_dispNLS(char *param)
{
#ifdef FEATURE_NLS
	refreshNLS();
	displayString(TEXT_NLS_DISPLAY
	 , nlsBuf->country, nlsBuf->charset
	 , nlsBuf->datefmt, nlsBuf->timefmt
	 , nlsBuf->thousendsSep, nlsBuf->decimalSep
	 , nlsBuf->dateSep, nlsBuf->timeSep, nlsBuf->listSep
	 , nlsBuf->curSymbol, nlsBuf->precision, nlsBuf->curFormat
	 , nlsBuf->inclFirst, nlsBuf->inclLast
	 , nlsBuf->exclFirst, nlsBuf->exclLast
	 , nlsBuf->illegalChars);
#else
	displayString(TEXT_NLS_EXCLUDED);
#endif
	return 1;
}
