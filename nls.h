/*
	Declarations to access the DOS NLS information
*/

#ifndef __NLS_H
#define __NLS_H

#include <cntry.h>

extern Country *nlsBuf;		/* internally cached NLS info buffer */

void refreshNLS(void);		/* make sure the nlsBuf is valid */

#endif
