/*
 * NLS.C -- Support for DOS NLS information
 *
 *  By default COMMAND assumes that the NLS information do change.
 *  refreshNLS() will make sure that nlsBuf is updated and on
 *  occasion when the information could have been changed, nlsBuf
 *  is invalidated in order to be re-read by refreshNLS().
 *
 * 1999/04/27 ska
 *  started
 * add: nlsBuf, refreshNLS
 */

#include "config.h"

#ifdef FEATURE_NLS

#include <assert.h>

#include "nls.h"

Country *nlsBuf = NULL;

void refreshNLS(void)
{ if(!nlsBuf)
    nlsBuf = nlsNewInfo();
}

#endif
