/* $Id$

	function: tempfile(extension = "", directory = %TEMP%)

	However, %TEMP% expands to the current temporary directory,
		if TEMP itself is not set.

	Return:
		0: on error (error message already displayed)
		else: pointer to dynamically allocated tempfilename
*/

#include "../config.h"

#include "../include/command.h"
#include "../include/cmdline.h"
#include "../include/misc.h"

char *fct_tempfile(char *param)
{	char *argv[2];

	if(splitFctLine(2, argv, param))
		return 0;

	if(argv[0] && !*argv[0])
		argv[0] = 0;
	return (argv[1] && !*argv[1])
		? mktempfile(argv[1], argv[0])
		: tmpefn(argv[0]);
}
