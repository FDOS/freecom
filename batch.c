/*
 *  BATCH.C - batch file processor for COMMAND.COM.
 *
 *  Comments:
 *
 * ??/??/?? (Evan Jeffrey)
 *   started.
 *
 * 15 Jul 1995 (Tim Norman)
 *   modes and bugfixes.
 *
 * 08 Aug 1995 (Matt Rains)
 *   i have cleaned up the source code. changes now bring this source
 *   into guidelines for recommended programming practice.
 *
 *   i have added some constants to help making changes easier.
 *
 * 29 Jan 1996 (Steffan Kaiser)
 *   made a few cosmetic changes
 *
 * 05 Feb 1996 (Tim Norman)
 * - changed to comply with new first/rest calling scheme
 *
 * 14 Jun 1997 (Steffen Kaiser)
 * - bug fixes.  added error level expansion %?.  ctrl-break handling
 *
 * 16 Jul 1998 (Hans B Pufal)
 * - Totally reorganised in conjunction with COMMAND.C (cf) to implement
 *   proper BATCH file nesting and other improvements.
 *
 * 16 Jul 1998 (John P Price <linux-guru@gcfl.net>)
 * - Seperated commands into individual files.
 *
 * 19 Jul 1998 (Hans B Pufal) [HBP_001]
 * - Preserve state of echo flag across batch calls.
 *
 * 19 Jul 1998 (Hans B Pufal) [HBP_002]
 * - Implementation of FOR command
 *
 * 20-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added error checking after malloc calls
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 02-Aug-1998 (Hans B Pufal) [HBP_003]
 * - Fixed bug in ECHO flag restoration at exit from batch file
 *
 * 10-Aug-1998 ska
 * - added modifyable batchfile (standard behaviour)
 * - corrected ^Break processing
 * - added "newBatchContext()" to create/clear/chain a bcontext variable
 * - added initBatchContext() & clearBatchContext() to make changes in the
 *   bcontext structure easier.
 *
 * 12-Aug-1998 ska
 * - added tracemode variable, however, it's currently ignored
 *
 * 1998/12/05 ska
 * - fix: ECHO state of interactive command line was not properly preserved
 *
 * 1999/04/23 ska
 * bugfix: batch_param(): Missing right quote, after stopping the do-while
 *	loop, the '\0' byte is copied, then s2 incremented --> right of string.
 * bugfix: batch(): fullname is duplicated into heap, but
 *	not freed if this function fails
 * bugfix: batch(): no error checking of batch_params()
 *
 * 1999/05/06 ska		(see CMT2.TXT)
 * bugfix: FOR %a IN (dir\*.*) :: must expand to dir\...
 * bugfix: %9 wrong, if 9 > number of parameters
 * chg: %0 returns full name now
 * add: exit_all_batch() for session management
 *
 * 1999/07/08 ska
 * add: find_arg() returns a valid string for all, even negative indexes
 */

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dfn.h"

#include "command.h"
#include "cmdline.h"
#include "batch.h"

struct bcontext *bc = NULL;     /* The stack of current batch contexts.
                                 * NULL when no batch is active
                                 */

unsigned int echo = 1;          /* The echo flag */
int tracemode = 0;              /* debug trace of scripts */


/* Returns a pointer to the n'th parameter of the current batch file.
 * If no such parameter exists returns pointer to empty string.
 * If no batch file is current, returns NULL
 */
char *find_arg(int n)
{
  dprintf(("[find_arg (%d)]\n", n));

  if (bc == NULL)
    return NULL;

  n += bc->shiftlevel;
  if(n == 0)
  	return bc->bfnam;
  if(n > bc->numParams || n < 0)
  	return "";
  return bc->params[n - 1];
}

/*
 * setBatchParams builds a parameter list in newly allocated memory.
 * The parameters consist of null terminated strings with a final
 * NULL character signalling the end of the parameters.
 */
int setBatchParams(char *s)
{
  if((bc->params = split(s, &bc->numParams)) == NULL)
  {
    error_out_of_memory();
    return 0;
  }
  return 1;
}

/* Move init/clear functionality out of the files in order to centralize
 * the low-level functionality --> easier to add/remove members of bcontext
 */
void clearBatchContext(struct bcontext *b)
{
	assert(b);

  if (b->bfile)
    fclose(b->bfile);
  if (b->bfnam)
    free(b->bfnam);
  if (b->blabel)
    free(b->blabel);

  if (b->ffind)
    free(b->ffind);
  if (b->forproto)
    free(b->forproto);
  if (b->params)
    freep(b->params);
}

void initBatchContext(struct bcontext *b)
{
	assert(b);
  memset(b, 0, sizeof(*b));

  b->brewind = 1;
}

void exit_batch(void)
{
/*
 * If a batch file is current, exits it, freeing the context block and
 * chaining back to the previous one.
 *
 * If no new batch context is found, sets ECHO back ON.
 *
 * If the parameter is non-null or not empty, it is printed as an exit
 * message
 */

  dprintf(("exit_batch (..)\n"));

  if (bc)
  {
    struct bcontext
     *t = bc;

    if (bc->blabel)
      error_bfile_no_such_label(bc->bfnam, bc->blabel);

    clearBatchContext(bc);

    echo = bc->echo;            /* Preserve echo state across batch calls */
    bc = bc->prev;
    free(t);
  }

  if (!bc)                      /* Notify ^Break handler to cancel
  									"leave all" state */
    chkCBreak(BREAK_ENDOFBATCHFILES);
}

/* kill all batch contexts */
void exit_all_batch(void)
{	while(bc)
		exit_batch();
}


/*  Create/Clear/Chain all fields of the structure */
struct bcontext *newBatchContext(void)
{
  struct bcontext *b;

  b = malloc(sizeof(*b));
  if (!b)
  {
    error_out_of_memory();
    return NULL;
  }

  initBatchContext(b);
  b->echo = echo;               /* a new context must always preserve the
                                   current ECHO state */

  b->prev = bc;
  return bc = b;
}

/*
 *  Batch files are entitled to be "modifyable" and may span across
 *  floppy disks (you know: calling an external program that says
 *  "Please insert next floppy", then the INSTALL.BAT file on the
 *  the "next floppy" is accessed rather the originally opened one).
 *  The current implementation keeps the batchfile open, which is not
 *  the standard behaviour.
 */
int batch(char *fullname, char *firstword, char *param)
{
  /*
   * Start batch file execution
   *
   * The firstword parameter is the full filename of the batch file.
   */

   assert(fullname);
   assert(firstword);
   assert(param);

  if ((fullname = dfnexpand(fullname, NULL)) == NULL)
  {
    error_out_of_memory();
    return 1;
  }

  dprintf(("batch ('%s', '%s', '%s')\n", fullname, firstword,
           param));

  while (bc && bc->forvar)      /* Kill any and all FOR contexts */
    exit_batch();

  if (bc == NULL)               /* No current batch file, create new context */
  {
    if (!newBatchContext()) {
    	free(fullname);
      return 1;
    }
  }
  else
  {                             /* Then we are transferring to another batch */
    struct bcontext *q;
    int echo;

    clearBatchContext(bc);

    q = bc->prev;               /* preserve context chain intact */
    echo = bc->echo;            /* preserve former ECHO state */
    /* if the _current_ ECHO state would be preserved,
       the following case would forget about the
       ECHO state on the interactive command line:
       === File BATCH1.BAT
       @echo off
       batch2
       === File BATCH2.BAT
       @echo off
       ===
       The transfer to BATCH2 would destroy BTACH1's
       context and therefore the original ECHO state.
       because BATCH2 is called with ECHO OFF, this
       state would be preserved as the command line's
       ECHO state */
    initBatchContext(bc);
    bc->prev = q;
    bc->echo = echo;
  }

  bc->bfnam = fullname;         /* already duplicated */
  if(!setBatchParams(param)) {	 /* out of memory condition */
  	exit_batch();		/* clear this erroreous batch context */
  	return 1;
  }

  return 0;
}

char *readbatchline(int *eflag, char *textline, int size)
{
  /*
   * Read and return the next executable line from the current batch file
   *
   * If no batch file is current or no further executable lines are found
   * return NULL.
   *
   * Here we also look out for FOR bcontext structures which trigger the
   * FOR expansion code.
   *
   * Set eflag to 0 if line is not to be echoed else 1
   */

  char *first;
  char *ip;

  if (bc == NULL)               /* No batch */
    return NULL;

  dprintf(("readbatchline ()\n"));
  assert(textline);
  assert(size > 1);
  assert(eflag);

  ip = "";                      /* make sure ip != NULL in the first
  									iteration of the loop */
  while (bc)
  {
    first = NULL;               /* by default return "no file" */

    if (bc->forvar)             /* If its a FOR context... */
    {
      char
       *fv1,
       *sp,      /* pointer to prototype command */
       *dp,          /* Place to expand protoype */
       *fv;				       /* Next list element */

      if (chkCBreak(BREAK_FORCMD) || bc->shiftlevel > bc->numParams)
        /* End of list or User break so... */
      {
        exit_batch();           /* just exit this context */
        continue;
      }

      fv1 = fv = find_arg(0);

	if (bc->ffind) {          /* First already done fo do next */
		if(FINDNEXT(bc->ffind) != 0) {		/* no next file */
          free(bc->ffind);      /* free the buffer */
          bc->ffind = NULL;
          bc->shiftlevel++;     /* On to next list element */
          continue;
        }
	  fv = bc->ffind->ff_name;
	} else
	{
      if (strpbrk(fv, "?*") == 0) {      /* element is not wild file */
        bc->shiftlevel++;       /* No use it and shift list */
        fv1 = "";				/* No additional info */
      } else
        /* Wild file spec, find first (or next) file name */
      {
	  /*  For first find, allocate a find first block */
          if ((bc->ffind = (struct ffblk *)malloc(sizeof(struct ffblk)))
           == NULL)
          {
            error_out_of_memory();
            exit_batch();		/* kill this FOR context */
            break;
          }

         if(FINDFIRST(fv, bc->ffind, FA_NORMAL) == 0) {
         	/* found a file */
         	*dfnfilename(fv) = '\0';	/* extract path */
        	fv = bc->ffind->ff_name;
         } else {			/* if not found use the string itself */
			++bc->shiftlevel;
			fv1 = "";				/* No additional info */
        }

      }
      }

      /* At this point, fv points to parameter string */
      /* fv1 is the string usually set to the path to the
      	found file, otherwise it points to "" */

       sp = bc->forproto;      /* pointer to prototype command */
       dp = textline;          /* Place to expand protoype */

       assert(sp);

      while (*sp)
      {
        if (*sp == '%' && sp[1] == bc->forvar)  /* replace % var */
          dp = stpcpy(stpcpy(dp, fv1), fv), sp += 2;
        else
          *dp++ = *sp++;        /* Else just copy */
      }

      *dp = '\0';

      assert(dp - textline <= size);

      *eflag = echo;

      first = textline;
      break;
    }

    if (!bc->bfile)
    {                           /* modifyable batchfiles */
      if ((bc->bfile = fopen(bc->bfnam, "rt")) == NULL)
      {
        error_bfile_vanished(bc->bfnam);
        exit_batch();
        continue;
      }
      bc->bclose = 1;
      if (bc->brewind)
      {
        bc->brewind = 0;        /* fopen() position at start of file */
        bc->blinecnt = 0;
      }
      else if (fsetpos(bc->bfile, &bc->bpos))
      {                         /* end of file reached */
        /* so says MS COMMAND */
        exit_batch();
        continue;
      }
    }
    else if(bc->brewind) {
    	rewind(bc->bfile);
    	bc->brewind = 0;
    	bc->blinecnt = 0;
    }

    ++bc->blinecnt;
    if (chkCBreak(BREAK_BATCHFILE)      /* User break */
        || fgets(textline, size, bc->bfile) == NULL     /* End of file.... */
        || (ip = strchr(textline, '\n')) == NULL)       /* line too long */
    {
      if (!ip)
        error_long_batchline(bc->bfnam, bc->blinecnt);

      exit_batch();

      continue;
    }

    /* Strip leading spaces and trailing space/control chars */
    first = trim(textline);

    assert(first);

    /* ignore empty lines */
    if (!*first)
      continue;

    if (*first == ':')
    {
      /* if a label is searched for test if we reached it */
      if (bc->blabel)
      {
        ip = first;
        /* label: the 1st word immediately following the colon ':' */
        while (isgraph(*++ip)) ;
        *ip = '\0';
        if (stricmp(first + 1, bc->blabel) == 0)
        {                       /* OK found */
          free(bc->blabel);
          bc->blabel = NULL;
        }
      }
      continue;                 /* ignore label */
    }

    if (bc->blabel)
      continue;                 /* we search for a label! */

    if (*first == '@')          /* don't echo this line */
    {
    	first = ltrim(first + 1);
      *eflag = 0;
    }
    else
      *eflag = echo;

    break;
  }

  if (bc && bc->bclose)
  {                             /* modifyable batchfiles - ska */
    fgetpos(bc->bfile, &bc->bpos);
    fclose(bc->bfile);
    bc->bfile = NULL;
    bc->bclose = 0;
  }

  return first;
}
