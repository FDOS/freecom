/*
 * LOADHIGH.C - command that loads a DOS executable into upper memory.
 *
 *   Comments
 *
 * ??/??/96 (Svante Frey)
 *   began.
 *
 * 01/17/96 (Tim Norman)
 *   plugged into COMMAND.COM
 *
 * 02/05/96 (Tim Norman)
 *   converted to new first/rest calling scheme
 *
 * 08/27/96 (Tim Norman)
 *   removed support for - as a switch character
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 2000/07/05 Ron Cemer
 *	bugfix: renamed skipwd() -> skip_word() to prevent duplicate symbol
 *
 * 2001/02/14 ska
 *	chg: made all helper functions and variables 'static'
 *	chg: clean up code to not implement some functions twice
 *	chg: reduced some static variables
 *
 * 2002/02/14 ska
 *	chg: to behave as documented in DOCS\LOADHIGH.TXT
 */

#include "../config.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <mcb.h>
#include <suppl.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/cswap.h"
#include "../err_fcts.h"
#include "../include/misc.h"
#include "../strings.h"

/* Macro to convert bytes to paragraphs */
#define topara(x) (((x) + 0xf) >> 4)
#define mcbAssign(mcb,wordValue)	(mcb = (struct MCB _seg *)(wordValue))
	/* skip to next MCB in chain */
#define mcbNext(mcb)	mcbAssign(mcb, nxtMCB((word)mcb))
#define DosAlloc(value)	DOSalloc((value), 0xF)

enum error_codes
{
  err_help = -1, OK,
  err_silent = -2,

  /* These error codes are more or less DOS-compatible */
  err_file_not_found = 2, err_mcb_chain = 7,
  err_out_of_memory = 8, err_invalid_parms = 87
};

static int lh_lf(char *c);
static int loadhigh_prepare(void);
static int loadfix_prepare(void);
static int findUMBRegions(void);
static int parseArgs(char *cmdline, char **fnam, char **rest);
static void lh_error(int errcode);

/* This module takes care of both the LOADHIGH and the LOADFIX command,
 * since those two commands have much in common.
 *
 * The global variable 'loadfix_flag' is used to keep track of which
 * command is currently executing. If this variable is non-zero, it's
 * LOADFIX, otherwise LOADHIGH.
 */

#ifdef INCLUDE_CMD_LOADHIGH

static int loadfix_flag;         /* Flag: LOADFIX instead of LOADHIGH? */
#define INCLUDE_LOADHIGH_HANDLER

/* This is the loadhigh handler */
#pragma argsused
int cmd_loadhigh(char *rest)
{
        loadfix_flag = 0;
        return lh_lf(rest);
}
#endif

#ifdef INCLUDE_CMD_LOADFIX

#ifndef INCLUDE_LOADHIGH_HANDLER
static int loadfix_flag;         /* Flag: LOADFIX instead of LOADHIGH? */
#define INCLUDE_LOADHIGH_HANDLER
#endif

/* This is the loadfix handler */
#pragma argsused
int cmd_loadfix(char *rest)
{
#ifdef FEATURE_XMS_SWAP
#error "LOADFIX is not compatible with XMS-Swap!"
#endif
  loadfix_flag = 1;
  return lh_lf(rest);
}
#endif

#ifdef INCLUDE_LOADHIGH_HANDLER

/* This array will contain the memory blocks that the new program can't use */
static int allocatedBlocks = 0;
static word *block = 0;

static int upper_flag;           /* Flag: should the program be loaded high? */

/* UMB region info */
static int umbRegions = 0;       /* How many UMB regions are there? */

static struct UMBREGION
{
  word start;             /* start of the region */
  word end;               /* end of the region */
  word minSize;           /* minimum free size, given by the L switch */
  int access;             /* does the program have access to this region? */
}
 *umbRegion = 0;


static int optS;
static char *optL = 0;

/* Helper functions */

static int initialise(void)
{
  /* reset global variables */
  allocatedBlocks = 0;
  upper_flag = 1;
  /* initialize options */
  optS = 0;

  /* Allocate dynamic memory for some arrays */
  if ((umbRegion = malloc(64 * sizeof(*umbRegion))) == 0)
    return err_out_of_memory;

  if ((block = malloc(256 * sizeof(*block))) == 0)
    return err_out_of_memory;

  /* find the UMB regions */
	return findUMBRegions();
}


/* This is the "real" handler of the two commands. The argument is
 * the original command line.
 */

static int lh_lf(char *args)
{
  int rc;
  char *fullname, *fnam;
	int i;	

  int old_link = dosGetUMBLinkState();
  int old_strat = dosGetAllocStrategy();

#ifdef FEATURE_XMS_SWAP
	assert(_CS < 0xa000);
#endif

  assert(args);
  assert(umbRegion == 0);
  assert(block == 0);
  assert(optL == 0);

    if((rc = initialise()) == OK)
    {
      if ((rc = parseArgs(args, &fnam, &args)) == OK)
      {
        /* command line was OK - try to find the file */

        fullname = find_which(fnam);
      free(fnam);

        if (fullname)
        {
          /* a file was found - allocate the memory */
          if (loadfix_flag)
            rc = loadfix_prepare();
          else
            rc = loadhigh_prepare();

          /* finally, execute the file */
          if (!rc)
            rc = exec(fullname, args, 0);

        }
        else
          rc = err_file_not_found;

      }
    }


  	/** Clean Up **/
  /* free any memory that was allocated to prevent the program from using it */
  for (i = 0; i < allocatedBlocks; i++)
    DOSfree(block[i]);

	/* free dynamic arrays */
	free(umbRegion);
	free(block);
	free(optL);
	optL = 0;
	umbRegion = 0;
	block = 0;

  /* Restore UMB link state and DOS malloc strategy to their
   * original values. */

  dosSetUMBLinkState(old_link);
  dosSetAllocStrategy(old_strat);


  /* if any error occurred, rc will hold the error code */
  if (rc)
    lh_error(rc);

  return rc;
}


/* lh_error(): print error messages to stderr */

static void lh_error(int errcode)
{
  switch (errcode)
  {
    case err_invalid_parms:
      error_syntax(0);
      break;

    case err_file_not_found:
      error_file_not_found();
      break;

    case err_out_of_memory:
      error_out_of_memory();
      break;

    case err_mcb_chain:        /* error while searching the MCB chain */
		error_bad_mcb_chain();
      break;

    default:
      error_unknown(errcode);
      break;

    case err_silent:
      break;
  }
}

/* findUMBRegions():
 * This routine scans the MCB chain to find all active memory regions.
 * Info about the regions is written to the array "umbRegions".
 *
 * Each continous region is numbered. Region 0 is the conventional
 * memory.
 */

static int findUMBRegions(void)
{
  struct UMBREGION *region = umbRegion;
  struct MCB _seg *mcb;
  char sig;
  int i;

	mcbAssign(mcb, GetFirstMCB());  /* get start of MCB chain */
  umbRegions = 0;
  region->start = FP_SEG(mcb);

  /* First, find the end of the conventional memory:
   * Turn UMB link off, and track the MCB chain to the end. */

  dosSetUMBLinkState(0);

  while (mcb->mcb_type == 'M')
    mcbNext(mcb);

  if (mcb->mcb_type != 'Z')
    return err_mcb_chain;

  /* If the last memory block in conventional memory is "reserved",
   * conventional memory ends at the paragraph before the block. If
   * the last block is an ordinary one, conventional memory ends at
   * the last paragraph of the block. */

  if (mcb->mcb_ownerPSP == 8 && !_fmemcmp(mcb->mcb_name, "SC", 2))
    region->end = FP_SEG(mcb) - 1;
  else
    region->end = FP_SEG(mcb) + mcb->mcb_size;

  region++;
  region->start = 0;

  /* Turn UMB link on. If MS-DOS UMBs are available, the signature of
   * the last conventional memory block will change from 'Z' to 'M'. */

  dosSetUMBLinkState(1);

  if (mcb->mcb_type == 'M')
  {                             /* UMBs are available */
    mcbNext(mcb); /* go to next block */

    /* This loop searches for the regions, by searching either for
     * special MCBs or 'reserved' memory regions. */

    do
    {
      sig = mcb->mcb_type;

      if (mcb->mcb_ownerPSP == 8 && !_fmemcmp(mcb->mcb_name, "SC", 2))
      {
        /* this is a 'hole' in memory */
        if (region->start)
        {
          region->end = FP_SEG(mcb) - 1;
          region++;
          region->start = 0;
        }
      }
      else
      {
        /* In MS-DOS 6.x, each UMB region starts with a 'major' mcb
         * that is outside the ordinary MCB chain. This mcb defines
         * the size of the whole region. */

        struct MCB _seg *umb_mcb;
        mcbAssign(umb_mcb, (word)mcb - 1);

        if((umb_mcb->mcb_type == 'Z' || umb_mcb->mcb_type == 'M')
         && !_fmemcmp(umb_mcb->mcb_name, "UMB     ", 8))
          {
            /* This is the signature of the special MS-DOS MCBs */

            region->start = umb_mcb->mcb_ownerPSP;
            region->end = umb_mcb->mcb_ownerPSP + umb_mcb->mcb_size - 1;
            if ((sig = umb_mcb->mcb_type) == 'M')
              region->end--;
            region++;
            region->start = 0;
            mcbAssign(mcb, (word)umb_mcb + umb_mcb->mcb_size);
            if (sig == 'Z')
              break;
            continue;
          }
        if (!region->start)
          region->start = FP_SEG(mcb);
      }

      if (sig == 'Z')
      {
        region->end = FP_SEG(mcb) + mcb->mcb_size;
        region++;
        break;
      }

      mcbNext(mcb);
    }
    while (sig == 'M');

    if (sig != 'Z')
      return err_mcb_chain;
  }
  umbRegions = region - umbRegion;

  /* By default, the program will have access to all UMB regions. This
   * may be modified by command-line arguments. */

  for (i = 0; i < umbRegions; i++)
  {
    umbRegion[i].access = 1;
    umbRegion[i].minSize = 0xffff;
  }

  return OK;
}

/* loadhigh_prepare():

 * This routine allocates memory as necessary. All memory that the
 * program is not allowed to access must be temporarily allocated
 * while the program is running.
 */

static int loadhigh_prepare(void)
{
  int i;
  struct UMBREGION far *region = umbRegion;
  word *availBlock;
  word availBlocks = 0;

  /* Set the UMB link and malloc strategy */
  dosSetUMBLinkState(1);
  dosSetAllocStrategy(0);

  if ((availBlock = malloc(256 * sizeof(*availBlock))) == 0)
    return err_out_of_memory;

  /* Call to force DOS to catenate any successive free memory blocks */
  DosAlloc(0xffff);

  /* This loop sets up each UMB region as specified:
   * If the 'L' switch was present on the command line, any memory
   * region not listed there will be disabled for the program.
   *
   * If a minimal size was specified, the memory region will only be
   * available to the program if it contains a free memory block of
   * at least this size. If the 'S' switch was present, the memory
   * region will be shrunk to the minimal size.
   *
   * When the loop finishes, the array 'block' will contain the memory
   * handles of every free memory block that the program was not
   * allowed to access.
   *
   * The array 'availBlock' will contain the memory handles of every
   * free memory block that the program can use.
   */

  for (i = 0; i < umbRegions; i++, region++)
  {
    struct MCB _seg *mcb;
    word startBlock = allocatedBlocks;
    int found_one = 0;

    for (mcbAssign(mcb, region->start)
     ; FP_SEG(mcb) < region->end && mcb->mcb_type == 'M'
     ; mcbNext(mcb))
    {
      if (!mcb->mcb_ownerPSP)
      {
        /* Found a free memory block: allocate it */
        word bl = DosAlloc(mcb->mcb_size);

        if (bl != FP_SEG(mcb) + 1)  /* Did we get the block we wanted? */
          return err_mcb_chain;

        if (region->access)		/* /L option allows access to this region */
        {
          if (region->minSize == 0xffff ||		/* no minimum size set
          											--> use it */
              (!optS && found_one) ||	/* a found previous block means to
              					make this region available regardless
              					of the size of this block; with an active
              					/S option only one block per region is
              					allowed. */
              (!(optS && found_one) &&
               mcb->mcb_size >= region->minSize))
          {

            availBlock[availBlocks++] = bl;

            if (optS)
              DOSresize(bl, region->minSize);
            else if (allocatedBlocks > startBlock)
            {
              /* These _previously_ found blocks had been found
              	too small, but must be made available now as this
              	region is available */
              memcpy(availBlock + availBlocks
               , block + startBlock
               , (allocatedBlocks - startBlock) * sizeof(*block));
              availBlocks += allocatedBlocks - startBlock;
              allocatedBlocks = startBlock;
            }
            found_one = 1;

            continue;
          }
        }
        block[allocatedBlocks++] = bl;  /* no access to this block */
      }
    }
  }

  /* Now, block[] contains the blocks that can't be used by the
   * program, and availBlock[] contains the blocks that can be used.
   * Those blocks will be released. */

  for (i = 0; i < availBlocks; i++)
    DOSfree(availBlock[i]);
  free(availBlock);

  /* If the program is to be loaded in upper memory, set the malloc
   * strategy to 'first fit high', otherwise to 'first fit low'. */

  dosSetAllocStrategy(upper_flag ? 0x80 : 0);
  return OK;
}

/* loadfix_prepare(): Allocates all memory up to 1000:0000. */

static int loadfix_prepare(void)
{
  struct MCB _seg *mcb;

  mcbAssign(mcb, umbRegion[0].start);

  dosSetAllocStrategy(0);

  while (FP_SEG(mcb) < 0x1000)
  {
    if (mcb->mcb_type != 'M' && mcb->mcb_type != 'Z')
      return err_mcb_chain;

    if (!mcb->mcb_ownerPSP)
    {
      word bl = DosAlloc(mcb->mcb_size);

      if (bl != FP_SEG(mcb) + 1)  /* Did we get the block we wanted? */
        return err_mcb_chain;

      block[allocatedBlocks++] = bl;

      if (bl + mcb->mcb_size > 0x1000)  /* Don't allocate more than necessary */
        DOSresize(bl, 0x1000 - bl);
    }
    mcbNext(mcb);
  }
  return OK;
}


#pragma argsused
optScanFct(opt_lh)
{ switch(ch) {
  case 'S': return optScanBool(optS);
  case 'L': return optScanString(optL);
  }
  optErr();
  return E_Useage;
}


/*  parseArgs(): parse the command line

 *  Only '/' is recognized as switch characters.
 *  The first argument not starting with '/' will be
 *  considered to be the filename.
 *
 *  The LOADFIX command only accepts the '?' switch.
 */

static int parseArgs(char *cmdline, char **fnam, char **rest)
{
  char *c;

  assert(fnam);
  assert(rest);

  if(leadOptions(&cmdline, loadfix_flag? 0: opt_lh, 0) != E_None)
      return err_silent;

  if((c = optL) != 0) {
    int i, r;

    /* Disable access to all UMB regions not listed here */
    for (i = 1; i < umbRegions; i++)
    umbRegion[i].access = 0;

    r = 0;

    do
    {
    dword region_minSize = 0xffff;  /* flag value, indicating no minsize was specified */

    int region_number = (int)strtol(c, &c, 10);

    if (*c == ',')
    {
    long larg;
      c++;
      if ((larg = strtol(c, &c, 10)) != -1L)
      region_minSize = topara(larg);
    }

    if (region_number >= umbRegions)
      displayString(TEXT_ERROR_REGION_WARNING, region_number);
    else
    {
      if (!r && !region_number)
      upper_flag = 0;

      r++;
      assert(region_minSize < 0x10000ul);
      umbRegion[region_number].minSize = (unsigned)region_minSize;
      umbRegion[region_number].access = 1;
    }
    }
    while (*c++ == ';');
    if(c[-1])
            cmdline = ""; /* to cause an error later */
  }

  /* does a file name follow? */
  if (!*cmdline)
    return err_invalid_parms;

  if (optS && !optL)
    return err_invalid_parms;

  /* The next argument is the file name. The rest of the command line
   * are passed as parameters to the new program. */

  if((*fnam = unquote(cmdline, c = skip_word(cmdline))) == 0)
    return err_out_of_memory;

  *rest = skipdm(c);

  return OK;
}

#endif
