/*	$Id: $

	delay function for ia16-elf-gcc with newlib
	uses _dos_gettime, only for delays < 1 minute (60000 ms)
 */

#include "../config.h"

#ifdef __GNUC__

#include <portable.h>
#include "../include/timefunc.h"

void delay(unsigned ms)
{
  struct dostime_t t;
  int diff, cnt = -1, v;

  while (1) {
    _dos_gettime(&t);
    v = 100 * t.second + t.hsecond;
    if (cnt == -1)
      cnt = v;
    diff = v - cnt;
    if (diff < 0) diff += 6000;
    if (diff >= ms/10) break;
    asm volatile ("int $0x28");
  }
}

#endif
