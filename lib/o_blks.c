/* $id$

	Output functions

	Output a string according the specified modes

*/

#include "../config.h"

#include <dynstr.h>

#include "out.h"

void oBlockString(unsigned mode, int size, const char * const line)
{	int len, olen, filler;

	len = Strlen(line);
	if(mode & OUT_MODE_BRACKETS)
		len += 2;

	switch(mode & OUT_MODE_BLOCK_MASK) {
	case OUT_MODE_BLOCK_CHUNK:
		if(size) {
			div_t d;
			d = div(len, size);
			olen = d.quot;
			if(d.rem)
				++olen;
			olen *= size;
			break;
		}
		/**FALL THROUGH**/
	case OUT_MODE_BLOCK_ENLARGE:
		olen = len > size? len: size;
		break;
	case OUT_MODE_BLOCK_CUT:
		olen = len < size? len: size;
		break;
#ifdef DEBUG
		default:
			dprintf(("oBlockString() unsupported block mode: %u"
			 , mode & OUT_MODE_BLOCK_MASK));
#endif
	}

	if(!olen)		/* nothing to do at all */
		return;

	if((mode | OUT_MODE_NO_WRAP) != 0)
		oNeedSpace(olen);

	filler = 0;
	if(olen > len) {
		/* Apply justification */
		switch(mode & OUT_MODE_JUSTIFY_MASK) {
		case OUT_MODE_JUSTIFY_LEFT:
			filler = olen - len;
			break;
		case OUT_MODE_JUSTIFY_RIGHT:
			oPadChars(mode, olen - len);
			break;
		case OUT_MODE_JUSTIFY_CENTER:
			filler = (olen - len) / 2;
				/* odd fillers get one additional char left */
			oPadChars(mode, olen - len - filler);
			break;
#ifdef DEBUG
		default:
			dprintf(("oBlockString() unsupported justify mode: %u"
			 , mode & OUT_MODE_JUSTIFY_MASK));
#endif
		}
	}

	if(mode & OUT_MODE_BRACKETS) {
		oCh('[');
		if(!--olen)
			return;
		if(!--olen) {		/* only space for closing bracket */
			oCh(']');
			return;
		}
		len -= 2;
	}

	oMem(line, len < olen? len: olen);

	if(mode & OUT_MODE_BRACKETS)
		oCh(']');
	oPadChars(mode, filler);
}
