/*  $Id$

	Extracts the MOD_VERSION from module asm files.

	Useage: EXTRVERS pattern

	It creates corresponing files named <filename>.VER

  	$Log$
  	Revision 1.1.2.1  2000/12/17 21:57:37  skaus
  	intermediate update 1

*/

#include <ctype.h>
#include <dir.h>
#include <stdio.h>
#include <string.h>

static char buf[1024];
static char fnam[1024];
static const char versToken[] = "MODULE_VERSION";
static const char versExt[] = ".VER";

main(int argc, char **argv)
{	int i;
	struct ffblk ff;
	char *p, *q, *h;
	FILE *f, *o;

	for(i = 1; i <= argc; ++i)
		if(findfirst(argv[i], &ff, 0) == 0) {
			p = stpcpy(fnam, argv[i]);
			while(--p >= fnam && *p != ';' && *p != '/' && *p != '\\');
			++p;
			do {
				h = stpcpy(p, ff.ff_name);
				if((f = fopen(fnam, "rt")) == NULL)
					fprintf(stderr, "Failed to open: %s\n", fnam);
				else {
					while(fgets(buf, sizeof(buf), f)) {
						q = buf - 1;
						while(isspace(*++q));
						if(memcmp(q, versToken, sizeof(versToken) - 1) == 0) {
							/* version found */
							while(--h >= p && *h != '.');
							if(h < p) strcat(p, versExt);
							else strcpy(h, versExt);
							if((o = fopen(fnam, "wt")) == NULL)
								fprintf(stderr, "Cannot create: %s\n", fnam);
							else {
								q += sizeof(versToken) - 1;
								while(isspace(*++q));
								if(memcmp(q, "EQU", 3) == 0) {
									q += 2;
									while(isspace(*++q));
								}
								fputs("#define MOD_VERSION ", o);
								while(*q && !isspace(*q))
									putc(*q++, o);
								putc('\n', o);
								fclose(o);
								printf("Created: %s\n", fnam);
							}
							break;
						}
					}
					fclose(f);
				}
			} while(findnext(&ff) == 0);
		}

	return 0;
}
