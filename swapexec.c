/*
   --- Version 3.4 93-06-22 13:40 ---

   EXEC.C: EXEC function with memory swap - Prepare parameters.

   Public domain software by

   Thomas Wagner
   Ferrari electronic GmbH
   Beusselstrasse 27
   D-1000 Berlin 21
   Germany

  2000/06/07 Ron Cemer
  fix: Prototype cleanup for TC++1 compatibly
 */

#include "config.h"

#ifdef FEATURE_SWAP_EXEC

#include "command.h"

#include "compat.h"
#include "swapexec.h"
#include <bios.h>

/* internal flags for prep_swap */

#define CREAT_TEMP      0x0080
#define DONT_SWAP_ENV   0x4000

#define ERR_COMSPEC     -7
#define ERR_NOMEM       -8

#ifdef __cplusplus
extern "C" {
#endif

int  do_spawn(int swapping,        /* swap if non-0 */
           char *xeqfn,         /* file to execute */
           char *cmdtail,       /* command tail string */
           unsigned envlen,     /* environment length */
           char *envp           /* environment pointer */
);

int  prep_swap(int method,         /* swap method */
            char *swapfn);      /* swap file name and/or path */

#ifdef __cplusplus
};
#endif

void spawn_check(int swapping, char *execfn, char *progpars)
{
  if (swapping > 0)
  {
    switch (swap_prep.swapmethod)
    {
      case USE_EMS:
        printf("Swapping to EMS handle %u\n", swap_prep.handle);
        break;

      case USE_XMS:
        printf("Swapping to XMS handle %u\n", swap_prep.handle);
        break;

      case USE_FILE:
        printf("Swapping to File <%s>\n", swap_prep.swapfilename);
        break;
    }
  }
  else
  {
    printf("swapping == %s (not good!)\n",swapping);
  }
  printf("Executing %s %s\n\n", execfn, progpars);
}

/*
   retcode = do_exec (filename to execute,
   program parameters,
   spawn options,
   memory needed (0xffff to always swap, 0 to never swap),
   environment pointer/flag)
 */

int do_exec(char *exfn, char *epars, int spwn, unsigned needed, char **envp)
{
  static char swapfn[MAXPATH];
  static char execfn[MAXPATH];
  unsigned avail;
  union REGS regs;
  unsigned envlen;
  int rc;
  char **env;
  char *ep, *envptr, *envbuf;
  char *progpars;
  char *tempname = NULL;
  int swapping;

  envlen = 0;
  envptr = NULL;
  envbuf = NULL;

  if (epars == NULL)
    epars = "";
  if (exfn == NULL)
    execfn[0] = 0;
  else
    strcpy(execfn, exfn);

  progpars = (char *)malloc(strlen(epars) + 1);
  if (progpars == NULL)
    return RC_NOFILE | -ERR_NOMEM;
  strcpy(progpars, epars);

  /* Now create a copy of the environment if the user wants it. */

  if (envp != NULL)
    for (env = envp; *env != NULL; env++)
      envlen += strlen(*env) + 1;

  if (envlen)
  {
    /* round up to paragraph, and alloc another paragraph leeway */
    envlen = (envlen + 32) & 0xfff0;
    envbuf = (char *)malloc(envlen);
    if (envbuf == NULL)
    {
      rc = RC_ENVERR;
      goto exit;
    }

    /* align to paragraph */
    envptr = envbuf;
    if (FP_OFF(envptr) & 0x0f)
      envptr += 16 - (FP_OFF(envptr) & 0x0f);
    ep = envptr;

    for (env = envp; *env != NULL; env++)
    {
      ep = stpcpy(ep, *env) + 1;
    }
    *ep = 0;
  }

  if (!spwn)
    swapping = -1;
  else
  {
    /* Determine amount of free memory */

    regs.x.ax = 0x4800;
    regs.x.bx = 0xffff;
    intdos(&regs, &regs);
    avail = regs.x.bx;

    /* No swapping if available memory > needed */

    if (needed < avail)
    {
       swapping = 0;
      dprintf(("No swap, enough memory already.\n"));
    }
    else
    {
      /* Swapping necessary, use 'TMP' or 'TEMP' environment variable
         to determine swap file path if defined. */

      swapping = spwn;
      if (spwn & USE_FILE)
      {
        if ((tempname = tmpfn()) != NULL)
        {
#if 0
          spwn &= ~USE_FILE;
          swapping = spwn;
#endif
          strcpy(swapfn, tempname);
          free(tempname);
        }
#if 0
        else
        {
          if (OS_MAJOR >= 3)
            swapping |= CREAT_TEMP;
        }
#endif
      }
    }
  }

  /* All set up, ready to go. */

  if (swapping > 0)
  {
    if (!envlen)
      swapping |= DONT_SWAP_ENV;

    dprintf(("[prep_swap %s]\n",swapfn));
    rc = prep_swap(swapping, swapfn);
    dprintf(("[prep returned %d]\n",rc));
    if (rc < 0)
      rc = RC_PREPERR | -rc;
    else
      rc = 0;
  }
  else
    rc = 0;

  if (!rc)
  {
    spawn_check(swapping, execfn, progpars);
    dprintf(("[do_spawn swapping=%d]\n",swapping));
    rc = do_spawn(swapping, execfn, progpars, envlen, envptr);
  }

  /* Free the environment buffer if it was allocated. */

exit:
  dprintf(("[do_spawn done]\n"));
  free(progpars);
  if (envlen)
    free(envbuf);

  return rc;
}

#endif
