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
 */

#include "config.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <dos.h>                /* must have those MK_FP() macros */
#include <dir.h>                /* for searchpath() */

#include "command.h"            /* command shell interface functions */
#include "loadhigh.h"           /* contains macros, global variables, etc */
#include "strings.h"
#include "cmdline.h"

/* This module takes care of both the LOADHIGH and the LOADFIX command,
 * since those two commands have much in common.
 *
 * The global variable 'loadfix_flag' is used to keep track of which
 * command is currently executing. If this variable is non-zero, it's
 * LOADFIX, otherwise LOADHIGH.
 */

#ifdef INCLUDE_CMD_LOADHIGH
/* This is the loadhigh handler */
#pragma argsused
int cmd_loadhigh(char *rest)
{
        loadfix_flag = 0;
        return lh_lf(rest);
}
#endif

#ifdef INCLUDE_CMD_LOADFIX
/* This is the loadfix handler */
#pragma argsused
int cmd_loadfix(char *rest)
{
  loadfix_flag = 1;
  return lh_lf(rest);
}
#endif

#if defined(INCLUDE_CMD_LOADHIGH)
#define INCLUDE_LOADHIGH_HANDLER
#elif defined(INCLUDE_CMD_LOADFIX)
#define INCLUDE_LOADHIGH_HANDLER
#endif

#ifdef INCLUDE_LOADHIGH_HANDLER

static int optS;
static char *optL;

/* This is the "real" handler of the two commands. The argument is
 * the original command line.
 */

int lh_lf(char *args)
{
  int rc = err_out_of_memory;
  char *fullname, *fnam;

  assert(args);


    if (initialise() == OK)
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
    cleanup();

  /* if any error occurred, rc will hold the error code */
  if (rc)
    lh_error(rc);

  return rc;
}

int initialise(void)
{
  int rc;

  /* reset global variables */
  allocatedBlocks = 0;
  upper_flag = 1;

  /* Save the UMB link state and the DOS malloc strategy, to restore them later */
  old_link = DosSetUMBLink(1);
  old_strat = DosSetStrategy(0);

  /* Allocate dynamic memory for some arrays */
  if ((umbRegion = malloc(64 * sizeof(*umbRegion))) == NULL)
    return err_out_of_memory;

  if ((block = malloc(256 * sizeof(*block))) == NULL)
    return err_out_of_memory;

  /* find the UMB regions */
  if ((rc = findUMBRegions()) != 0)
    return rc;

  return OK;
}

void cleanup(void)
{
  int i;

  /* free any memory that was allocated to prevent the program from using it */
  for (i = 0; i < allocatedBlocks; i++)
    DosFree(block[i]);

  /* free dynamic arrays */
  free(umbRegion);
  free(block);
  free(optL);

  /* Restore UMB link state and DOS malloc strategy to their
   * original values. */

  DosSetUMBLink(old_link);
  DosSetStrategy(old_strat);
}

/* lh_error(): print error messages to stderr */

void lh_error(int errcode)
{
  switch (errcode)
  {
    case err_invalid_parms:
      displayString(TEXT_ERROR_SYNTAX);
      break;

    case err_file_not_found:
      displayString(TEXT_ERROR_FILE_NOT_FOUND);
      break;

    case err_out_of_memory:
      displayString(TEXT_ERROR_OUT_OF_MEMORY);
      break;

    case err_mcb_chain:        /* error while searching the MCB chain */
      displayString(TEXT_ERROR_BAD_MCB_CHAIN);
      break;

    default:
      displayString(TEXT_ERROR_UNDEFINED_ERROR,errcode);
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

int findUMBRegions(void)
{
  struct UMBREGION *region = umbRegion;
  struct MCB far *mcb = MK_FP(GetFirstMCB(), 0);  /* get start of MCB chain */
  char sig;
  int i;

  umbRegions = 0;
  region->start = FP_SEG(mcb);

  /* First, find the end of the conventional memory:
   * Turn UMB link off, and track the MCB chain to the end. */

  DosSetUMBLink(0);

  while (mcb->sig == 'M')
    FP_SEG(mcb) += mcb->size + 1;

  if (mcb->sig != 'Z')
    return err_mcb_chain;

  /* If the last memory block in conventional memory is "reserved",
   * conventional memory ends at the paragraph before the block. If
   * the last block is an ordinary one, conventional memory ends at
   * the last paragraph of the block. */

  if (mcb->owner == 8 && !farmemcmp(mcb->name, "SC", 2))
    region->end = FP_SEG(mcb) - 1;
  else
    region->end = FP_SEG(mcb) + mcb->size;

  region++;
  region->start = 0;

  /* Turn UMB link on. If MS-DOS UMBs are available, the signature of
   * the last conventional memory block will change from 'Z' to 'M'. */

  DosSetUMBLink(1);

  if (mcb->sig == 'M')
  {                             /* UMBs are available */
    FP_SEG(mcb) += mcb->size + 1; /* go to next block */

    /* This loop searches for the regions, by searching either for
     * special MCBs or 'reserved' memory regions. */

    do
    {
      sig = mcb->sig;

      if (mcb->owner == 8 && !farmemcmp(mcb->name, "SC", 2))
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

        struct MCB far *umb_mcb = mcb;
        FP_SEG(umb_mcb)--;

        if (umb_mcb->sig == 'Z' || umb_mcb->sig == 'M')
          if (!farmemcmp(umb_mcb->name, "UMB     ", 8))
          {
            /* This is the signature of the special MS-DOS MCBs */

            mcb = umb_mcb;
            region->start = mcb->owner;
            region->end = mcb->owner + mcb->size - 1;
            if ((sig = mcb->sig) == 'M')
              region->end--;
            region++;
            region->start = 0;
            FP_SEG(mcb) += mcb->size;
            if (sig == 'Z')
              break;
            continue;
          }
        if (!region->start)
          region->start = FP_SEG(mcb);
      }

      if (sig == 'Z')
      {
        region->end = FP_SEG(mcb) + mcb->size;
        region++;
      }

      FP_SEG(mcb) += mcb->size + 1;
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

int loadhigh_prepare(void)
{
  int i;
  struct UMBREGION far *region = umbRegion;
  WORD *availBlock;
  WORD availBlocks = 0;

  /* Set the UMB link and malloc strategy */
  DosSetUMBLink(1);
  DosSetStrategy(0);

  if ((availBlock = malloc(256 * sizeof(*availBlock))) == NULL)
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
    struct MCB far *mcb;
    WORD startBlock = allocatedBlocks;
    int found_one = 0;

    for (mcb = MK_FP(region->start, 0); FP_SEG(mcb) < region->end;
         FP_SEG(mcb) += mcb->size + 1)
    {
      if (!mcb->owner)
      {
        /* Found a free memory block: allocate it */
        WORD bl = DosAlloc(mcb->size);

        if (bl != FP_SEG(mcb) + 1)  /* Did we get the block we wanted? */
          return err_mcb_chain;

        if (region->access)
        {
          if (region->minSize == 0xffff ||
              (!optS && found_one) ||
              (!(optS && found_one) &&
               mcb->size >= region->minSize))
          {

            availBlock[availBlocks++] = bl;

            if (optS)
              DosResize(bl, region->minSize);
            else if (allocatedBlocks > startBlock)
            {
              memcpy(availBlock + availBlocks, block + startBlock, (allocatedBlocks - startBlock) * sizeof(*block));
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
    DosFree(availBlock[i]);
  free(availBlock);

  /* If the program is to be loaded in upper memory, set the malloc
   * strategy to 'first fit high', otherwise to 'first fit low'. */

  DosSetStrategy(upper_flag ? 0x80 : 0);
  return OK;
}

/* loadfix_prepare(): Allocates all memory up to 1000:0000. */

int loadfix_prepare(void)
{
  struct MCB far *mcb = MK_FP(umbRegion[0].start, 0);

  DosSetStrategy(0);

  while (FP_SEG(mcb) < 0x1000)
  {
    if (mcb->sig != 'M' && mcb->sig != 'Z')
      return err_mcb_chain;

    if (!mcb->owner)
    {
      WORD bl = DosAlloc(mcb->size);

      if (bl != FP_SEG(mcb) + 1)  /* Did we get the block we wanted? */
        return err_mcb_chain;

      block[allocatedBlocks++] = bl;

      if (bl + mcb->size > 0x1000)  /* Don't allocate more than necessary */
        DosResize(bl, 0x1000 - bl);
    }
    FP_SEG(mcb) += mcb->size + 1;
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

int parseArgs(char *cmdline, char **fnam, char **rest)
{
  char *c;

  assert(fnam);
  assert(rest);

  /* initialize options */
  optS = 0;
  optL = NULL;

  if(leadOptions(&cmdline, loadfix_flag? NULL: opt_lh, NULL) != E_None)
      return err_silent;

  if((c = optL) != NULL) {
    int i, r;

    /* Disable access to all UMB regions not listed here */
    for (i = 1; i < umbRegions; i++)
    umbRegion[i].access = 0;

    r = 0;

    do
    {
    DWORD region_minSize = 0xffff;  /* flag value, indicating no minsize was specified */

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

  if((*fnam = unquote(cmdline, c = skip_word(cmdline))) == NULL)
    return err_out_of_memory;

  *rest = skipdm(c);

  return OK;
}

#endif
