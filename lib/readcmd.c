/*	$Id$

	Read a line from stdin

*/


#include "../config.h"

#include "../include/command.h"

#if defined(DBCS) && !defined(VZTSR)
#define VZTSR 1
#endif

#if defined(FEATURE_ENHANCED_INPUT) && defined(VZTSR)
static char check_vz_tsr(void)
{
	static unsigned mcb_top;
	char rv = 0;
	unsigned mcb;

	if (mcb_top == 0) {
		IREGS r;
		r.r_ax = 0x5200;
		intrpt(0x21, &r);
		mcb_top = *((unsigned short FAR *)MK_FP(r.r_es, r.r_bx - 2));
	}
	mcb = mcb_top;
	while (1) {
		const char FAR *m = MK_FP(mcb, 0);
		char c = *m;
		unsigned owner, len;
		if (c != 'M' && c != 'Z') break; /* corrupt MCB */
		owner = *(unsigned short FAR *)&(m[1]);
		len = *(unsigned short FAR *)&(m[3]);
		if ( owner == (mcb + 1) && len > 0x12
		    && m[0x10 + 0x102] == 'V'
		    && m[0x10 + 0x103] == 'Z'
		    && m[0x10 + 0x109] == '\0'
		    && m[0x10 + 0x10a] == '\x1a' )
		{
			rv = m[0x10 + 0x122];
			break;
		}
		if (c == 'Z') break;
		mcb += len + 1;
	}
	return rv;
}
#endif

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
 #ifdef VZTSR
	if (use_enh && !check_vz_tsr())
 #else
	if (use_enh)
 #endif
		readcommandEnhanced(str, maxlen);
	else
		readcommandDOS(str, maxlen);
#else
	readcommandDOS(str, maxlen);
#endif
}

