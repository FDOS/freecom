/* $id $

	Functions handling the various execution contexts

*/

#include "../config.h"

#include "../include/context.h"

char* (*ecFunction[])(ctxtEC_t far * const) = {
/* EC_TAG_INTERACTIVE */ 
#ifdef FEATURE_ENHANCED_INPUT
	/* If redirected from file or so, should use normal one */
	readcommandEnhanced
#else
	readcommandDOS
#endif
/* EC_TAG_BATCH */ 
	, readbatchline
/* EC_TAG_FOR_FIRST */ 
	, readFORfirst
/* EC_TAG_FOR_NEXT */ 
	, readFORnext
/* EC_TAG_COMMAND */ 
	, readCommand
/* EC_TAG_COMMAND_NO_IGNORE */ 
	, readCommandNoIgnore
/* EC_TAG_KEEP_RUNNING */ 
	, keepMeRunning
/* EC_TAG_TERMINATE */ 
	, terminateShell
};
