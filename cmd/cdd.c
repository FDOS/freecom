/*
 * CDD - changes drive and directory
 */

#include "../config.h"

#include "../include/command.h"

int cmd_cdd(char *param)
{
	return cd_dir(param, 1, "CDD");
}
