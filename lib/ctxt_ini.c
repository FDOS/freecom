/*
 	Initial values of the context's subMCB and the exec context
 */

#include "../config.h"

#include "../include/context.h"

ctxtCB_t ctxtInitialCB = {
	'Z', 0, 0, 13, ""
};
ctxtEC_t ctxtInitialEC = {
	EC_TAG_KEEP_RUNNING
	, 0
};
