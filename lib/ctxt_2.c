/* $id$

	Temporary flags used during the 

	ctxtMain

	currently there is just one context
		-> so-called dynamic context
		-> all data is located in there it is the location of the subMCB

*/

#include "../config.h"

#include "../include/command.h"
#include "../include/context.h"

ctxt_flags_t ctxtInitialFlags = {
	 TRUE				/* display prompt */
	,TRUE				/* echo command line */
	,ERROR				/* default to swap */
};
