/* $id $

	Functions handling the various execution contexts

*/

#include "../config.h"

#include "../include/context.h"

char* (*ecFunction[])(ctxtEC_t far * const) = {
/* EC_TAG_INTERACTIVE */ 
	readInteractive
/* EC_TAG_BATCH */ 
	, readbatchline
/* EC_TAG_FOR_FIRST */ 
	, readFORfirst
/* EC_TAG_FOR_NEXT */ 
	, readFORnext
/* EC_TAG_COMMAND */ 
	, readCommand
/* EC_TAG_KEEP_RUNNING */ 
	, keepMeRunning
/* EC_TAG_TERMINATE */ 
	, terminateShell
};

#if sizeof(ecFunction) / sizeof(char* (*)()) != (EC_TAG_TERMINATE + 1)
#error "The ecFunction[] array has not EC_TAG_TERMINATE entries"
#endif
