/*
 *  PROMPT.C - prompt handling.
 *
 */

#include "../config.h"


#include "../include/command.h"
#include "../include/context.h"
#include "../include/misc.h"

int cmd_prompt(char *param)
{
  if(param && *param == '=')    /* skip '=' & spaces */
    param = ltrim(param + 1);

  return chgEnvRemove(PROMPTVAR, param);
}
