/*
 *      Prototypes for the CP/M-style device name-aware open functions
 */
#ifndef __OPENF_H
#define __OPENF_H

#include <stdio.h>

/*
 *      Check if the passed file descriptor is connected to a
 *      character device rather than a block device
 *
 *      Return:
 *              0: block device (disk file)
 *              else: character device
 */
int isadev(const int fd);

/*
 *	Return the attributes of an opened handle; 0 on error
 */
int fdattr(const int fd);
int fdsetattr(const int fd, const int attr);

/*
 *      Check for a CP/M-style device name
 *
 *      Return:
 *              0: no, fnam[] unchanged
 *              else: yes, fnam[] is modifed to contain a valid DOS filename
 */
int isDeviceName(char * const fnam);

/*
 *      IO-style CP/M aware open() function
 *
 *      If it is a device name, fnam[] is modified to contain a valid
 *      DOS filename.
 */
#ifndef OPENF_NO_PROTOTYPE
int devopen(char * const fnam, int mode, ... /* int oflags */);
#else
int devopen(char * const fnam, int mode, int oflags);
#endif

/*
 *      FIO-style CP/M aware fopen() function
 *
 *      If it is a device name, fnam[] is modified to contain a valid
 *      DOS filename.
 */
FILE *fdevopen(char * const fnam, const char * const mode);

#endif
