/*
	--- Version 3.3 91-11-21 16:12 ---

   EXEC.H: EXEC function with memory swap - Main function header file.

   Public domain software by

        Thomas Wagner
        Ferrari electronic GmbH
        Beusselstrasse 27
        D-1000 Berlin 21
        Germany
*/


extern int do_exec (char *xfn, char *pars, int spawn, unsigned needed,
						  char **envp);

/*
   The EXEC function.

      Parameters:

         xfn      is a string containing the name of the file
                  to be executed. If the string is empty,
                  the COMSPEC environment variable is used to
                  load a copy of COMMAND.COM or its equivalent.
                  If the filename does not include a path, the
                  current PATH is searched after the default.
                  If the filename does not include an extension,
                  the path is scanned for a COM, EXE, or BAT file
                  in that order.

         pars     The program parameters.

         spawn    If 0, the function will terminate after the
                  EXECed program returns, the function will not return.

                  NOTE: If the program file is not found, the function
                        will always return with the appropriate error
                        code, even if 'spawn' is 0.

                  If non-0, the function will return after executing the
                  program. If necessary (see the "needed" parameter),
                  memory will be swapped out before executing the program.
                  For swapping, spawn must contain a combination of the
                  following flags:

                     USE_EMS  (0x01)  - allow EMS swap
                     USE_XMS  (0x02)  - allow XMS swap
                     USE_FILE (0x04)  - allow File swap

                  The order of trying the different swap methods can be
                  controlled with one of the flags

                     EMS_FIRST (0x00) - EMS, XMS, File (default)
                     XMS_FIRST (0x10) - XMS, EMS, File

                  If swapping is to File, the attribute of the swap file
                  can be set to "hidden", so users are not irritated by
                  strange files appearing out of nowhere with the flag

                     HIDE_FILE (0x40)    - create swap file as hidden

                  and the behaviour on Network drives can be changed with

                     NO_PREALLOC (0x100) - don't preallocate
                     CHECK_NET (0x200)   - don't preallocate if file on net.

                  This checking for Network is mainly to compensate for
                  a strange slowdown on Novell networks when preallocating
                  a file. You can either set NO_PREALLOC to avoid allocation
                  in any case, or let the prep_swap routine decide whether
                  to do preallocation or not depending on the file being
                  on a network drive (this will only work with DOS 3.1 or
                  later).

         needed   The memory needed for the program in paragraphs (16 Bytes).
                  If not enough memory is free, the program will
                  be swapped out.
                  Use 0 to never swap, 0xffff to always swap.
                  If 'spawn' is 0, this parameter is irrelevant.

         envp     The environment to be passed to the spawned
                  program. If this parameter is NULL, a copy
                  of the parent's environment is used (i.e.
                  'putenv' calls have no effect). If non-NULL,
                  envp must point to an array of pointers to
                  strings, terminated by a NULL pointer (the
                  standard variable 'environ' may be used).

      Return value:

         0x0000..00FF: The EXECed Program's return code

         0x0101:       Error preparing for swap: no space for swapping
         0x0102:       Error preparing for swap: program too low in memory

         0x0200:       Program file not found
         0x0201:       Program file: Invalid drive
         0x0202:       Program file: Invalid path
         0x0203:       Program file: Invalid name
         0x0204:       Program file: Invalid drive letter
         0x0205:       Program file: Path too long
         0x0206:       Program file: Drive not ready
         0x0207:       Batchfile/COMMAND: COMMAND.COM not found
         0x0208:       Error allocating temporary buffer

         0x03xx:       DOS-error-code xx calling EXEC

         0x0400:       Error allocating environment buffer

         0x0500:       Swapping requested, but prep_swap has not
                       been called or returned an error.
         0x0501:       MCBs don't match expected setup
         0x0502:       Error while swapping out

         0x0600:       Redirection syntax error
         0x06xx:       DOS error xx on redirection
*/


/* Return codes (only upper byte significant) */

#define RC_PREPERR   0x0100
#define RC_NOFILE    0x0200
#define RC_EXECERR   0x0300
#define RC_ENVERR    0x0400
#define RC_SWAPERR   0x0500
#define RC_REDIRERR  0x0600

/* Swap method and option flags */

#define USE_EMS      0x01
#define USE_XMS      0x02
#define USE_FILE     0x04
#define EMS_FIRST    0x00
#define XMS_FIRST    0x10
#define HIDE_FILE    0x40
#define NO_PREALLOC  0x100
#define CHECK_NET    0x200

#define USE_ALL      (USE_EMS | USE_XMS | USE_FILE)


/*
   The function pointed to by "spawn_check" will be called immediately
   before doing the actual swap/exec, provided that

      - the preparation code did not detect an error, and
      - "spawn_check" is not NULL.

   The function definition is
      int name (int cmdbat, int swapping, char *execfn, char *progpars)

   The parameters passed to this function are

      cmdbat      1: Normal EXE/COM file
                  2: Executing BAT file via COMMAND.COM
                  3: Executing COMMAND.COM (or equivalent)

      swapping    < 0: Exec, don't swap
                    0: Spawn, don't swap
                  > 0: Spawn, swap

      execfn      the file name to execute (complete with path)

      progpars    the program parameter string

   If the routine returns anything other than 0, the swap/exec will
   not be executed, and do_exec will return with this code.

   You can use this function to output messages (for example, the
   usual "enter EXIT to return" message when loading COMMAND.COM)
   and to do clean-up and additional checking.

   CAUTION: If swapping is > 0, the routine may not modify the
   memory layout, i.e. it may not call any memory allocation or
   deallocation routines.

   "spawn_check" is initialized to NULL.
*/

//typedef int (spawn_check_proc)(int cmdbat, int swapping, char *execfn, char *progpars);
//extern spawn_check_proc *spawn_check;

/*
   The 'swap_prep' variable can be accessed from the spawn_check
   call-back routine for additional information on the nature and
   parameters of the swap. This variable will ONLY hold useful
   information if the 'swapping' parameter to spawn_check is > 0.
   The contents of this variable may not be changed.

   The 'swapmethod' field will contain one of the flags USE_FILE,
   USE_XMS, or USE_EMS.

   Caution: The module using this data structure must be compiled
   with structure packing on byte boundaries on, i.e. with /Zp1 for
   MSC, or -a- for Turbo/Borland.
*/

typedef struct {
               long xmm;            /* XMM entry address */
               int first_mcb;       /* Segment of first MCB */
               int psp_mcb;         /* Segment of MCB of our PSP */
               int env_mcb;         /* MCB of Environment segment */
               int noswap_mcb;      /* MCB that may not be swapped */
               int noswap_mcb2;     /* Handle MCB that may not be swapped */
               int ems_pageframe;   /* EMS page frame address */
               int handle;          /* EMS/XMS/File handle */
               int total_mcbs;      /* Total number of MCBs */
               char swapmethod;     /* Method for swapping */
               char swapfilename[81]; /* Swap file name if swapping to file */
               } prep_block;

extern prep_block swap_prep;

