/* $Id$

	Breaks up var=value like strings and displays the set if
	no argument

	Using the environment subsystem of SUPPL

*/

#include "../config.h"

#include <assert.h>
#include <string.h>

#include <environ.h>

#include "../err_fcts.h"
#include "../include/context.h"
#include "../include/misc.h"

#pragma argsused
static int view(void *arg, word segm, word ofs)
{
  if(peekb(segm, ofs) > ' ')		/* control characters tag hidden
  										features */
  	fprintf(outStream, "%Fs\n", MK_FP(segm, ofs));
  return 0;                     /* don't stop */
}


int breakVarAssign(ctxt_t context, char * const s, char ** const value)
{
	char *p;

	assert(value);
	assert(s);

	/* if no parameters, show the environment */
	if(!*s) {
		env_forAll(context, view, 0);
		return 0;
	}

	/* make sure there is an = in the command */
	if((p = strchr(s, '=')) == 0 || p == s)
		return 1;

	*p = 0;			/* separate name and value */
	*value = p + 1;
	return 2;
}
