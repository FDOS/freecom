/* $id$

	Internal error codes
*/

#ifndef __IERROR_H
#define __IERROR_H

enum InternalErrorCodes {
	E_None = 0,
	E_Useage = 1,
	E_Other = 2,
	E_CBreak = 3,
	E_NoMem,
	E_CorruptMemory,
	E_NoOption,
	E_Exit,
	E_Ignore,			/* Error that can be ignored */
	E_Empty,
	E_Syntax,
	E_Range,				/* Numbers out of range */
	E_NoItems,
	E_Operation
};

#endif
