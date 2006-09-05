/* $Id$
 *  DSTACK.C - Directory stack PUSHD/POPD support, compatible with 4/NDOS
 *
 * Outputs directory stack contents
 */

#include "../config.h"

#include "../include/context.h"
#include "../include/command.h"
#include "../strings.h"

int cmd_dirs (char * param) {
    (void)param;
    return ctxtView (CTXT_TAG_DIRSTACK, TEXT_DIRSTACK_EMPTY);
}
