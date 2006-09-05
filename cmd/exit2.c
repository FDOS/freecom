/* $Id$
 *  EXIT -- exits current instance of FreeCOM or leave all batch files
 *
 * set the exitflag to true
 * and _force_ exit
 */

#include "../config.h"

#include "../include/command.h"

int force_exit (char * rest) { (void)rest; canexit = exitflag = 1; return 0; }
