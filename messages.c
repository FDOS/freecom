/*
 * MESSAGES.C - Read messages from strings database located in a file or
 *              at the end of command.com
 *
 *
 * Comments:
 *
 * 27 Jul 1998  John P. Price
 * - started.
 *
 * 03-Dec-1998  John P. Price
 * - Reduced the ammount of memory allocated by not reading in entire index,
 *   but only the index element needed.
 *
 * 1998/12/05 ska
 * - chg: split displayString() into two parts: 1) load string into
 *  local heap, 2) display string
 * - add: load messages into conventional far memory, high memory tried
 *  first
 *
 * 11-Feb-1999  John P. Price <linux-guru@gcfl.net>
 * - Changed openStrFile() function to try to read the strings from the end
 *   of command first, and then to try to read strings.dat in the same
 *   directory as command.com.  This helps when running from the debugger.
 *
 * 1999/07/07 ska
 * chg: the messages are always loaded into memory now, but are freed
 *  before an external program is executed. This helps to reduce the
 *  "bug" reports that if FreeCom is started from floppy the answer
 *  time in case of error is too long.
 * sub: feature LOAD MESSAGES
 * fix: if open(STRINGS.DAT) succeeds, but the ID check fails, the
 *  return value is not NULL
 * sub: error messages, why the COMMAND.COM/STRINGS.DAT is wrong
 *  this error messages are useful while debugging, but end-user should
 *  something like "%s does not point to FreeCom, please enter correct
 *  path to FreeCom or insert correct media and press ENTER".
 * add: Invalid message IDs or if the message file is absent, a default
 *  message is generated, "Error #<ID>\n". Will work for most of the
 *  current uses
 */

//#define READ_FROM_FILE
//#undef DEBUG

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <alloc.h>
#include <dir.h>
#include <dos.h>
#include <io.h>

#include "config.h"
#include "command.h"
#include "strings.h"
#include "suppl.h"

/* Default message settings:
  PATTERN: how the string is constructed for the ID (with \n)
  OUTOFMEMORY: string to issue on out-of-memory condition (no \n)
*/
#define DFL_MSG_PATTERN "Error #%u\n"
#define DFL_MSG_OUTOFMEMORY "Out of memory!"
#define ERR_ID_OUTOFMEMORY TEXT_ERROR_OUT_OF_MEMORY

struct indextype
{
  unsigned index;
  unsigned size;
};

static unsigned msgSegm = 0;    /* strings segment if loaded */

void unloadMsgs(void)  /* called at exit */
{
  if (msgSegm) {
    freeBlk(msgSegm);
    dprintf( ("[Message segment deallocated.]\n") );
  }
  msgSegm = 0;
}

static FILE *openStrFile_(void)
{
  FILE *f;
  char fdid[sizeof(STRINGS_ID)];
  char *theid = STRINGS_ID;
  char *thisstr;
#ifdef DEBUG
  char *fnam;
#define ASSIGN_FNAM fnam =
#else
#define ASSIGN_FNAM
#endif

#ifndef READ_FROM_FILE
  unsigned offs;
  unsigned segs;

  dprintf( ("[Try to read the strings from COMMAND.COM]\n") );

  if ((f = fopen(ASSIGN_FNAM comFile(), "rb")) == NULL)
  {
#ifdef DEBUG
    if (fddebug) perror("Can not open COMMAND.COM to read strings");
#endif
    goto tryFile;
  }

  dprintf(("[File '%s' opened]\n", fnam));

  fread(fdid, 2, 1, f);
  if (fdid[0] != 'M' || fdid[1] != 'Z')
  {
    dprintf(("[File %s is not valid COMMAND.COM]\n", fnam));
    fclose(f);
    goto tryFile;
  }

  fread(&offs, sizeof(unsigned), 1, f);
  fread(&segs, sizeof(unsigned), 1, f);

  dprintf(("[segs = %u  offs = %u]\n",segs,offs));

  if (offs != 0)
    segs--;

  fseek(f, (long)segs * 512 + offs, SEEK_SET);

  dbg_printmem();
  fread(fdid, strlen(theid), 1, f);

  if (strncmp(fdid, theid, strlen(STRINGS_ID)) == 0)
    return f;

  dprintf(("[Strings ID not valid!]\n"));
  fclose(f);
#endif

tryFile:
  dprintf( ("[Try to read the strings from STRINGS.DAT]\n") );
  if((thisstr = comPathFile("STRINGS.DAT")) == NULL)
    return NULL;

  if ((f = fopen(thisstr, "rb")) == NULL)
  {
#ifdef DEBUG
    if (fddebug) perror("Can not access strings.dat");
#endif
    free(thisstr);
    return NULL;
  }
  free(thisstr);

  fread(fdid, strlen(theid), 1, f);

  if (strncmp(fdid, theid, strlen(STRINGS_ID)))
  {
    dprintf(("[Strings ID not valid!]"));
    fclose(f);
    return NULL;
  }

  return f;
}

/*
 *  If the messages are NOT loaded and if there is an error during
 *  the load, the automatic load process might hang here infinitely.
 */
static FILE *openStrFile(void)
{
  static int recurs = 0;
  FILE *f;

  f = (recurs++)? NULL: openStrFile_();
  --recurs;

  return f;
}

unsigned msgSegment(void)              /* load messages into memory */
{
  FILE *f;
  size_t len;
  struct REGPACK r;

  if(msgSegm)     /* already loaded */
    return msgSegm;

  if ((f = openStrFile()) == NULL)
    return 0;

  /* At this point f is positioned at the very first string index
     the data area is NUMBER_OF_STRINGS * sizeof(index) +
     SIZE_OF_STRINGS   */
  len = SIZE_OF_STRINGS + NUMBER_OF_STRINGS * sizeof(struct indextype);
  /* allocation mode: last fit, high first */
  if ((msgSegm = allocBlk(len, 0x82)) == 0)
  {
    fclose(f);
    return 0;
  }

  /* synchronize FILE* with file descriptor */
  lseek(fileno(f), ftell(f), SEEK_SET);
  r.r_ax = 0x3f00;              /* read from file descriptor */
  r.r_bx = fileno(f);           /* file descriptor */
  r.r_cx = len;                 /* size of block to read */
  r.r_ds = msgSegm;             /* segment of buffer to read block to */
  r.r_dx = 0;                   /* offset of buffer to read block to */
  intr(0x21, &r);               /* perform DOS API */
  if ((r.r_flags & 1) != 0      /* read failed */
      || r.r_ax != len)       /* couldn't read everything */
  {
    unloadMsgs();
    fclose(f);
    return 0;
  }

  dprintf(("[Messages successfully loaded to segment 0x%04x]\n", msgSegm));

  /* strings read successfully */
  fclose(f);
  return msgSegm;
}

char *defaultMessage(unsigned id)
{
  char *h;

  /* Allocate 5 bytes for the numerical ID itself */
  if(id != ERR_ID_OUTOFMEMORY
   && (h = malloc(sizeof(DFL_MSG_PATTERN) + 5)) != NULL) {
    /* Got enough memory to allocate the stuff */
    sprintf(h, DFL_MSG_PATTERN, id);
    return h;
  }

  puts(DFL_MSG_OUTOFMEMORY);
  return NULL;
}

char *fetchString(unsigned id)
{
  char *thisstr;
  unsigned segm;
    struct indextype far *idx;

  if (id >= NUMBER_OF_STRINGS
   || (segm = msgSegment()) == 0)
    return defaultMessage(id);

  /* pointer to id's control data */
    idx = MK_FP(msgSegm, id * sizeof(*idx));
    if ((thisstr = malloc(idx->size)) == NULL)
      return defaultMessage(ERR_ID_OUTOFMEMORY);

    _fmemcpy(thisstr
     , MK_FP(segm, NUMBER_OF_STRINGS * sizeof(*idx) + idx->index)
     , idx->size);

    return thisstr;
}

/*
 * As fetchString(), but removes the last character.
 *
 *  This is necessary, because there is always a newline character
 *  at the end of the last line.
 */
char *getMessage(unsigned id)
{
  char *h;

  if ((h = fetchString(id)) != NULL && *h)
    strchr(h, '\0')[-1] = '\0';
  return h;
}

void displayString(unsigned id,...)
{
  char *thisstr;
  va_list argptr;

  if ((thisstr = fetchString(id)) == NULL)
    return;

  va_start(argptr, id);
  vprintf(thisstr, argptr);
  va_end(argptr);
  free(thisstr);
}
