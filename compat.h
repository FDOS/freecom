/*
   COMPAT.H: EXEC function with memory swap - 
             Borland/Microsoft compatibility header file.

   Public domain software by

        Thomas Wagner
        Ferrari electronic GmbH
        Beusselstrasse 27
        D-1000 Berlin 21
        Germany
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>

#if defined(__TURBOC__)
#define TURBO  1
#define MSC    0
#else
#define TURBO  0
#define MSC    1
#endif

#if (TURBO)
#include <dir.h>
#include <alloc.h>
#include <sys\stat.h>

#define OS_MAJOR  (_version & 0xff)

#endif

#if (MSC)
#include <malloc.h>
#include <direct.h>
#include <sys\types.h>
#include <sys\stat.h>

#define fnsplit   _splitpath
#define fnmerge   _makepath

#define MAXPATH   _MAX_PATH
#define MAXDRIVE  _MAX_DRIVE
#define MAXDIR    _MAX_DIR
#define MAXFILE   _MAX_FNAME
#define MAXEXT    _MAX_EXT

#define FA_RDONLY _A_RDONLY
#define FA_HIDDEN _A_HIDDEN
#define FA_SYSTEM _A_SYSTEM
#define FA_ARCH   _A_ARCH

#define findfirst(a,b,c) _dos_findfirst(a,c,b)
#define ffblk find_t

#define OS_MAJOR  _osmajor

#define farmalloc(x)    ((void far *)halloc(x,1))
#define farfree(x)      hfree((void huge *)x)

#define stpcpy(d,s)     (strcpy (d, s), d + strlen (s))

#define MK_FP(seg,ofs)  ((void far *)(((unsigned long)(seg) << 16) | (unsigned short)(ofs)))
#endif

