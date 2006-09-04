/* 
 * Header file for the long filename-related functions
 */

#ifndef __LFNFUNCS_H
#define __LFNFUNCS_H

#ifdef __TURBOC__ /* OpenWatcom has an LFN CLIB Extension available */

#ifdef FEATURE_LONG_FILENAMES

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
        char ff_name[256];      /* null-terminated filename   */
        unsigned char  lfnsup;  /* DOS LFN support status     */
};

#ifdef __LFNFUNCS_C

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

#endif

const char * getshortfilename( const char *longfilename );
int          lfn_chmod( const char *filename, int func, ... );
int          lfn_creat( const char *filename, int mode );
int          lfncreat( const char *filename, int amode );
int          lfncreatnew( const char *filename, int mode );
FILE *       lfnfopen( const char *filename, const char *mode );
int          lfnopen( const char *filename, int access, ... );
int          lfn_mrc_dir( const char *name, int function );
int          lfnrename( const char *oldfilename, const char *newfilename );
int          lfnfindfirst( const char *path, struct lfnffblk *buf,
                           unsigned attr );
int          lfnfindnext( struct lfnffblk *buf );
int          lfnfindclose( struct lfnffblk *buf );

extern unsigned char __supportlfns;
extern unsigned char lfncomplete;

#ifndef __LFNFUNCS_C

#undef remove
#define access( x, y )    access( getshortfilename( x ), y )
#define chmod( x, y )     chmod( getshortfilename( x ), y )
#define creattemp( x, y ) creattemp( getshortfilename( x ), y )
#define dfnstat( x )      dfnstat( getshortfilename( x ) )
#define _open( x, y )     _open( getshortfilename( x ), y )
#define unlink( x )       unlink( getshortfilename( x ) )
#define _chmod            lfn_chmod
#define _creat            lfn_creat
#define creat             lfncreat
#define creatnew          lfncreatnew
#define fopen             lfnfopen
#define remove            unlink
#define open              lfnopen
#define rename            lfnrename
#define stat( x, y )      stat( getshortfilename( x ), y )
#define findfirst         lfnfindfirst
#define findnext          lfnfindnext
#define findclose         lfnfindclose
#define ffblk             lfnffblk

#undef MAXFILE
#undef MAXDIR
#undef MAXPATH
#undef MAXEXT
#define MAXFILE 255
#define MAXDIR  255
#define MAXEXT  255
#define MAXPATH 260

#endif

#endif

#endif

#endif
