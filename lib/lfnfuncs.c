/* 
 * Gets the short filename equivalent 
 */

#include "../config.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <suppl.h>
#include <dfn.h>
#include <sstr.h>
#include <io.h>

#define __LFNFUNCS_C

#include "suppl.h"
#include "../include/misc.h"
#include "../include/lfnfuncs.h"


#ifdef FEATURE_LONG_FILENAMES

/*#ifdef __TURBOC__*/ /* OpenWatcom has an LFN CLIB Extension available 
			 but not used yet */

#define _STC     __emit__(0xF9)
#define _PUSH_DS __emit__(0x1E)
#define _PUSH_ES __emit__(0x06)
#define _POP_DS  __emit__(0x1F)
#define _POP_ES  __emit__(0x07)

/*
 * The following function copies a short filename equivalent of a long
 * filename into a local static buffer, which will be overwritten on
 * subsequent calls
 */
const char * getshortfilename( const char *longfilename )
{
    static char shortfilename[ 128 ];
    IREGS r;

    if (!__supportlfns) return( longfilename );

    r.r_ds = FP_SEG( longfilename );
    r.r_si = FP_OFF( longfilename );
    r.r_es = FP_SEG( shortfilename );
    r.r_di = FP_OFF( shortfilename );
    shortfilename[0] = '\0';
    r.r_cx = 0x8001; /* Get short filename */
    r.r_ax = 0x7160;/* LFN truename function */

    intrpt( 0x21, &r );

    return( ( ( r.r_flags & 1 ) || r.r_ax == 0x7100 || !shortfilename[0] ) ?
            longfilename : shortfilename );
}

/* creates or keeps/truncates the LFN in filename
   if the LFN already exists and code == 0x12, then truncate it
   if the LFN already exists and code == 0x10, then do nothing
   returns 1 if the LFN already exists or has been created, else 0
   LFN handles are avoided for compatibility with buggy LFN implementations,
   most notably NTVDM in Windows 2000 and XP.
*/
static int __creat_or_truncate( const char * filename, int mode, int code )
{
    int handle;
    IREGS r;

    if( !__supportlfns ) {
        return 0;
    }

    r.r_ds = FP_SEG( filename );
    r.r_si = FP_OFF( filename );
    r.r_bx = O_WRONLY;
    r.r_cx = mode;
    r.r_dx = code;
    r.r_ax = 0x716C;

    intrpt( 0x21, &r );

    if( ( r.r_flags & 1 ) || r.r_ax == 0x7100 )
        return r.r_ax == 0x50; /* file already exists for error 0x50 */
    handle = r.r_ax;
    /*
     * Win2k always returns handle == 2, which is a bug.
     * File handle 2 is already used for stderr
     * Fortunately, FreeCOM never actually expects to get file handle 2
     * ( as it does for '1' and '0', when redirecting stdin and stdout )
     */
    if( handle != 2 )dos_close( handle );
    return 1;
}

#if defined(DEBUG) || defined(FEATURE_CALL_LOGGING)
FILE * lfnfopen( const char *filename, const char *mode )
{
    /* only used with "at" in debugging code */
    if( strpbrk( mode, "aw" ) )
        __creat_or_truncate( filename, 0, 0x10 );
    return( fopen( getshortfilename( filename ), mode ) );
}
#endif

int lfn_creat( const char *filename, int attr )
{
    if ( __creat_or_truncate( filename, attr, 0x12 ) )
        return sfn_open( getshortfilename( filename ), O_WRONLY );
    return sfn_creat( filename, attr );
}

int lfnrename( const char *oldfilename, const char *newfilename )
{   /* Must use the actual interrupt for this */
    IREGS r;

    if(dfnstat(newfilename) != 0) {
        errno = EACCES;
        return( -1 );
    }

    r.r_ds = FP_SEG( oldfilename );
    r.r_dx = FP_OFF( oldfilename );
    r.r_es = FP_SEG( newfilename );
    r.r_di = FP_OFF( newfilename );
    r.r_ax = 0x7156;

    intrpt( 0x21, &r );

    if( ( r.r_flags & 1 ) || r.r_ax == 0x7100 ) {
        r.r_ax = 0x5600;

        intrpt( 0x21, &r );

        if( ( r.r_flags & 1 ) ) {
            errno = r.r_ax;
            return( -1 );
        }
    }

    return( 0 );
}

static void convert_to_ffblk( struct lfnffblk *dosblock,
                              struct locffblk *lfnblock )
{
    dosblock->ff_attrib = lfnblock->attributes;
    dosblock->cr_time   = lfnblock->creattime;
    dosblock->cr_date   = lfnblock->creatdate;
    dosblock->ac_time   = lfnblock->accesstime;
    dosblock->ac_date   = lfnblock->accessdate;
    dosblock->ff_ftime  = lfnblock->wrtime;
    dosblock->ff_fdate  = lfnblock->wrdate;
    dosblock->ff_fsize  = lfnblock->filesize;
    if( lfnblock->lfn ) {
        memcpy( dosblock->ff_name, lfnblock->lfn, sizeof( lfnblock->lfn ) );
    } else {
        memcpy( dosblock->ff_name, lfnblock->sfn, sizeof( lfnblock->sfn ) );
    }
}

int lfnfindfirst( const char *path, struct lfnffblk *buf, unsigned attr )
{
    struct locffblk lfnblock;
    IREGS r;

    buf->lfnax = buf->lfnsup = 0; /* Zero find handle and LFN-supported flag */

    if( !__supportlfns )
        return( sfnfindfirst( path, ( struct ffblk * )buf, attr ) );

    r.r_ds = FP_SEG( path );
    r.r_dx = FP_OFF( path );       /* path goes in DS:DX */
    r.r_es = FP_SEG( &lfnblock );
    r.r_di = FP_OFF( &lfnblock );  /* LFN find block goes in ES:DI */
    r.r_si = 1;                    /* Use DOS date/time format */
    r.r_cx = attr;
    r.r_ax = 0x714E;               /* LFN Findfirst */

    intrpt( 0x21, &r );

    /*
     * If ax = 7100, there is probably an LFN TSR but no LFN support for
     * whatever drive or directory is being searched. In that case, fall back on
     * the old findfirst.  Also if the function fails, it could be because of
     * no LFN TSR so fall back to the old findfirst.
     */
    if( ( r.r_flags & 1 ) || r.r_ax == 0x7100 )
        return( sfnfindfirst( path, ( struct ffblk * )buf, attr ) );

    /*
     * If there was no failure, the next step is to move the values from the
     * LFN block into the non-lfn block
     */
    buf->lfnax = r.r_ax;
    buf->lfnsup = 1;
    convert_to_ffblk( buf, &lfnblock );
    /*
     * 0 is always returned because we've already checked cflag; the
     * old_dos_findfirst will handle any errors
     */
    return( 0 );
}

int lfnfindnext( struct lfnffblk *buf )
{
    struct locffblk lfnblock;
    IREGS r;

    /*
     * Before going through the possibly unnecessary steps of calling the LFN
     * function first, buf->lfnsup will tell us if LFN was supported with the
     * previous call to findfirst.
     */
    if( !buf->lfnsup || !__supportlfns ) {
        return( sfnfindnext( ( struct ffblk * )buf ) );
    }

    r.r_es = FP_SEG( &lfnblock );
    r.r_di = FP_OFF( &lfnblock );          /* The LFN find block */
    r.r_bx = buf->lfnax;                   /* The lfn handle set by findfirst */
    r.r_si = 1;                            /* Use DOS times */
    r.r_ax = 0x714F;

    intrpt( 0x21, &r );

    /* Check for errors */
    if( ( r.r_flags & 1 ) ) {
        errno = r.r_ax;
        return( -1 );
    }
    convert_to_ffblk( buf, &lfnblock );
    return( 0 );
}

int lfnfindclose( struct lfnffblk *buf )
{
    IREGS r;

    /* Let's check if LFN was used; if not, there is no need for findclose */
    if( !buf->lfnsup || !__supportlfns ) return( 0 );

    r.r_bx = buf->lfnax;        /* Findfirst handle */
    r.r_ax = 0x71A1;            /* LFN findclose */

    intrpt( 0x21, &r );

    /*
     * Check for errors (which really shouldn't be a problem anyways
     * except for bad code)
     */
    if( ( r.r_flags & 1 ) ) {
        errno = r.r_ax;
        return( -1 );
    }
    return( 0 );
}

int lfnmkdir( const char *path )
{
	IREGS r;

    r.r_dx = FP_OFF( path );
	r.r_ds = FP_SEG( path );
	r.r_flags = 1;
    if (__supportlfns) {
        r.r_ax = 0x7139;
        intrpt( 0x21, &r );
	}
    if ( ( r.r_flags & 1 ) || r.r_ax == 0x7100 ) {
        r.r_ax = 0x3900;
        intrpt( 0x21, &r );
	}
    return( -( r.r_flags & 1 ) );
}

static int lfn_rc_dir( const char *path, int func )
{
	IREGS r;

    path = getshortfilename( path );
    r.r_ax = func;
    r.r_dx = FP_OFF( path );
	r.r_ds = FP_SEG( path );
    intrpt( 0x21, &r );
    return( -( r.r_flags & 1 ) );
}

int lfnrmdir( const char *path )
{
	return lfn_rc_dir( path, 0x3a00 );
}

int lfnchdir( const char *path )
{
	return lfn_rc_dir( path, 0x3b00 );
}

/* #endif */

#endif
