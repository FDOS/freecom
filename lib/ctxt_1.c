/* $id$

	Central pointer to dynamic context

	ctxtMain

	currently there is just one context
		-> so-called dynamic context
		-> all data is located in there it is the location of the subMCB

*/

#include "../config.h"

#include "../include/context.h"

volatile ctxt_t ctxtMain = 0;
ctxt_flags_t far *ctxtFlagsP = &ctxtInitialFlags;
