/* $id$

	Read command from file, if the stdin is redirected from a file.

*/

#include "../config.h"

#include <string.h>

#include <supplio.h>

#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"

char *readcommandFromFile(void)
{	char *line, *p;

	dprintf(("readcommandFromFile()\n"));
	/* flags like doExit etc. has been checked by caller */

	line = Fgetline(stdin);

	if(!line) {
		error_out_of_memory();
		return cmdlineIgnore;
	}
	if(!*line) {	/* EOF -> kill context */
		myfree(line);
		return 0;
	}

	/* Strip a possibily appended newline; strlen(line) >= 1! */
	if((p = strchr(line, '\0'))[-1] == '\n')
		p[-1] = 0;

	return line;
}
