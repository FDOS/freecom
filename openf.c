/*
 *      To handle CP/M style devices, like "CON:" and "LPT1:", which
 *      are not understood by the DOS kernel directly, these functions
 *      wrap the open() and fopen() functions and deal with it.
 *
 *    Also included are some low-level functions dealing with
 *    file descriptors, such as "isadev()".
 *
 * 1999/01/24 ska
 *      started
 *
 * 2000/01/13 ska
 * add: fdattr()
 */

#include <assert.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <string.h>

#define OPENF_NO_PROTOTYPE
#include "openf.h"

/*
 *      Just for the algorithm:
 *              One could just transform the "XXXX:" style into
 *              "\dev\XXXX". However, WinNT DOSbox does not support
 *              this as it seems ("copy \dev\con test" says path not found).
 *
 *              On the other hand, how many other DOS clones don't support
 *              the \DEV\ prefix, too?
 *
 *      So I open the device and check the handle it it's connected to
 *      a character device rather than a block device.
 */

/*
 *  Return attributes of an opened handle
 *
 *  Return:
 *    0: on failure (means: modified regular file on fixed drv A:)
 *    else: attributes
 */
int fdattr(const int fd)
{
  union REGS r;

  r.x.ax = 0x4400;              /* Get handle information */
  r.x.bx = fd;
  int86(0x21, &r, &r);
  return r.x.cflag == 0         /* call OK */
  ? r.x.dx          /* attributes */
  : 0;            /* error */
}
/*
 *      Check if the passed file descriptor is connected to a
 *      character device rather than a block device
 *
 *      Return:
 *              0: block device (disk file)
 *              else: character device
 */
int isadev(const int fd)
{
  return (fdattr(fd) & 0x80) != 0; /* is character device */
}

/*
 *      Check if the passed filename is a CP/M style device
 *
 *      These device names must not contain any drive or path or
 *      extension component, but have a colon ':' at their end.
 *
 *      Return:
 *              0: no
 *              else: yes; fnam contains a valid filename now
 */
int isDeviceName(char *const fnam)
{
  char *p;

  assert(fnam);

  if ((p = strpbrk(fnam, ":\\/")) != 0 /* has a colon or path component */
      && *p == ':'              /* has colon --> no path component */
      && p[1] == '\0'           /* at end of string */
      && p >= fnam + 2)
  {                             /* no simple drive letter */
    int fd;

    *p = '\0';                  /* strip off the colon to make it valid for DOS */

    /* open with read access only to prevent from creating
       disk fiels accidently */
    if ((fd = open(fnam, O_RDONLY)) >= 0)
    {                           /* open OK */
      int rv;

      /* check for character device */
      rv = isadev(fd);
      close(fd);
      if (rv)
        return 1;               /* is device name */
    }
    *p = ':';                   /* restore the original filename */
  }

  return 0;                     /* no device name */
}

int devopen(char *const fnam, int mode, int omode)
{
  isDeviceName(fnam);           /* modify fnam if device */
  return open(fnam, mode, omode);
}

FILE *fdevopen(char *const fnam, const char *const mode)
{
  isDeviceName(fnam);           /* modify fnam if device */
  return fopen(fnam, mode);
}
