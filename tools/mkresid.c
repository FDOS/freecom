/*  $Id$

	Create the source code file that creates the resource IDs
	for assembly.

	Useage: MKRESID <RESOURCE.H >file.C

  	$Log$
  	Revision 1.1.2.1  2000/12/17 21:57:37  skaus
  	intermediate update 1

*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

static char id_prefix[] = "RES_ID_";
static char buf[1024];

main(void)
{
	char *p, *q;

	/* Prolog of source */
	puts("#include <stdio.h>\n"
		"#include \"resource.h\"\n"
		"main(void) {");

	while(fgets(buf, sizeof(buf), stdin)) {
		p = buf - 1;
		while(isspace(*++p));
		if(memcmp(p, id_prefix, sizeof(id_prefix) - 1) == 0) {
			q = p + sizeof(id_prefix) - 2;
			while(*++q && isalnum(*q));
			*q = 0;
			printf("\tprintf(\"%s EQU %%d\\n\", %s);\n", p, p);
		}
	}

	/* Epilog */
	puts("\n\treturn 0;\n}");

	return 0;
}
