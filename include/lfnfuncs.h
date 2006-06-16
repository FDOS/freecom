/* 
 * Header file for the long filename-related functions
 */

#ifndef __LFNFUNCS_H
#define __LFNFUNCS_H

#ifdef __TURBOC__ /* OpenWatcom has an LFN CLIB Extension available */

struct lfnffblk {
        unsigned short cr_time; /* time of file creation      */
        unsigned short cr_date; /* date of file creation      */
        unsigned short ac_time; /* time of last file access   */
        unsigned short ac_date; /* date of last file access   */
        unsigned short lfnax;   /* DOS LFN search handle      */
        unsigned char  lfnsup;  /* DOS LFN support status     */
        char ff_reserved[10];   /* reserved for use by DOS    */
        char ff_attrib;         /* attribute byte for file    */
        unsigned short ff_ftime;/* time of last write to file */
        unsigned short ff_fdate;/* date of last write to file */
        unsigned long  ff_fsize;/* length of file in bytes    */
        char ff_name[256];      /* null-terminated filename   */
};

struct locffblk {
    int             attributes;
    int             creattime;
    int             creatdate;
    int             accesstime;
    int             accessdate;
    int             wrtime;
    int             wrdate;
    unsigned long   filesize;
    char            reserved[8];
    char            lfn[260];
    char            sfn[14];
};

char *  getshortfilename( const char *longfilename );
int     lfn_chmod( const char *filename, int func, ... );
int     lfnchmod( const char *filename, int mode );
int     lfncreat( const char *filename, int amode );
int     lfn_creat( const char *filename, int mode );
int     lfncreatnew( const char *filename, int mode );
int     lfncreattemp( const char *filename, int attrib );
int     lfnaccess( const char *filename, int mode );
char *  lfn_getdcwd( int drive, char *buf, int size );
FILE *  lfnfopen( const char *filename, const char *mode );
int     lfnopen( const char *filename, int access, ... );
int     lfn_open( const char *filename, int omode );
int     lfnremove( const char *filename );
int     lfnrename( const char *oldfilename, const char *newfilename );
int     lfnstat( const char *filename, struct stat * statbuf );
int     lfnfindfirst( const char *path, struct lfnffblk *buf, unsigned attr );
int     lfnfindnext( struct lfnffblk *buf );
int     lfnfindclose( struct lfnffblk *buf );

#ifndef __LFNFUNCS_C

#define _chmod    lfn_chmod
#define chmod     lfnchmod
#define creat     lfncreat
#define _creat    lfn_creat
#define creatnew  lfncreatnew
#define creattemp lfncreattemp
#define access    lfnaccess
#define _getdcwd  lfn_getdcwd
#define fopen     lfnfopen
#define open      lfnopen
#define _open     lfn_open
#define remove    lfnremove
#define rename    lfnrename
#define stat      lfnstat
#define findfirst lfnfindfirst
#define findnext  lfnfindnext
#define findclose lfnfindclose
#define ffblk     lfnffblk

#endif

#endif

#endif
