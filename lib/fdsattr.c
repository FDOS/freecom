/*	$id$
	$Locker$	$Name$	$State$

 *  Set attributes of an opened handle
 *
 *  Return:
 *    0: on succes
 *    else: DOS error code

	$Log$
	Revision 1.1  2002/04/08 16:02:22  skaus
	bugfix: CTTY: added to try to set the device flags

 */

#include <dos.h>

int fdsetattr(const int fd, const int attr)
{
  union REGS r;

  r.x.ax = 0x4400;              /* Get handle information */
  r.x.bx = fd;
  r.x.dx = attr;
  int86(0x21, &r, &r);
  return r.x.cflag         /* call failed */
  ? r.x.ax          /* error code */
  : 0;            /* OK */
}
