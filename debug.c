/*
 *  Defines the functions only necessary while debugging is active
 */

#include "config.h"

#ifdef DEBUG

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <alloc.h>

#include "command.h"

void dbg_printmem(void)
{ static unsigned nearLast = 0;
  static unsigned long farLast = 0;

  unsigned nearThis;
  unsigned long farThis;

  switch(heapcheck()) {
  case _HEAPCORRUPT:
    cputs("HEAP CORRUPTED. Cannot proceed!\r\n");
    abort();
  case _HEAPEMPTY:
    cputs("NO HEAP. Cannot proceed!\r\n");
    abort();
  default:
    cputs("Unknown heapcheck() error. Cannot proceed!\r\n");
    abort();
  case _HEAPOK:
    break;
  }

  nearThis = coreleft();
  farThis = farcoreleft();

  dprintf(("[free memory: near=%6u far=%13lu]\n", nearThis, farThis));
  if(nearLast)
    dprintf(("[changed    : near=%6d far=%13ld]\n"
     , nearThis - nearLast , farThis - farLast));

  nearLast = nearThis;
  farLast = farThis;
}

#endif    /* defined(DEBUG) */
