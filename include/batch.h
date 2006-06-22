/* A structure to preserve the context of a batch file */

/*  Added:
   bfnam -- name of batchfile for modifyable scripts if bfile == NULL
   bpos -- position within batchfile, if bfile == NULL
   blinecnt  -- line counter
   bclose -- close file if it was opened temporarily
   brewind -- rewind file instead of positioning to bpos
   blabel -- to search for if != NULL
*/

#include <stdio.h>
#ifndef __PACIFIC__
#include <dir.h>
#endif

#define __LFNFUNCS_C
#include "../include/lfnfuncs.h"

struct bcontext
{
  struct bcontext *prev;
  FILE *bfile;
  char *bfnam;                  /* abs filename of batchfile */
  char *bfirst;                 /* name of batchfile as typed on cmd line */
  char *forproto;				/* command to execute */
  char *forvar;					/* current FOR variable */
  char **params;				/* FOR/batch parameter list */
  char *blabel;                 /* label to search for */
#ifdef FEATURE_LONG_FILENAMES
  struct lfnffblk *ffind;
#else
  struct ffblk *ffind;			/* already started FOR wildcard expand loop */
#endif
#ifndef __PACIFIC__
  fpos_t bpos;                  /* position within file if bfile == NULL */
#else
  unsigned long bpos;           /* position within file if bfile == NULL */
#endif
  long blinecnt;                /* line counter */
  int shiftlevel;				/* number of skipped arguments */
  int echo;                     /* Preserve echo flag across batch calls */
  int bclose;					/* close batch file on exit */
  int brewind;					/* rewind batch file next time */
  int numParams;				/* number of parameters */
};

/*  The stack of current batch contexts.
 * NULL when no batch is active
 */
extern struct bcontext *bc;

void exit_batch(void);
void exit_all_batch(void);

extern setBatchParams(char *s);

extern struct bcontext *newBatchContext(void);
/* static void initBatchContext(struct bcontext *b); */
extern struct bcontext *activeBatchContext(void);
char *find_arg_bc(struct bcontext const * const b, int n);

#define	find_arg(num)	find_arg_bc(activeBatchContext(), (num))
#define getArgCur(num)	find_arg_bc(bc, (num))
