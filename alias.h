/*
 *  ALIAS.H - header file for alias module
 *  Oliver Mueller
 *  02/02/1996
 *
 */

#ifndef __ALIAS_H_OGM
#define __ALIAS_H_OGM

void aliasexpand(char *cmd, int maxlen);
int aliasswapout(void);
int aliasswapin(void);

#endif
