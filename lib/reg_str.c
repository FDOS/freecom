/* $Id$

	Register a dynamically allocated string.
	Those strings are temporary strings with a short time to live;
	only a limited number of them are cached, the eldest is deallocated
	(FIFO).
*/

#include "../config.h"

#include <stdlib.h>
#include <string.h>

#include "../include/misc.h"

#ifdef DEBUG
#define ITEMS (STRING_CACHE_ITEMS + 0)
//#define ITEMS (STRING_CACHE_ITEMS + 20)

#else
#define ITEMS STRING_CACHE_ITEMS

#endif

#if STRING_CACHE_ITEMS < 5
#error "A string cache of less than five items is too dangerous!"
#endif
static char *strCache[ITEMS];
static int top = ITEMS;

#ifdef DEBUG
static char *dupe[ITEMS];
static void probeStrCache(void)
{	int i;

	i = ITEMS;
	while(i--)
		assert(!(strCache[i]) || heapchecknode((strCache[i])) == _USEDENTRY);
}

#else
#define probeStrCache()

#endif


char *regStr(const char * const s)
{
	probeStrCache();
	if(s) {
#ifdef DEBUG
		char *q;
#endif

		if(!top--)
			top = ITEMS - 1;

#ifdef DEBUG
			/* myfree() is checking the string cache */

		assert(isRegStr(s) < 0);

		q = strCache[top];
		strCache[top] = 0;
#else
#define q strCache[top]
#endif
		myfree(q);
#undef q
		strCache[top] = (char*)s;
#ifdef DEBUG
		myfree(dupe[top]);
		dupe[top] = estrdup(s);
#endif
	}

	return (char*)s;
}

int isRegStr(const char * const s)
{	int i;

	i = ITEMS;
	while(i--) if(strCache[i] == s)
		break;
	return i;
}

char *unregStr(const char * const s)
{	int i;

	probeStrCache();
	if(s) {
		if((i = isRegStr(s)) >= 0) {
			assert(top >= 0 && top < ITEMS);
			strCache[i] = 0;

			assert(isRegStr(s) < 0);

			if(strCache[top] == 0) {
				i = top;	/* prevent endless loop if cache is empty */
				do {
					if(++top == ITEMS)
						top = 0;
				} while(strCache[top] == 0 && top != i);
			}
#ifdef DEBUG
			myfree(dupe[i]);
			dupe[i] = 0;
#endif
			return (char*)s;
		}
		dprintf(("[MEM: Unregistering of string failed]\n"));
	}
	return (char*)s;
}
