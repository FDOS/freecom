/*
 * CD / CHDIR - changes the current working directory of a drive
 */

#include "../config.h"

#include "../include/command.h"

int cmd_chdir(char *param)
{
  return cd_dir(param, 0, "CHDIR");
}
