/* 
 * Header file for the long filename-related functions
 */

#ifndef H__LFNFUNCS_
#define H__LFNFUNCS_

/*#ifdef __TURBOC__*/ /* OpenWatcom has an LFN CLIB Extension available 
			 but not used yet */

#ifdef FEATURE_LONG_FILENAMES

#include <stdio.h>

#include <algnbyte.h>
struct lfnffblk {
        unsigned short cr_time; /* time of file creation      */
        unsigned short cr_date; /* date of file creation      */
        unsigned short ac_time; /* time of last file access   */
        unsigned short ac_date; /* date of last file access   */
        unsigned short lfnax;   /* DOS LFN search handle      */
        char ff_reserved[11];   /* reserved for use by DOS    */
        char ff_attrib;         /* attribute byte for file    */
        unsigned short ff_ftime;/* time of last write to file */
        unsigned short ff_fdate;/* date of last write to file */
        unsigned long  ff_fsize;/* length of file in bytes    */
        char ff_name[260];      /* null-terminated filename   */
        unsigned char  lfnsup;  /* DOS LFN support status     */
};
#include <algndflt.h>

#ifdef __LFNFUNCS_C

#include <algnbyte.h>
struct locffblk {
    long attributes;
    long creattime;
    long creatdate;
    long accesstime;
    long accessdate;
    long wrtime;
    long wrdate;
    long hifilesize;
    long filesize;
    char reserved[8];
    char lfn[260];
    char sfn[14];
};
#include <algndflt.h>

#endif /* __LFNFUNCS_C */

const char * getshortfilename( const char *longfilename );
int          lfn_chmod( const char *filename, int func, ... );
int          lfn_creat( const char *filename, int mode );
FILE *       lfnfopen( const char *filename, const char *mode );
int          lfnopen( const char *filename, int access, ... );
int          lfnrename( const char *oldfilename, const char *newfilename );
int          lfnfindfirst( const char *path, struct lfnffblk *buf,
                           unsigned attr );
int          lfnfindnext( struct lfnffblk *buf );
int          lfnfindclose( struct lfnffblk *buf );
int          lfnmkdir( const char *path );
int          lfnrmdir( const char *path );
int          lfnchdir( const char *path );

extern unsigned char __supportlfns;
extern unsigned char lfncomplete;

#ifndef __LFNFUNCS_C

#undef remove
#define access( x, y )    access( getshortfilename( x ), y )
#define chmod( x, y )     chmod( getshortfilename( x ), y )
#define creattemp( x, y ) creattemp( getshortfilename( x ), y )
#define dfnstat( x )      dfnstat( getshortfilename( x ) )
#undef dos_open
#define dos_open( x, y )  sfn_open( getshortfilename( x ), y )
#define unlink( x )       unlink( getshortfilename( x ) )
#define _chmod            lfn_chmod
#undef dos_creat
#define dos_creat         lfn_creat
#define fopen             lfnfopen
#define remove            unlink
#define open              lfnopen
#define rename            lfnrename
#define stat( x, y )      stat( getshortfilename( x ), y )
#undef  dos_findfirst
#undef  dos_findnext
#undef  dos_findclose
#define dos_findfirst     lfnfindfirst
#define dos_findnext      lfnfindnext
#define dos_findclose     lfnfindclose
#define dos_ffblk         lfnffblk
#define mkdir             lfnmkdir
#define rmdir             lfnrmdir
#define chdir             lfnchdir

#undef MAXFILE
#undef MAXDIR
#undef MAXPATH
#undef MAXEXT
#define MAXFILE 255
#define MAXDIR  255
#define MAXEXT  255
#define MAXPATH 260

#endif /* __LFNFUNCS_C */

#else

#define dos_ffblk         ffblk

#endif /* FEATURE_LONG_FILENAMES */
/*#endif*/ /* __TURBOC__ */
#endif /* H__LFNFUNCS_ */
