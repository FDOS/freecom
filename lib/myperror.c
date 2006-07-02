/* Smaller PERROR replacement for FreeCOM (only used in REN and DEL) */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../include/misc.h"
#include "../strings.h"

void myperror( const char *msg )
{
    displayError( errno == EACCES ? TEXT_ERROR_PERMISSION_DENIED :
                  errno == ENOENT ? TEXT_ERROR_NO_SUCH_FILE :
                  TEXT_ERROR_UNKNOWN_ERROR, msg );
}
