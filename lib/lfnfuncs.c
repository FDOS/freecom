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

#define __LFNFUNCS_C

#include "../include/misc.h"
#include "../include/lfnfuncs.h"
#include "../strings.h"
#include "../err_fcts.h"
#include "../include/cmdline.h"


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
    
    _PUSH_DS;

    _DS = FP_SEG( longfilename );
    _SI = FP_OFF( longfilename );
    _ES = FP_SEG( shortfilename );
    _DI = FP_OFF( shortfilename );
    _CL = 0x01; /* Get short filename */
    _CH = 0x80; /* Include substed drive letters rather than path to them */
    _AX = 0x7160;/* LFN truename function */
    _STC;

    geninterrupt( 0x21 );

    _POP_DS;

    return( ( _CFLAG || _AX == 0x7100 || !__supportlfns ) ?
            longfilename : shortfilename );
}

static void __creat_or_truncate( const char * filename, int mode )
{
    int handle;

    if( !__supportlfns ) {
        _close( creatnew( filename, mode ) );
        return;
    }

    _PUSH_DS;
    _DS = FP_SEG( filename );
    _SI = FP_OFF( filename );
    _BX = O_WRONLY;
    _CX = mode;
    _DX = 0x10;
    _AX = 0x716C;
    _STC;

    geninterrupt( 0x21 );

    _POP_DS;

    if( _CFLAG || _AX == 0x7100 )
        handle = ( access( getshortfilename( filename ), 0 ) == 0 ) ?
                 -1 : creatnew( filename, mode );
    else handle = _AX;
    /*
     * Win2k always returns handle == 2, which is a bug.
     * File handle 2 is already used for stderr
     * Fortunately, FreeCOM never actually expects to get file handle 2
     * ( as it does for '1' and '0', when redirecting stdin and stdout )
     */
    if( handle != 2 )_close( handle );
}

#if 0 /* these functions aren't used at all in the FreeCOM source code */
int lfn_chmod( const char *filename, int func, ... )
{
    int attrib = 0;
    va_list vargs;

    va_start( vargs, func );
    if( func )attrib = va_arg( vargs, int );
    va_end( vargs );

    return( _chmod( getshortfilename( filename ), func, attrib ) );
}

int lfn_creat( const char *filename, int mode )
{
    __creat_or_truncate( filename, mode );
    return( _open( getshortfilename( filename ), O_BINARY|O_RDWR|O_TRUNC ) );
}

int lfncreat( const char *filename, int amode )
{
    __creat_or_truncate( filename,
                         ( !( amode & S_IWRITE ) ) ? FA_RDONLY : 0 );
    return( _open( getshortfilename( filename ), _fmode|O_RDWR|O_TRUNC ) );

}

int lfncreatnew( const char *filename, int mode )
{
    if( access( getshortfilename( filename ), 0 ) == 0 ) {
        errno = EEXIST;
        return( -1 );
    }
    __creat_or_truncate( filename, mode );
    return( _open( getshortfilename( filename ), O_RDWR ) );
}
#endif

FILE * lfnfopen( const char *filename, const char *mode )
{
    if( strchr( mode, 'a' ) || strchr( mode, 'w' ) )
        __creat_or_truncate( filename, 0 );
    return( fopen( getshortfilename( filename ), mode ) );
}

int lfnopen( const char *filename, int access, ... )
{
    va_list vargs;

    va_start( vargs, access );

    if( access & O_CREAT ) {
        access &= ~O_CREAT; /* Remove the O_CREAT bit */

        __creat_or_truncate( filename, va_arg( vargs, unsigned ) );
    }
    va_end( vargs );

    return( open( getshortfilename( filename ), access ) );
}

int lfnrename( const char *oldfilename, const char *newfilename )
{   /* Must use the actual interrupt for this */
#if 1
    printf( "%s, %s\n", oldfilename, newfilename ); /* Debug */
    _PUSH_DS;

    _DS = FP_SEG( oldfilename );
    _DX = FP_OFF( oldfilename );
    _ES = FP_SEG( newfilename );
    _DI = FP_OFF( newfilename );
    _AX = 0x7156;
    _STC;

    geninterrupt( 0x21 );

    _POP_DS;

    if( _CFLAG || _AX == 0x7100 ) {
        _PUSH_DS;

        _DS = FP_SEG( oldfilename );
        _AH = 0x56;

        geninterrupt( 0x21 );

        _POP_DS;

        if( _CFLAG ) {
            errno = _AX;
            printf( "%s, %s\n", oldfilename, newfilename );

            return( -1 );
        }
    }

    return( 0 );
#else /* Debug */
    struct REGPACK r;
    printf( "%s, %s\n", oldfilename, newfilename );

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
#endif
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

    buf->lfnax = buf->lfnsup = 0; /* Zero find handle and LFN-supported flag */

    if( !__supportlfns )
        return( findfirst( path, ( struct ffblk * )buf, attr ) );

    _PUSH_DS;
    _DS = FP_SEG( path );
    _DX = FP_OFF( path );       /* path goes in DS:DX */
    _ES = FP_SEG( &lfnblock );
    _DI = FP_OFF( &lfnblock );  /* LFN find block goes in ES:DI */
    _SI = 1;                    /* Use DOS date/time format */
    _CX = attr;
    _AX = 0x714E;               /* LFN Findfirst */
    _STC;

    geninterrupt( 0x21 );

    _POP_DS;

    /*
     * If ax = 7100, there is probably an LFN TSR but no LFN support for
     * whatever drive or directory is being searched. In that case, fall back on
     * the old findfirst.  Also if the function fails, it could be because of
     * no LFN TSR so fall back to the old findfirst.
     */
    if( _CFLAG || ( buf->lfnax  = _AX ) == 0x7100 )
        return( findfirst( path, ( struct ffblk * )buf, attr ) );

    /*
     * If there was no failure, the next step is to move the values from the
     * LFN block into the non-lfn block
     */
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

    /*
     * Before going through the possibly unnecessary steps of calling the LFN
     * function first, buf->lfnsup will tell us if LFN was supported with the
     * previous call to findfirst.
     */
    if( !buf->lfnsup || !__supportlfns ) {
        return( findnext( ( struct ffblk * )buf ) );
    }

    _ES = FP_SEG( &lfnblock );
    _DI = FP_OFF( &lfnblock );          /* The LFN find block */
    _BX = buf->lfnax;                   /* The lfn handle set by findfirst */
    _SI = 1;                            /* Use DOS times */
    _AX = 0x714F;

    geninterrupt( 0x21 );

    /* Check for errors */
    if( _CFLAG ) {
        errno = _AX;
        return( -1 );
    }
    convert_to_ffblk( buf, &lfnblock );
    return( 0 );
}

int lfnfindclose( struct lfnffblk *buf )
{
    /* Let's check if LFN was used; if not, there is no need for findclose */
    if( !buf->lfnsup ) return( 0 );

    _BX = buf->lfnax;        /* Findfirst handle */
    _AX = 0x71A1;            /* LFN findclose */

    geninterrupt( 0x21 );

    /*
     * Check for errors (which really shouldn't be a problem anyways
     * except for bad code)
     */
    if( _CFLAG ) {
        errno = _AX;
        return( -1 );
    }
    return( 0 );
}

#endif

#endif
