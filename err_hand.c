/*
 * ERR_HAND.C -- Critical Error Handling, INT24 Function
 *
 * Comments:
 *
 * 07/03/98 (Rob Lake)
 *      Started
 *
 * 07/13/98 (Rob Lake)
 *  Added critical error handling counting.  Now only 5 calls to error
 *  handler can occur, unless user aborts or fails, without auto-failing.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 12-Aug-1998 ska
 * - added autofail (/F command line option)
 *
 * 1999/03/17 ska
 * bugfix: Print garbage on screen, if no disk in floppy drive
 *  An "interrupt" function saves & restores all registers and
 *  also sets up DS correctly.
 */

#include "config.h"

#include <dos.h>

#include "err_hand.h"

static unsigned calls = 0;
int autofail = 0;               /* just let everything FAIL */

void init_error_handler(void)
{
#ifdef __TINY__
  _DX = FP_OFF(dos_critical_error);
  _AX = 0x2524;
  geninterrupt(0x21);
#else
  setvect(0x24, dos_critical_error);
#endif
}


/*
 * printstring -- uses DOS call (09h) to print a '$' terminated
 * string
 */
static void printstring(char *string)
{
  asm {
    push ds
  }
  _DS = FP_SEG(string);
  _DX = FP_OFF(string);
  asm {
    mov ah, 09h
    int 21h
    pop ds
  }
}

/*
 *
 * get_err -- returns the text to the actual error message
 *
 */
static char *get_err(unsigned err_code)
{
  switch (err_code)
  {
    case WRITE_PROTECT_ERROR:
      return "Write Protect Error";
    case UNKNOWN_UNIT:
      return "Unknown Unit";
    case DRIVE_NOT_READY:
      return "Drive Not Ready";
    case UNKNOWN_COMMAND:
      return "Unknown Command";
    case CRC_ERROR:
      return "CRC Error";
    case BAD_LENGTH:
      return "Bad Length";
    case SEEK_ERROR:
      return "Seek Error";
    case UNKNOWN_MEDIA:
      return "Unknown Media Type";
    case SECTOR_NOT_FOUND:
      return "Sector Not Found";
    case OUT_OF_PAPER:
      return "Out of Paper";
    case WRITE_FAULT:
      return "Write Fault";
    case READ_FAULT:
      return "Read Fault";
    case GENERAL_FAILURE:
    default:
      return "General Failure";
  }
}

/*
 *
 * dos_critical_error -- The actual handler
 *
 */
#pragma argsused
void interrupt far dos_critical_error(unsigned reg_Bp, unsigned reg_Di,
         unsigned reg_Si, unsigned reg_Ds, unsigned reg_Es, unsigned reg_Dx,
         unsigned reg_Cx, unsigned reg_Bx, unsigned reg_Ax)
{
  unsigned err_flags;
  unsigned drive;
  unsigned err_code;
  dev_hdr far *dev;
  char *err_msg;
  char device[8];
  char buf[BUFSIZE];
  char action;

  err_flags = reg_Ax >> 8;      /* make it AH */
  drive = reg_Ax & 0xFF;        /* make it AL */
/* Clear AL for return */
  reg_Ax &= 0xFF00;

  if(autofail) {
    reg_Ax |= FAIL;
    return;
  }

  if (++calls == 5)
  {
    calls = 0;
    printstring("Fail on INT24\n\r$");
    reg_Ax |= FAIL;
    return;
  }

  err_code = reg_Di & 0xFF;     /* error code in lower byte of DI */
  dev = (dev_hdr far *) MK_FP(reg_Bp, reg_Si);
  err_msg = get_err(err_code);
retry:
  if (err_flags & CHAR_DEV)
  {
    int i;
    for (i = 0; i < 8; i++)
    {
      if (dev->dev_name[i] == ' ')
        break;
      device[i] = dev->dev_name[i];
    }
    device[i] = '\0';
    sprintf(buf, "\n\r\n%s error on %s\n\r$", err_msg, device);
    printstring(buf);
  }
  else
  {
    sprintf(buf, "\n\r\n%s error on drive %c\n\r$", err_msg,
            drive + 'A');
    printstring(buf);
  }

/* Print appropriate message */
  printstring("Abort$");
  if (err_flags & RETRY_FLG)
  {
    if ((err_flags & (IGNORE_FLG | FAIL_FLG)) == 0)
      printstring(", or $");
    else
      printstring(", $");
    printstring("Retry$");
  }
  if (err_flags & FAIL_FLG)
  {
    if ((err_flags & IGNORE_FLG) == 0)
      printstring(", or $");
    else
      printstring(", $");
    printstring("Fail$");
  }
  if (err_flags & IGNORE_FLG)
    printstring(", or Ignore$");
  printstring("?$");

  action = getche();
  printstring("\n\r$");
  switch (toupper(action))
  {
    case 'A':
      reg_Ax |= ABORT;
      calls = 0;
      return;
    case 'R':
      if ((err_flags & RETRY_FLG) == 0)
      {
        if ((err_flags & FAIL_FLG) == 0)
        {
          reg_Ax |= ABORT;
          calls = 0;
        }
        else
        {
          reg_Ax |= FAIL;
          calls = 0;
        }
      }
      else
        reg_Ax = RETRY;
      return;
    case 'F':
      if ((err_flags & FAIL_FLG) == 0)
        reg_Ax |= ABORT;
      else
        reg_Ax |= FAIL;
      calls = 0;
      return;
    case 'I':
      if ((err_flags & IGNORE_FLG) == 0)
      {
        if ((err_flags & FAIL_FLG) == 0)
        {
          reg_Ax |= ABORT;
          calls = 0;
        }
        else
        {
          reg_Ax |= FAIL;
          calls = 0;
        }
      }
      else
        reg_Ax |= IGNORE;
      return;
    default:
      goto retry;
  }
}
