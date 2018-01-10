/* io.h compatibility header */

#ifndef __IO_H
#define __IO_H

#include <unistd.h>
extern int _open(const char *pathname, int flags, ...);
extern int _creat(const char *pathname, mode_t mode);
extern int _read(int fd, void *buf, size_t cnt);
extern int _write(int fd, const void *buf, size_t cnt);
extern int _close(int fd);
extern long int _lseek(int fd, long int pos, int whence);

static inline long int filelength(int fd)
{
  long int cur = lseek(fd, 0, SEEK_CUR);
  long int fsize = lseek(fd, 0, SEEK_END);
  lseek(fd, cur, SEEK_SET);
  return fsize;
}
#endif
