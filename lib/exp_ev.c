/* $Id$
	expand environment variables
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <dynstr.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"
#include "../include/misc.h"

	/* about how many bytes cmdline is increased if necessary */
#define BUF_DIF_SIZE 32

char *cmdline;
int cmdlen, cmdbuf;
FLAG8 error;

char *verbatim_token = 0;
int verbatim = 0;

static char *ifctArgRun(struct IFCT *fct, char ** const Xp)
{	char *q, *p;
	char *cmdline, *parens, c;
	int rightPar, leftPar, cntParens;

	assert(Xp);

	p = *Xp;

	switch(leftPar = *p) {
	case '(':	rightPar = ')'; parens = "()"; break;
	case '[':	rightPar = ']'; parens = "[]"; break;
	case '{':	rightPar = '}'; parens = "{}"; break;
	default: assert(*p == '(' || *p == '[' || *p == '{');
	}

	for(cntParens = 1, q = p; ;)
		if(*(q = skipQuoteChars(q + 1, parens)) == leftPar)
			++cntParens;
		else if(*q == rightPar) {
			if(--cntParens == 0)
				break;
		} else if(!*q) {
			error_close_quote(rightPar);
			return 0;
		}
#ifndef NDEBUG
		else assert(*q == leftPar || *q == rightPar);
#endif

	c = *q;
	*q = 0;
	cmdline = fct? (fct->func)(p + 1): 0;
	*q = c;
	*Xp = q + 1;
	return cmdline;
}

static void appStr(const char * const s)
{	int len;
	const char *p;

	if(!s)	return;

redo:
	if((len = strlen(p = s)) < 0 || len > INT_MAX / 2 || cmdbuf + len <= 0
	 || cmdbuf < 0) {
		error_long_internal_line();
		error = 2;
	} else {
		if(cmdbuf - cmdlen > 2 * len) {
			memcpy(&cmdline[cmdlen], s, len + 1);
			cmdlen += len;
		} else {
			if((p = realloc(cmdline, 1 + (cmdbuf += BUF_DIF_SIZE))) == 0) {
				error = 1;		/* out of mem */
				return;
			}
			cmdline = (char*)p;
			goto redo;			/* apply sanity checks above again */
		}
	}
}
static void appCh(int c)
{	char buf[2];

	buf[0] = c;
	buf[1] = 0;
	appStr(buf);
}

char *expEnvVars(char * const line)
{	char *h, c;
	char *p, *q;

	assert(!verbatim);
	assert(!verbatim_token);

	cmdline = 0;
	cmdbuf = cmdlen = 0;
	error = 0;

	for(p = line; !error && *p;) {
		if(*p == '%') {			/* Scanner metafunctions */
			/* Try to identify special FOR variables */
			h = p;
			while(*++h == '%');
			if(isalpha(*h)) {
				int rv;

				c = *++h;
				*h = 0;
				rv = is_ivar(p, &q);
				*h = c;
				switch(rv) {
				case 2:	q = 0;
					goto loop;		/* Force an out-of-mem error */
				case 0: 			/* Found */
					appStr(q);
					p = h;			/* Skip the FOR variable */
					goto loop;
				default: break;		/* Not found */
				}
			}

			switch(*++p) {
			case '%':
				++p;
			appPercent:
			case '\0': 		/* lone % at EOL */
				appCh('%');
				break;

			case '0':	/* script variables */
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(0 == (h = find_arg(*p - '0')))
					  /* Let the digit be copied in the cycle */
					goto appPercent;
				++p;
				appStr(h);
				break;

			case '*':	/* all script variables %1..%# (ignores shiftlevel!) */
				if((q = fct_argv(",")) != 0) {
					appStr(q);
					myfree(q);
				}
				break;

			case '?':
				{	char hbuf[sizeof(unsigned) * 8 + 2];

					sprintf(hbuf, "%u", F(errorlevel));
					appStr(hbuf);
				}
				break;

			case '@':		/* internal function */
				{	struct IFCT *fct;

					q = ++p;
					while(isalpha(*p)) ++p;
					c = *p;
					*p = 0;
					if((fct = is_ifct(q)) == 0)
						error_no_ifct(q - 1);
					*p = c;
					if(*p == '[' || *p == '(')
						q = ifctArgRun(fct, &p);
					else
						q = fct? (fct->func)((char*)0): 0;
					if(q) {
						appStr(q);
						myfree(q);
					}
					if(verbatim) {
						/* %@VERBATIM() fetched --> process */
						verbatim = 0;
						if(!verbatim_token || !*verbatim_token
						 || (q = strstr(p, verbatim_token)) == 0) {
						 	/* quote all the remaining line */
						 	appStr(p);
							StrFree(verbatim_token);
						 	goto endloop;
						} else {
							*q = 0;
							appStr(p);
							p = q + strlen(verbatim_token);
							StrFree(verbatim_token);
						}
					}
				}
				break;

			default:			/* some variable */
				if((h = strchr(p, '%')) != 0) {
					char *var;

					*h = '\0';
					if((var = getEnv(p)) != 0) {
						appStr(var);
						myfree(var);
					}
					p = h + 1;
				}
				break;
			}
		} else
			appCh(*p++);

loop:
	}
endloop:

	switch(error) {
	case 1:
		error_out_of_memory();
	case 2:
		myfree(cmdline);
		return 0;
	}

	return StrTrim(cmdline);
}
