/*  errno.h

    Defines the system error variable errno and the error
    numbers set by system calls. Errors which exist in Unix(tm)
    but not MSDOS have value -1.

    Copyright (c) 1987, 1991 by Borland International
    All Rights Reserved.
*/

#ifndef __ERRNO_H
#define __ERRNO_H

#if !defined( __DEFS_H )
#include <_defs.h>
#endif

/*  Dos Error Codes */

#define EZERO    0      /* Error 0                  */
#define EINVFNC  1      /* Invalid function number  */
#define ENOFILE  2      /* File not found           */
#define ENOPATH  3      /* Path not found           */
#define ECONTR   7      /* Memory blocks destroyed  */
#define EINVMEM  9      /* Invalid memory block address */
#define EINVENV 10      /* Invalid environment      */
#define EINVFMT 11      /* Invalid format           */
#define EINVACC 12      /* Invalid access code      */
#define EINVDAT 13      /* Invalid data             */
#define EINVDRV 15      /* Invalid drive specified  */
#define ECURDIR 16      /* Attempt to remove CurDir */
#define ENOTSAM 17      /* Not same device          */
#define ENMFILE 18      /* No more files            */

#define ENOENT   2      /* No such file or directory*/
#define EMFILE   4      /* Too many open files      */
#define EACCES   5      /* Permission denied        */
#define EBADF    6      /* Bad file number          */
#define ENOMEM   8      /* Not enough core          */
#define ENODEV  15      /* No such device           */
#define EINVAL  19      /* Invalid argument         */
#define E2BIG   20      /* Arg list too long        */
#define ENOEXEC 21      /* Exec format error        */
#define EXDEV   22      /* Cross-device link        */
#define EDOM    33      /* Math argument            */
#define ERANGE  34      /* Result too large         */
#define EEXIST  35      /* File already exists      */
#define EDEADLOCK 36    /* Locking violation        */
#define ECHILD  -1      /* Unix/DOS                 */
#define EFAULT  -1      /* Unknown error            */
#define EPERM   -1      /* UNIX - not MSDOS         */
#define ESRCH   -1      /* UNIX - not MSDOS         */
#define EINTR   -1      /* UNIX - not MSDOS         */
#define EIO     -1      /* UNIX - not MSDOS         */
#define ENXIO   -1      /* UNIX - not MSDOS         */
#define EAGAIN  -1      /* UNIX - not MSDOS         */
#define ENOTBLK -1      /* UNIX - not MSDOS         */
#define EBUSY   -1      /* UNIX - not MSDOS         */
#define ENOTDIR -1      /* UNIX - not MSDOS         */
#define EISDIR  -1      /* UNIX - not MSDOS         */
#define ENFILE  -1      /* UNIX - not MSDOS         */
#define ENOTTY  -1      /* UNIX - not MSDOS         */
#define ETXTBSY -1      /* UNIX - not MSDOS         */
#define EFBIG   -1      /* UNIX - not MSDOS         */
#define ENOSPC  -1      /* UNIX - not MSDOS         */
#define ESPIPE  -1      /* UNIX - not MSDOS         */
#define EROFS   -1      /* UNIX - not MSDOS         */
#define EMLINK  -1      /* UNIX - not MSDOS         */
#define EPIPE   -1      /* UNIX - not MSDOS         */
#define EUCLEAN -1      /* UNIX - not MSDOS         */

extern  int _Cdecl  errno;
extern  int _Cdecl  _doserrno;

#if !__STDC__
#define _sys_nerr   35      /* highest defined system error number */
#endif

#endif
