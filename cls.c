/*
 *  CLS.C - clear screen internal command
 *
 *  Comments:
 *
 *  07/27/1998 (John P. Price)
 *    started.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 30-Nov-1998 (John P Price <linux-guru@gcfl.net>)
 * - CLS now sets the text colors to lightgray on black before clearing the
 *   screen.
 *
 */

#include "config.h"

#ifdef INCLUDE_CMD_CLS

#include <conio.h>

#include "command.h"

#pragma argsused
int cmd_cls(char *rest)
{
  textcolor(LIGHTGRAY);
  textbackground(BLACK);
  clrscr();
  return 0;
}
#endif
