/* $Id$
 * print the command-line prompt
 *
 */

#include "../config.h"


#include "../include/command.h"
#include "../include/misc.h"

void printprompt(void)
{	char *pr;

	dbg_printmem();

	pr = getEnv(PROMPTVAR);        /* get PROMPT environment var. */

	displayPrompt(pr? pr: DEFAULT_PROMPT);
}
