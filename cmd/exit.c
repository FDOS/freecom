/*
 *  EXIT -- exits current instance of FreeCOM or leave all batch files
 *
 * set the exitflag to true
 */

#include "../config.h"

#include "../include/command.h"

#pragma argsused
int internal_exit(char *rest)
{
  exitflag = 1;

  return 0;
}
