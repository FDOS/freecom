/*
 * CMDLINE.C -- Command line parser and supplemental functions
 *
 * 1999/04/27 ska
 *  started
 * chg: moved trim(), ltrim(), rtrim(), match_(), split(), freep() here
 * chg: split() may recieve NULL as line --> treat as empty line
 *
 * 1999/07/06 ska
 * add: longname options
 *
 * 1999/11/02 ska
 * chg: re-enabled skipwd() as it used within FreeCom and resolved from
 *  SUPPL currently, which is not 100% compatible
 *
 * 2000/06/22 ska
 *	bugfix: unquote() calculated portion preceeding left quote
 *
 * 2000/07/05 Ron Cemer
 *	bugfix: renamed skipwd() -> skip_word() to prevent duplicate symbol
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#ifndef NDEBUG
#include <stdio.h>
#endif

#include "command.h"
#include "cmdline.h"


/*
 * Name: ltrim() - left trims a string by removing leading spaces
 * Input: str - a pointer to a string
 * Output: returns a trimmed copy of str
 */
char *ltrim(char *str)
{ char c;

  assert(str);

  while ((c = *str++) != '\0' && isspace(c))
    ;

  return str - 1;
}

/*
 * Name: rtrim() - right trims a string by removing trailing spaces
 * Input: str - a pointer to a string
 * Output: str will have all spaces removed from the right.
 */
void rtrim(char * const str)
{ char *p;

  assert(str);

  p = strchr(str, '\0');
  while (--p >= str && isspace(*p))
    ;
  p[1] = '\0';
}

/*
 *  Combines ltrim() & rtrim()
 */
char *trim(char *str)
{ assert(str);
  rtrim(str);
  return ltrim(str);
}

/*
 *  Skip a quoted word, stop at end of string, any whitespace or
 *  at a given string (case-sensitive compare).
 */
char *skipqword(const char *pp, const char * const stop)
{ size_t len;
  int quote = 0;

  len = stop? strlen(stop): 0;

    if(*pp) do {
    if(quote) {
      if(quote == *pp)
        quote = 0;
    } else if(strchr(QUOTE_STR, *pp))
          quote = *pp;
    else if(isspace(*pp) || len && memcmp(pp, stop, len) == 0)
      break;
  } while(*++pp);

  return (char *) pp;
}

/*
 *  Check if the passed line begins with a specified word
 *  The word is matched case-insensitively.
 *  'len' is the length of the word.
 *  On success, the pointer is placed onto the next word.
 *  Return: 0: on failure
 */
int match_(char ** const Xp, const char * const word, int len)
{ char *p;

  assert(Xp && *Xp);
  assert(word && *word);
  assert(len > 0);

  if(strncmpi(p = *Xp, word, len) == 0) {
    /* line begins with string, now test if it is a word */
    p += len;
    if(*p) {
      if(!isspace(*p))    /* no word boundary */
        return 0;
      /* skip whitespaces */
      p = ltrim(p);
    }
    *Xp = p;
    return 1;   /* found */
  }

  return 0;
}

/*
 * Find the next delimiter/non-delimiter within p
 *  Honor quotes and leading option characters
 */
static char *find(char *p, int delim)
{ int ch, quote;
  int isopt;

  assert(p);

#define isdelim(ch) (isopt? isoptdelim(ch): isargdelim(ch))

  if((isopt = isoption(p)) != 0 && delim) {
    /* Assume the following example:
        p == "arg/opt"
      first find("arg/opt", 1) == find next delimiter
      returns pointer to "/opt"
      Now the process would call skip delimiters to reach the
      next argument --> find("/opt", 0) == find next non-delimiter
      will return "/opt", because '/' is delimiter only when
      searching for delimiters.
      Now the process would call skip non-delimiters to reach the
      end of the arg --> find("/opt", 1) == find next delimiter
      This time the leading optch's are part of the argument.
      ===> This is performed in this branch.

      If the string would be "/opt1/opt2", the call of
      find("/opt1/opt2", 1) == find next delimiter must stop
      at the second '/'.
    */
    while((ch = *++p) != 0 && isoptch(ch));
  }

  quote = 0;
  while((ch = *p++) != '\0' && (quote
   || (delim && !(isdelim(ch) || isoptch(ch)))
   || (!delim && isdelim(ch))))
    if(quote == ch)
      quote = 0;
    else if(strchr(QUOTE_STR, ch))
      quote = ch;

  return p - 1;
#undef isdelim
}

/*
 * skip_word - skip a word / find next word delimiter
 *  word delimiters are whitespaces and non-leading option characters
 */
char *skip_word(char *p)
{ return find(p, 1);
}

/*
 * skipdm - skip all word delimiters / find next non-word delimiter
 *  word delimiters are whitespaces only
 */
char *skipdm(char *p)
{ return find(p, 0);
}

/*
 * removeQuotes - generates a new dynamically allocated string that
 *  contains no quotes (outer most quotes).
 *  The passed in string is preserved.
 *  Return: NULL on memory allocation problems
 */
char *unquote(const char *str, const char * const end)
{ char *h, *newStr;
  const char *q;
  size_t len;

  assert(str);
  assert(end);
  assert(end >= str);

  if((h = newStr = malloc(end - str + 1)) == NULL)
    return NULL;

  while((q = strpbrk(str, QUOTE_STR)) != NULL && q < end) {
  /* there is a quote at *q */
    /* copy the portion before q */
    memcpy(h, str, len = q++ - str);
    h += len;
    if((str = strchr(q, q[-1])) == NULL || str >= end) {
      /* no right quote */
      str = q;
      break;
    }
    /* copy the quoted portion */
    memcpy(h, q, len = str++ - q);
    h += len;
  }
  /* copy the remaining unquoted portion */
  memcpy(h, str, len = end - str);
  h[len] = '\0';
  /* to not waste one byte per deleted quote use this:
    h = realloc(newStr, strlen(newStr) + 1);
    assert(h);
    return h;
      -or- if you assume that to shrink can fail
    return (h = realloc(newStr, strlen(newStr) + 1)) != NULL
      ? h: newStr;
  */
  return newStr;
}

int addArg(char ***Xarg, int *argc, char *sBeg, char **sEnd)
{ char **arg;

  assert(Xarg);
  assert(argc);
  assert(sEnd);
  assert(sBeg);

    *sEnd = skip_word(sBeg);   /* find end of argument */

    /* Because *start != '\0' && !isargdelim(*start) ==> s != start */
    assert(*sEnd > sBeg);

      /* add new entry for new argument */
      if((arg = realloc(*Xarg, (*argc + 2) * sizeof(char *))) ==  NULL) {
        freep(*Xarg);
        return 1;
      }
      /* create new entry */
      if((arg[(*argc)++] = unquote(sBeg, *sEnd)) == NULL) {
        freep(arg);
        return 1;
      }
      *Xarg = arg;

    return 0;
  }


/*
 * split - splits a line up into separate arguments, deliminators
 *      are spaces and /'s
 *  Each option is placed into one individual argument.
 *  The passed in string is preserved.
 */
char **split(char *s, int *args)
{
  char **arg,
   *start;
  int ac;

  assert(args);

  arg = malloc(sizeof(char *));
  if (!arg)
    return NULL;
  ac = 0;

    /* skip to next argument */
  if(s) while (*(start = skipdm(s)) != '\0')
  {
    if(addArg(&arg, &ac, start, &s))
      return NULL;
  }

  arg[*args = ac] = NULL;
  return arg;
}

/*
 * freep -- frees memory used for a call to split
 *
 */
void freep(char **p)
{
  char **q;

  if((q = p) != NULL) {
    while (*q)
    free(*q++);
    free(p);
  }
}

/*
 *  Perform an option check and parse value for boolean
 */
int optScanBool_(const char * const optstr, int bool, const char *arg, int *value)
{
  assert(optstr);
  assert(value);

  if(arg) {
    error_opt_arg(optstr);
    return E_Useage;
  }
  switch(bool) {
  case -1:  *value = 0; break;
  case 0:   *value = !*value; break;
  case 1:   *value = 1; break;
#ifndef NDEBUG
  default:  fprintf(stderr, "Invalid boolean option value: in file %s line %u\n", __FILE__, __LINE__);
    abort();
#endif
  }
  return 0;
}


/*
 *  Perform an option check and parse value for integer
 */
#pragma argsused
int optScanInteger_(const char * const optstr, int bool, const char *arg, int *value)
{ int v;

  assert(optstr);
  assert(value);

  if(!arg || !*arg) {
    error_opt_noarg(optstr);
    return E_Useage;
  }

  v = 0;
  --arg;
  while(isdigit(*++arg))
    v = v * 10 + *arg - '0';

  if(*arg) {
    error_invalid_number(optstr);
    return E_Useage;
  }

  *value = v;
  return 0;
}

/*
 *  Perform an option check and parse value for string
 */
#pragma argsused
int optScanString_(const char * const optstr, int bool, const char *arg, char **value)
{
  assert(optstr);
  assert(value);

  if(!arg) {
    error_opt_noarg(optstr);
    return E_Useage;
  }
  free(*value);
  if((*value = strdup(arg)) == NULL) {
    error_out_of_memory();
    return E_NoMem;
  }
  return 0;
}

/*
 * Scan an option and update its value within the option array
 *  Return: 0 on success
 */
int scanOption(optScanner fct, void * const ag, char *rest)
{
  char *line, *arg, *optend;
  int ch, bool;

  assert(rest);

  if(!isoption(line = rest))  return -1;

  /* check for leading boolean character */
  switch(*++line) {
  case '\0':    return E_None;    /* empty option */
  case '-':   bool = -1; ++line; break;
  case '+':   bool = 1; ++line; break;
  default:    bool = 0; break;
  }

  if(!isprint(ch = toupper(*line)) || strchr("-+=:", ch)) {
    error_illformed_option(rest);
    return E_Useage;
  }

  if((optend = strpbrk(line, "=:")) != NULL) {  /* option has argument */
    arg = optend + 1;
  } else {
    arg = NULL;
    optend = strchr(line, '\0');
  }

  /* check for trailing boolean character */
  switch(optend[-1]) {
  case '-': bool = -1;
        --optend;
        break;
  case '+': bool = 1;
        --optend;
        break;
  }

  *optend = '\0';

  if(!fct) {
    error_invalid_lswitch(line);
    return E_Useage;
  }

  return fct(line, line[1]? 0: ch, bool, arg, ag);
}


/*
 * Scan all leading options
 */
int leadOptions(char **Xline, optScanner fct, void * arg)
{ int ec = E_None;
  char *p, *q, *line;

  assert(Xline && *Xline);

  p = *Xline;

  while(*(line = skipdm(p))) {
    q = unquote(line, p = skip_word(line));
    if(!q) {
      error_out_of_memory();
      return E_NoMem;
    }

    if(!isoption(q)
     || ((ec = scanOption(fct, arg, q)) != E_None
          && ec != E_Ignore)) {
      free(q);
      break;
    }

    free(q);
  }

  *Xline = line;

  return ec;
}

/*
 * Parse global options
 */
static int parseOptions(optScanner fct, void * const arg, char **argv, int *argc, int *optcnt)
{ int ec = E_None;
  char **argp, *a;

  assert(argv);
  assert(argc);
  assert(optcnt);

  *optcnt = 0;
  argp = argv;
  while((*argp++ = a = *argv++) != NULL)
    if(isoption(a))
      if((ec = scanOption(fct, arg, a)) == E_None) {
        free(*--argp);  /* ignore (overwrite) it */
        ++*optcnt;
      } else if(ec != E_Ignore) { /* the entry caused an error */
        /* Copy the remaining entries */
        while((*argp++ = *argv++) != NULL);
        break;
      }

  *argc -= *optcnt;
  return ec;    /* everything done */
}

/*
 * Scan command line and handle options
 *  line may be NULL and is treated as empty.
 *  opt may be NULL and is treated as "no valid option".
 *    This will print an error message, if an option is specified.
 *  Return:
 *    == NULL: error, message already on screen any argument possibly
 *      modified
 *    != NULL: argument buffer, must be freed with "freep()"
 *      + *argc := number of non-option arguments within array, &
 *      + *opts := number of parsed options.
 *      + array contains *argc entries now.
 */
char **scanCmdline(char *line, optScanner fct, void * const arg
  , int *argc, int *opts)
{ char **argv;

  assert(argc);
  assert(opts);

  if((argv = split(line, argc)) == NULL) {
    error_out_of_memory();
    return NULL;
  }

  if(parseOptions(fct, arg, argv, argc, opts) != E_None) {
    freep(argv);
    return NULL;
  }

  return argv;
}
