
/*
 * ERR_HAND.H -- Header for Critical Error Handling
 * Rob Lake
 * 07/04/98
 *
 */

#if !defined(__ERR_HAND_H)
#define __ERR_HAND_H

#include "command.h"
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <ctype.h>

#define CHAR_DEV  128
#define BUFSIZE   512
#define FAIL_FLG        8
#define RETRY_FLG       16
#define IGNORE_FLG      32

#define FAIL            3
#define ABORT   2
#define RETRY           1
#define IGNORE    0

#define WRITE_PROTECT_ERROR     0
#define UNKNOWN_UNIT            1
#define DRIVE_NOT_READY         2
#define UNKNOWN_COMMAND         3
#define CRC_ERROR               4
#define BAD_LENGTH              5
#define SEEK_ERROR              6
#define UNKNOWN_MEDIA           7
#define SECTOR_NOT_FOUND        8
#define OUT_OF_PAPER            9
#define WRITE_FAULT             10
#define READ_FAULT              11
#define GENERAL_FAILURE         12

/* Device Driver Header */
typedef struct _dev_hdr
{
  struct _dev_hdr *dev_next;    /* Ptr to next DDH   */
  unsigned dev_attrib;          /* Device Attribute  */
  void (*dev_strategy) ();      /* Strategy Routine  */
  void (*dev_interrupt) ();     /* Interrupt Routine */
  char dev_name[8];
}
dev_hdr;

extern int autofail;            /* != 0 --> any critical ‘rrors are " Failed" */

#endif                          /* __ERR_HAND_H */
