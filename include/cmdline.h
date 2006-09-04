/*
  Declaration for the command line parsing interface
 */

#ifndef __CMDLINE_H
#define __CMDLINE_H

/* What quotes COMMAND shell honor (only paired quotes) */
/* Note: at least the double quotes MUST be included */
/* #define QUOTE_STR "\"'`" others are valid filename chars */
#define QUOTE_STR "\""

/* What characters COMMAND shall delimit arguments and
  options, useable w/in if() */
/* Note: Do not include the option character */
/* Also note: Do not include ',' or ';' within isoptdelim() */
#define isargdelim(ch) (isspace(ch) || iscntrl(ch) || strchr(",;=", ch))
#define isoptdelim(ch) (isspace(ch) || iscntrl(ch))

/* What option character COMMAND shell honor, used within arguments too */
#ifdef FEATURE_SWITCHAR
#define isoptch(ch) (ch == switchar())
#else
#define isoptch(ch) (ch == '/')
#endif

/* Test if an argument is an option */
#define isoption(string)  isoptch(*(string))

/* String ID of helpscreen of current internal command */
extern unsigned currCmdHelpScreen;

/* Trimming command line (arguments) basing on isargdelim() */
char *trimcl(char *str);
char *ltrimcl(const char *str);
void rtrimcl(char * const str);

char switchar(void);

/*
 * Callback function invoked when an option is scanned
 *
 *  optstr := long option string
 *  ch := single character option (upcased) or 0 if none
 *  bool := -1: '-' found; +1: '+' found; 0: neither
 *  strarg := NULL: no argument; else: pointer to option argument
 *    This string must be duplicated, because it will be removed as
 *    soon as the function returns.
 *
 *  Return:
 *    0 (E_None): OK
 *    else: some kind of error; error message already displayed
 *
 *  Note: BC5 with optimization enabled glues sequences like:
 *  switch(ch) {
 *  case 'A': return optScanBool(ch, bool, arg, &optA);
 *  case 'B': return optScanBool(ch, bool, arg, &optB);
 *    ...
 *  together. The generated code looks like:
 *  case 'A': push &optA
 *        goto callFct
 *  case 'B': push &optB
 *  callFct:  push arg
 *        push bool
 *        push ch
 *        call optScanBool
 *        add sp, 8
 *  One could say that no manual optimization is needed, couldn't one?
 */
typedef int (* const optScanner)(const char * const optstr, int ch, int bool, const char *strarg, void * const arg);


/*
 *  Define a callback function
 */
#define optScanFct(name)    \
  static int name (const char * const optstr, int ch, int bool, const char *strarg, void * const arg)

/*
 *  Print error message "Invalid option"
 */
#define optErr()  error_invalid_lswitch(optstr)

/*
 *  Check if a longname option matches, the argument must be in
 *  uppercase!
 */
#define optLong(str)  (strcmpi(optstr, (str)) == 0)


char *skipqword(const char *str, const char * const stop);

/* Use matchtok() to compare string constants */
#define match(line,word,len) match_(&(line), (word), (len))
#define matchtok(line,word) match_(&(line), word, sizeof(word) - 1)
int match_(char ** const p, const char * const word, int len);

char *skip_word(char *line);
char *skipdm(char *line);

char *unquote(const char *str, const char * const strend);
char **split(char *line, int *args);
void freep(char **p);

/* int isoption(char *str)  Return: 0: if str is no option */
int optScanString_(const char * const optstr, int bool, const char *arg, char **value);
int optScanBool_(const char * const optstr, int bool, const char *arg, int *value, int flip);
int optScanInteger_(const char * const optstr, int bool, const char *arg, int *value);
#define optHasArg()	(strarg != 0)

#define optScanString(var)    \
  optScanString_(optstr, bool, strarg, &(var))
#define optScanBool(var)    \
  optScanBool_(optstr, bool, strarg, &(var), 1)
#define optScanBool2(var)    \
  optScanBool_(optstr, bool, strarg, &(var), 0)
#define optScanInteger(var)   \
  optScanInteger_(optstr, bool, strarg, &(var))

int scanOption(optScanner fct, void * const ag, char *rest);
//int parseOptions(optScanner fct, void * const arg, char **argv, int *argc, int *optcnt);
int leadOptions(char **line, optScanner fct, void * const arg);
char **scanCmdline(char *line, optScanner fct, void * const arg, int *argc, int *opts);

#endif
