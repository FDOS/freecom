/* $Id$
 *  CTTY.C - ctty command.
 *
 *  Comments:
 *
 *  Current possible problems: -- 2000/01/14 ska
 *
 *  Problem: Only the first three handles are changed to the passed device.
 *  -->
 *  As far as I know to CTTY (Change TTY) it is not sufficient just to
 *  change the handles 0-2 (stdin, stdout, stderr), but one must inform
 *  the system that another driver is the default STDIN/OUT device,
 *  what would include to:
 *  + return ienabled STDIN/STDOUT bits when the device attributes are
 *    retreived via IOCTL,
 *  + change the pointer at LoL+0C.
 *  My original implication that INT-29 is effected, too, is wrong in
 *  the Win98 DOSbox.
 *
 *  FreeDOS kernel 2017f passes the DOS1-compatible IO functions to
 *  the handle based ones, so maybe most simply programs will work?!
 *
 * 14 Aug 1998 (John P Price)
 * - Created dummy command.
 *
 * 2000/01/14 ska
 * + Added to change the first three handles to the given device name
 * + Supports only redirection of stdin and stdout, e.g.:
 *    C:\> CTTY COM1 >file
 *  -or-
 *    C:\> echo Hallo | CTTY COM1 | echo du
 *  The CTTY command effects the commands on the _next_ line.
 */

#include "../config.h"

#include <fcntl.h>
#ifdef DEBUG
#include <dos.h>
#endif
#include <io.h>
#include <stdio.h>

#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/openf.h"
#include "../strings.h"

#ifdef DEBUG
static void devAttr(int fd)
{ int attr;

  if((attr = fdattr(fd)) == 0)
	{
    dprintf(("[Failed to read attributes of fd #%d]\n", fd));
	}
  else {
    dbg_print("[fd #%d is", fd);
    if(attr & 0x80) {
      if(attr & 7) {
        if(attr & 1) dbg_outs(" STDIN");
        if(attr & 2) dbg_outs(" STDOUT");
        if(attr & 4) dbg_outs(" NUL");
      } else dbg_outs(" a device");
    } else dbg_outs(" a file");
    dbg_outsn("]");
  }
}
#endif

int cmd_ctty(char *param)
{ int f, attr, failed;

  if(!param || !*param) {
    displayString(TEXT_CMDHELP_CTTY);
    return 1;
  }

#ifdef DEBUG
  devAttr(0);
  devAttr(1);
  devAttr(2);
#endif

  if((f = devopen(param, O_RDWR)) < 0) {
    error_no_rw_device(param);
    return 1;
  }

  if(((attr = fdattr(f)) & 0x80) == 0
   || write(f, "\r\n", 2) != 2) {		/* need \r as in bin mode */
    close(f);
    error_no_rw_device(param);
    return 1;
  }

	/* Now inform the system that this device is the new console
		now; STDIN and STDOUT flag are the most lowest two bits  */
	fdsetattr(f, (attr & 0xFF) | 0x03);

  fflush(stdin);      /* prepare asynchroneous changes of internal fd's */
  fflush(stdout);
  fflush(stderr);

  /* Now change the file descriptors:
    0 := rdonly
    1,2 := wronly

    if CTTY is called within a pipe or its I/O is redirected,
    oldinfd or oldoutfd is not equal to -1. In such case the
    old*fd is modified in order to effect the file descriptor
    after the redirections are restored. Otherwise a pipe or
    redirection would left CTTY in a half-made status.
  */
  failed = dup2(f, 2);   /* no redirection support */
  if(oldinfd != -1)
  	close(oldinfd);
  oldinfd = f;
  if(oldoutfd != -1)
  	close(oldoutfd);
  if((oldoutfd = dup(f)) == -1)
  	failed = 1;

  if(failed)
    error_ctty_dup(param);

  return failed;
}
