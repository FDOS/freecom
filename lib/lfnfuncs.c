/* 
 * Gets the short filename equivalent 
 */

#include "../config.h"

#include <assert.h>
#include <dir.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <suppl.h>
#include <dfn.h>
#include <sstr.h>

#define __LFNFUNCS_C

#include "../include/misc.h"
#include "../include/lfnfuncs.h"
#include "../strings.h"
#include "../err_fcts.h"
#include "../include/cmdline.h"
#include "../include/openf.h"


#ifdef FEATURE_LONG_FILENAMES

#ifdef __TURBOC__ /* OpenWatcom has an LFN CLIB Extension available */

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
    struct REGPACK r;

/* This function causes an invalid opcode when working with NUL */
/* access() doesn't even work here */
    if( _close( _open( longfilename, 0 ) ) == 0 ) return( longfilename );

    r.r_ds = FP_SEG( longfilename );
    r.r_si = FP_OFF( longfilename );
    r.r_es = FP_SEG( shortfilename );
    r.r_di = FP_OFF( shortfilename );
    r.r_cx = 0x8001; /* Get short filename */
    r.r_ax = 0x7160;/* LFN truename function */

    intr( 0x21, &r );

    return( ( ( r.r_flags & 1 ) || r.r_ax == 0x7100 || !__supportlfns ) ?
            longfilename : shortfilename );
}

static int mycreatnew( const char * filename, int mode )
{
    struct REGPACK r;

    r.r_ds = FP_SEG( filename );
    r.r_dx = FP_OFF( filename );
    r.r_cx = mode;
    r.r_ax = 0x5B00;

    intr( 0x21, &r );

    if( ( r.r_flags & 1 ) ) r.r_ax = 0xFFFF;
    
    return( r.r_ax );
}

static void __creat_or_truncate( const char * filename, int mode )
{
    int handle;
    struct REGPACK r;

    if( !__supportlfns ) {
        _close( mycreatnew( filename, mode ) );
        return;
    }

    r.r_ds = FP_SEG( filename );
    r.r_si = FP_OFF( filename );
    r.r_bx = O_WRONLY;
    r.r_cx = mode;
    r.r_dx = 0x10;
    r.r_ax = 0x716C;

    intr( 0x21, &r );

    if( ( r.r_flags & 1 ) || r.r_ax == 0x7100 )
        handle = ( dfnstat( getshortfilename( filename ) ) != 0 ) ?
                 -1 : mycreatnew( filename, mode );
    else handle = r.r_ax;
    /*
     * Win2k always returns handle == 2, which is a bug.
     * File handle 2 is already used for stderr
     * Fortunately, FreeCOM never actually expects to get file handle 2
     * ( as it does for '1' and '0', when redirecting stdin and stdout )
     */
    if( handle != 2 )_close( handle );
}

FILE * lfnfopen( const char *filename, const char *mode )
{
    if( strpbrk( mode, "aw" ) )
        __creat_or_truncate( filename, 0 );
    return( fopen( getshortfilename( filename ), mode ) );
}

int lfnopen( const char *filename, int access, ... )
{
    va_list vargs;

    va_start( vargs, access );

    if( access & O_CREAT ) {
        access &= ~O_CREAT; /* Remove the O_CREAT bit */

        __creat_or_truncate( filename, !( va_arg( vargs, unsigned ) & S_IWRITE ) ? FA_RDONLY : 0 );
    }
    va_end( vargs );

    return( open( getshortfilename( filename ), access ) );
}

int lfnrename( const char *oldfilename, const char *newfilename )
{   /* Must use the actual interrupt for this */
    struct REGPACK r;

    if(dfnstat(newfilename) != 0) {
        errno = EACCES;
        return( -1 );
    }

    r.r_ds = FP_SEG( oldfilename );
    r.r_dx = FP_OFF( oldfilename );
    r.r_es = FP_SEG( newfilename );
    r.r_di = FP_OFF( newfilename );
    r.r_ax = 0x7156;

    intr( 0x21, &r );

    if( ( r.r_flags & 1 ) || r.r_ax == 0x7100 ) {
        r.r_ax = 0x5600;

        intr( 0x21, &r );

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
    struct REGPACK r;

    buf->lfnax = buf->lfnsup = 0; /* Zero find handle and LFN-supported flag */

    if( !__supportlfns )
        return( findfirst( path, ( struct ffblk * )buf, attr ) );

    r.r_ds = FP_SEG( path );
    r.r_dx = FP_OFF( path );       /* path goes in DS:DX */
    r.r_es = FP_SEG( &lfnblock );
    r.r_di = FP_OFF( &lfnblock );  /* LFN find block goes in ES:DI */
    r.r_si = 1;                    /* Use DOS date/time format */
    r.r_cx = attr;
    r.r_ax = 0x714E;               /* LFN Findfirst */

    intr( 0x21, &r );

    /*
     * If ax = 7100, there is probably an LFN TSR but no LFN support for
     * whatever drive or directory is being searched. In that case, fall back on
     * the old findfirst.  Also if the function fails, it could be because of
     * no LFN TSR so fall back to the old findfirst.
     */
    if( ( r.r_flags & 1 ) || r.r_ax == 0x7100 )
        return( findfirst( path, ( struct ffblk * )buf, attr ) );

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
    struct REGPACK r;

    /*
     * Before going through the possibly unnecessary steps of calling the LFN
     * function first, buf->lfnsup will tell us if LFN was supported with the
     * previous call to findfirst.
     */
    if( !buf->lfnsup || !__supportlfns ) {
        return( findnext( ( struct ffblk * )buf ) );
    }

    r.r_es = FP_SEG( &lfnblock );
    r.r_di = FP_OFF( &lfnblock );          /* The LFN find block */
    r.r_bx = buf->lfnax;                   /* The lfn handle set by findfirst */
    r.r_si = 1;                            /* Use DOS times */
    r.r_ax = 0x714F;

    intr( 0x21, &r );

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
    struct REGPACK r;

    /* Let's check if LFN was used; if not, there is no need for findclose */
    if( !buf->lfnsup || !__supportlfns ) return( 0 );

    r.r_bx = buf->lfnax;        /* Findfirst handle */
    r.r_ax = 0x71A1;            /* LFN findclose */

    intr( 0x21, &r );

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

#endif

#endif
