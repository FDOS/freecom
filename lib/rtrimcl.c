/*	$id$
	$Locker$	$Name$	$State$

 * Name: rtrimcl() - right trims a string by removing trailing spaces
 	A '\0' is placed to terminate the string.
 * Input: str - a pointer to a string
 * Output: str will have all spaces removed from the right.

	$Log$
	Revision 1.1  2001/04/29 11:33:51  skaus
	chg: default heap size (tools\ptchsize) set to 6KB
	chg: error displaying functions centralized into lib\err_fcts.src
	add: displayError()
	chg: all errors are displayed through functions void error_*()
	bugfix: somtimes error messages are not displayed (see displayError())
	bugfix: docommand(): type:file must pass ":file" to TYPE
	bugfix: error_sfile(): string _SFILE_
	bugfix: error message on empty redirection
	bugfix: comma and semicolon ';' are recognized as argument seperators
		of internal commands

 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "../include/cmdline.h"

void rtrimcl(char * const str)
{ char *p;

  assert(str);

  p = strchr(str, '\0');
  while (--p >= str && isargdelim(*p))
    ;
  p[1] = '\0';
}
