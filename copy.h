/*
 * COPY.H -- Internal Copy Command Header File
 *
 * 01-Aug-98 (Rob Lake z63rrl@morgan.ucs.mun.ca)
 *      - started
 */

#define VERIFY  1               /* VERIFY Switch */
#define BINARY  2               /* File is to be copied as BINARY */
#define ASCII   4               /* File is to be copied as ASCII */
#define PROMPT  8               /* Prompt before overwriting files */
#define NPROMPT 16              /* Do not prompt before overwriting files */
#define HELP    32              /* Help was asked for */
#define SOURCE  128             /* File is a source */

//#ifdef DEBUG
//#define dprintf(p)  printf p
//#else
//#define dprintf(p)
//#endif

typedef struct files
{
  char file[MAXPATH];
  unsigned flag;                /* BINARY -xor- ASCII */
  struct files *next;
}
files;

int do_switches(char *, unsigned *);
int add_file(files *, char *, int *, int *, unsigned *);
int add_files(files *, char *, int *, int *, int *, unsigned *);
int get_dest(files *, files *);
int parse_cmd(files *, int, char **, unsigned *);
void freep(char **);
void delete_list(files *);
int overwrite(char *);
//int copy(struct files, char *, int, unsigned *);
