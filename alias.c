/*
 *  ALIAS.C - alias administration module
 *
 *
 *
 *  Comments:
 *
 *  02/02/1996 (Oliver Mueller)
 *    started.
 *
 *  02/03/1996 (Oliver Mueller)
 *    Added sorting algorithm and case sensitive substitution by using
 *    partstrupr().
 *
 * 27 Jul 1998  John P. Price
 * - added config.h include
 * - added ifdef's to disable aliases
 *
 * 1998/10/27 ska
 * - changed: rmtmpfile() closes file itself
 *
 * 01-Dec-1998  John P. Price
 * - changed to convert to lower case instead of upper
 *
 * 1998/12/05 ska
 * - bugfix: if useFlag warps its type range, all the use flags are
 *  clear, but in an infinite loop
 *
 * 16-Dec-1998  John P. Price
 * - Changed expander so that if the first non-whitespace character is a
 *   dot then not to do alias expansion.  This was "noalias" but I prefer
 *   the shorter version.
 *
 * 1999/01/24 ska
 * bugfix: aliasdel(): after the pointer is free()'ed, the loop accessed
 *      the memory again (reported by Eric Kohl <ekohl@abo.rhein-zeitung.de>)
 *
 * 28-Jan-1999  John P. Price
 * - Changed noalias character from a dot to an asterisk (*).
 *
 * 1/Feb/1999 Rob Linwood
 * - added code to allow alias commands to contain extra whitespace
 *
 * 24-Mar-1999  John P. Price
 * - fixed bug in aliasexpand
 *
 * 1999/04/23 ska
 * bugfix: aliasexpand() the main loop 'while(d)' always fails on second
 *	try, because 'ptr' is still NULL.
 * chg: aliasexpand(): renamed variable 'd' --> 'expanded'.
 * bugfix: the ltrim() function returns strings matching /^\s*$/ as NULL,
 *	however, all the other code expects that the string is != NULL.
 * chg: Move ltrim() & rtrim() into misc.c and use them everywhere else
 *
 * 1999/07/08 ska
 * chg: removed variable 'd' from cmd_alias(), it's value is never really
 *	used
 */

#include "config.h"

#ifdef FEATURE_ALIASES

/*
#ifdef DEBUG
#define DEBUG_ALIASES
#endif
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#include "command.h"
#include "cmdline.h"
#include "tempfile.h"
#include "alias.h"
#include "misc.h"

typedef struct TAlias
{
  char *name;
  char *subst;
  unsigned short used;
  struct TAlias *next;
}
TAlias;

static unsigned short useFlag = 0;
static TAlias *first = 0,
 *last = 0;

/* module internal functions */
void partstrlower(char *str)    /* strlwr only for first word in string */
{	char c;
  assert(str);
  while ((c = *str) != '\0' && !isspace(c))
  {
    *str++ = tolower(c);
  }
}

void aliasprint(void)
{
  TAlias *ptr = first;
  while (ptr)
  {
  	assert(ptr->name);
  	assert(ptr->subst);
    printf("%s=%s\n", ptr->name, ptr->subst);
    ptr = ptr->next;
  }
}

void aliasdel(char *name)
{
  TAlias *ptr = first,
   *prev = 0;

  while (ptr)
  {
  	assert(ptr->name);
  	assert(ptr->subst);
    if (!strcmp(ptr->name, name))
    {
      if (prev)
        prev->next = ptr->next;
      else
        first = ptr->next;
      free(ptr->name);
      free(ptr->subst);
      free(ptr);
      return;
    }
    prev = ptr;
    ptr = ptr->next;
  }
}

int aliasadd(char *name, char *subst)
{
  TAlias *ptr = first,
   *prev,
   *entry;
  char *s;

	assert(subst);
	assert(name);

	/* When the alias is already located within the list, we need not
		search for the correct location */
  while (ptr)
  {
  	assert(ptr->name);
  	assert(ptr->subst);
    if (!strcmp(ptr->name, name))
    {
      if((s = strdup(subst)) == NULL)
        return -1;
      free(ptr->subst);
      ptr->subst = s;
      return 0;
    }
    ptr = ptr->next;
  }

	/* newly create the alias */
  if((ptr = (TAlias *) malloc(sizeof(TAlias))) == NULL)
    return -1;
  ptr->next = NULL;

  if((ptr->name = strdup(name)) == NULL) {
    free(ptr);
    return -1;
  }

  if((ptr->subst = strdup(subst)) == NULL)
  {
    free(ptr->name);
    free(ptr);
    return -1;
  }
  partstrlower(ptr->subst);     /* it's necessary for recursive substitution */

  ptr->used = 0;

 /* Alias table must be sorted!
  * Here a little example:
  *   command line = "ls -c"
  * If the entries are
  *   ls=dir
  *   ls -c=ls /w
  * command line will be expanded to "dir -c" which is not correct.
  * If the entries are sortet as
  *   ls -c=ls /w
  *   ls=dir
  * it will be expanded to "dir /w" which is a valid DOS command.
  */
  entry = first;
  prev = 0;
  while (entry)
  {
    if (strcmp(ptr->name, entry->name) > 0)
    {
      if (prev)
      {
        prev->next = ptr;
        ptr->next = entry;
      }
      else
      {
        ptr->next = entry;
        first = ptr;
      }
      return 0;
    }
    prev = entry;
    entry = entry->next;
  }

  /* The new entry is the smallest (or the first) and must be
   * added to the end of the list.
   */
  if (!first)
    first = ptr;
  else
    last->next = ptr;
  last = ptr;

  return 0;
}

void aliasexpand(char *cmd, int maxlen)
{
  char *cp;
  unsigned m;
  unsigned len;
  short expanded;
  TAlias *ptr;

	assert(cmd);

  if (++useFlag == 0)           /* (int) overflow */
  {                             /* The useFlag specifies, if the particular
                                   ALIAS is used by the _current_ expand.
                                   To avoid to clear the flag each time when
                                   to expand a string a different flag value
                                   is used each time. */
    if((ptr = first) != NULL)
                        /* reset all values to be sure we hit no old one */
      do ptr->used = 0;
      while((ptr = ptr->next) != NULL);
    useFlag = 1;
  }

	cp = ltrim(cmd);		/* skip leading whitespaces */

  /* Check if the user disabled alias expansion */
  if (*cp == '*')
  {
  	cp = ltrim(cp + 1);
    memmove(cmd, cp, strlen(cp) + 1);
    return;
  }

  /* this allows to case-sensitively compare strings, which
   is much faster */
  partstrlower(cp);
  /* to simplify the loop below */
  memmove(cmd, cp, strlen(cp) + 1);

  /* substitution loop */
  /* Empty alias list --> no loop */
  if(first) do {
    expanded = 0;
    ptr = first;
    do {
    	assert(ptr);
    	assert(ptr->name);
    	assert(ptr->subst);
      len = strlen(ptr->name);
      if ((isspace(cmd[len]) || cmd[len] == '\0')	/* end of word */
       && !strncmp(cmd, ptr->name, len)		/* line begins with alias */
       && ptr->used != useFlag)					/* this alias unused */
      {
        m = strlen(ptr->subst);
        if (strlen(cmd) - len + m > maxlen)
        {
          error_command_too_long();
          cmd[0] = '\0';        /* the parser won't cause any problems
          							with an empty line */
          return;
        }
        else
        {
          	/* adjust the remaining part within the command line */
          memmove(&cmd[m], &cmd[len], strlen(&cmd[len]) + 1);
          	/* prepend the alias substitution */
          memcpy(&cmd[0], &ptr->subst[0], m);
          ptr->used = useFlag;
          expanded = 1;
        }
      }
    } while((ptr = ptr->next) != NULL);
  } while(expanded);
}

#pragma argsused
int cmd_alias(char *rest)
{
  char *ptr;

  if (!rest || !*rest)
  {
    aliasprint();
    return 0;
  }

  if ((ptr = strchr(rest, '=')) == 0)
  {
    errno = EINVAL;
    return -1;
  }

  /* Split rest into name and substitute */
  *ptr++ = '\0';

  partstrlower(rest);

  if (ptr[0])
    return aliasadd(rest, trim(ptr));

  aliasdel(rest);
  return 0;
}

#ifdef FEATURE_KERNEL_SWAP_SHELL
int alias_streamsize(void)
{	int size = 1;			/* end-of-array marker */
  TAlias *ptr = first;
  while (ptr)
  {
  	assert(ptr->name);
  	assert(ptr->subst);
    if((size += strlen(ptr->name) + strlen(ptr->subst) + 2) <= 0)
    	return 0;		/* error */
    ptr = ptr->next;
  }
  return size;
}
void alias_streamto(char far *p)
{ TAlias *ptr = first;
#ifdef DEBUG
	unsigned cnt = 0;
#endif

  while (ptr)
  {
  	assert(ptr->name);
  	assert(ptr->subst);
    p = _fstpcpy(p, ptr->name) + 1;
    p = _fstpcpy(p, ptr->subst) + 1;
    ptr = ptr->next;
#ifdef DEBUG
    ++cnt;
#endif
  }
  *p = 0;
  dprintf(("[KSWAP: %u alias(es) dumped to dynamic context]\n", cnt));
}
void alias_streamfrom(char far *p)
{	TAlias *ptr;
	size_t len;
#ifdef DEBUG
	unsigned cnt = 0;
#endif

#ifdef DEBUG_ALIASES
	dbg_outsn("[Alias dump]");
	for(len = 0; len < 4; ++len) {
		dbg_print("%04x ", len * 8);
		for(cnt = 0; cnt < 8; ++cnt)
			dbg_print("%02X ", (unsigned char)p[len * 8 + cnt]);
		dbg_outs("     ");
		for(cnt = 0; cnt < 8; ++cnt) {
			unsigned char ch = (unsigned char)p[len * 8 + cnt];
			if(isprint(ch))
				dbg_outc(ch);
			else dbg_outc('.');
		}
		dbg_outc('\n');
	}
	cnt = 0;
#endif

	last = 0;
	while(*p) {
		/* newly create the alias */
		if((ptr = (TAlias *) malloc(sizeof(TAlias))) == NULL)
			break;

		len = _fstrlen(p) + 1;
		if((ptr->name = malloc(len)) == NULL) {
			free(ptr);
			break;
		}
		_fmemcpy(ptr->name, p, len);
		p += len;

		len = _fstrlen(p) + 1;
		if((ptr->subst = malloc(len)) == NULL) {
			free(ptr->name);
			free(ptr);
			break;
		}
		_fmemcpy(ptr->subst, p, len);
		p += len;

#ifdef DEBUG
		++cnt;
#endif

		if(last)	last = last->next = ptr;
		else		last = first = ptr;
	}
	if(last)		last->next = NULL;
	dprintf(("[KSWAP: %u alias(es) read from dynamic context]\n", cnt));
}
#endif

#endif
