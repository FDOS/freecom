/*
 *  DSTACK.C - Directory stack PUSHD/POPD support, compatible with 4/NDOS
 *
 *
 *
 *  Comments:
 *  06/01/2000 - D. Lucas Parker
 *  - started
 *
 *	2000/06/22 ska
 *	chg: using cmd_cdd() instead to change directory direct
 *	chg: using dfnpath() instead of getdcwd()
 *	chg: informatial message using displayString().
 *	chg: make internal buffers depending on PUSHD only
 */

#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <string.h>
#include "dfn.h"

#include "config.h"
#include "command.h"
#include "strings.h"

#ifndef DIR_STACK_LEN
#define DIR_STACK_LEN 256       /* Should be no less than MAXPATH */
                                /* as defined in DIR.H            */
#endif

#if DIR_STACK_LEN <= MAXPATH
#error "DIR_STACK_LEN must be greater than MAXPATH"
#endif

#ifdef INCLUDE_CMD_PUSHD
/* Directory stack */
static char Dir_Stack[DIR_STACK_LEN];
static char Buffer[DIR_STACK_LEN];     /* memmove doesn't like to overlap */
static int Dir_Stack_Pointer = 0;
static int Dir_Stack_Entries = 0;
#endif

/*
 * Pushes the current directory onto the stack.  If the length of
 * the directory string exceeds the remainer of the stack, it pops the first
 * entry, adjusts, and proceeds with the push.
 * Also: changes to the directory specified on the command line.
 */
#ifdef INCLUDE_CMD_PUSHD
int cmd_pushd(char *rest)
   {
   char *Tmp_Stack = Dir_Stack;
   char *curdir;
   int i = 1;

   if((curdir = cwd(0)) == 0) {
   	return 1;
   }

   while(Dir_Stack_Pointer + strlen(curdir) > DIR_STACK_LEN)
      {
      while(*Tmp_Stack++)
         i++;

      /* Copy all but the first entry to the buffer, and then put it back */
      memcpy(Buffer, Tmp_Stack, i + Dir_Stack_Pointer - 1);
      Dir_Stack_Pointer -= i;
      memcpy(Dir_Stack, Buffer, Dir_Stack_Pointer);
      Dir_Stack_Entries--;
      }

   /* Append the stack and update the stack pointer */
   strcpy(Dir_Stack + Dir_Stack_Pointer, curdir);
   Dir_Stack_Pointer += strlen(curdir) + 1;
   Dir_Stack[Dir_Stack_Pointer - 1] = '\0';
   Dir_Stack_Entries++;

   free(curdir);

   /* Change to directory specified on command line */
   if(rest && *rest)
   	return cmd_cdd(rest);

   return 0;
   }
#endif

/*
 * Pops a directory off the stack, and sets it as cwd.
 * Accepts the parameter '*', causing it to wipe the stack.
 */
#ifdef INCLUDE_CMD_POPD
int cmd_popd(char *rest)
   {
#ifdef INCLUDE_CMD_PUSHD
   char *Tmp_Stack = Dir_Stack;
   char newdir[MAXPATH];
   int i = 0;

   /* '*' == Wipe stack contents */
   if(rest && *rest == '*')
      {
      Dir_Stack_Entries = 0;
      Dir_Stack_Pointer = 0;
      }
   /* If there is anything in the stack */
   else if(Dir_Stack_Entries)
      {
      /* Find the last entry */
      while(++i < Dir_Stack_Entries)
         while(*Tmp_Stack++) ;

      /* Copy the pathname to a buffer */
      strcpy(newdir, Tmp_Stack);
      /* Update pointer and such */
      Dir_Stack_Pointer -= strlen(Tmp_Stack) + 1;
      Dir_Stack_Entries -= 1;

	  return cmd_cdd(newdir);
      }
   /* If the stack is empty */
   else
#endif
   	displayString(TEXT_DIRSTACK_EMPTY);

   return 0;
   }
#endif

/* Outputs directory stack contents */
#ifdef INCLUDE_CMD_DIRS
#pragma argsused
int cmd_dirs(char *rest)
   {
#ifdef INCLUDE_CMD_PUSHD
   char *Tmp_Stack = Dir_Stack;
   int i = 0;

   if(Dir_Stack_Entries)
      {
      while(i < Dir_Stack_Entries)
         {
         puts(Tmp_Stack);
         while(*Tmp_Stack++) ;
         i++;
         }
      }
   else
#endif
      displayString(TEXT_DIRSTACK_EMPTY);

   return 0;
   }
#endif
