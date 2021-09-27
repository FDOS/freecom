/*	$Id$

	Read a line from stdin

*/


#include "../config.h"

#include "../include/command.h"

#ifdef FEATURE_ENHANCED_INPUT
static int use_enh = 1;
int set_readcommandType(int enhanced)
{
	int rv = use_enh;
	use_enh = ( enhanced != 0 );
	return rv;
}
#else
int set_readcommandType(int enhanced)
{
	(void)enhanced;
	return 0;
}
#endif

void readcommand(char * const str, int maxlen)
{
#ifdef FEATURE_ENHANCED_INPUT
	/* If redirected from file or so, should use normal one */
	if (use_enh)
		readcommandEnhanced(str, maxlen);
	else
		readcommandDOS(str, maxlen);
#else
	readcommandDOS(str, maxlen);
#endif
}

