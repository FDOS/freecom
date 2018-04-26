/* $Id$

	Return the name of the resource file

*/

#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <dfn.h>

#include "../include/command.h"
#include "../include/misc.h"

char *comResFile(void)
{  if(isSwapFile) {
       char *p;
       if((p = comFile()) != 0) {
           assert(strlen(p) == isSwapFile + 3);
           memcpy(p + isSwapFile, "SWP", 3);
           if(exist(p))
			   return p;
           free(p);
       }
   }

   return comFile();
}
