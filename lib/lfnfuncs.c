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

#pragma warn -sus

#define _STC __emit__(0xF9)

char * getshortfilename( const char *longfilename )
{
    static char shortfilename[ 128 ];
    
    _DS = FP_SEG( longfilename );
    _SI = FP_OFF( longfilename );
    _ES = FP_SEG( shortfilename );
    _DI = FP_OFF( shortfilename );
    _CL = 0x01; /* Get short filename */
    _CH = 0x80; /* Include substed drive letters rather than path to them */
    _AX = 0x7160;/* LFN truename function */
    _STC;

    geninterrupt( 0x21 );

    if( _AX == 0x7100 || _CFLAG ) return( NULL );

    return( shortfilename );
}

int lfn_chmod( const char *filename, int func, ... )
{
    char *sfn = getshortfilename( filename );
    int attrib = 0;
    va_list vargs;

    if( sfn == NULL ) sfn = filename;
    va_start( vargs, func );
    if( func )attrib = va_arg( vargs, int );
    va_end( vargs );

    return( _chmod( sfn, func, attrib ) );
}

int lfnchmod( const char *filename, int mode )
{
    char *sfn = getshortfilename( filename );

    if( sfn == NULL ) sfn = filename;
    return( chmod( sfn, mode ) );
}

int lfncreat( const char *filename, int amode )
{
    char *sfn = getshortfilename( filename );

    if( sfn == NULL ) sfn = filename;
    return( creat( sfn, amode ) );
}

int lfn_creat( const char *filename, int mode )
{
    char *sfn = getshortfilename( filename );

    if( sfn == NULL ) sfn = filename;
    return( _creat( sfn, mode ) );
}

int lfncreatnew( const char *filename, int mode )
{
    char *sfn = getshortfilename( filename );

    if( sfn == NULL ) sfn = filename;
    return( creatnew( sfn, mode ) );
}

int lfncreattemp( const char *filename, int attrib )
{
    char *sfn = getshortfilename( filename );

    if( sfn == NULL ) sfn = filename;
    return( creattemp( sfn, attrib ) );
}

int lfnaccess( const char *filename, int mode )
{
    int attribs = lfn_chmod( filename, 0 );

    return( ( attribs == -1 ) ? -1 : ( mode == 0 || mode == 1 || mode == 4 ) ?
           0 : ( ( mode == 2 || mode == 6 ) && !( attribs & FA_RDONLY ) ) ? 0 :
           -1 );
}

char * lfn_getdcwd( int drive, char *buffer, int size )
{
    char *tmpbuf;
    static char longfilename[ 270 ];
    
    if( !buffer ) tmpbuf = _getdcwd( drive, NULL, size );
    else if( _getdcwd( drive, tmpbuf, size ) == NULL ) return( NULL );
    if( tmpbuf == NULL ) return( NULL );

    _DS = FP_SEG( tmpbuf );
    _DX = FP_OFF( tmpbuf );
    _ES = FP_SEG( longfilename );
    _DI = FP_OFF( longfilename );
    _CL = 0x02;
    _CH = 0x80;
    _AX = 0x7160;
    _STC;

    geninterrupt( 0x21 );

    if( _AX == 0x7100 || _CFLAG ) return( ( buffer = tmpbuf ) );
    if( size < strlen( longfilename ) ) {
        errno = ERANGE;
        return( NULL );
    }
    if( ( buffer = malloc( strlen( longfilename ) + 1 ) ) == NULL ) {
        errno = ENOMEM;
        return( NULL );
    }

    strcpy( buffer, longfilename );
    return( buffer );
}

FILE * lfnfopen( const char *filename, const char *mode )
{
    char *sfn = getshortfilename( filename );

    if( sfn == NULL ) sfn = filename;
    return( fopen( sfn, mode ) );
}

int lfnopen( const char *filename, int access, ... )
{
    char *sfn = getshortfilename( filename );
    unsigned mode = 0;
    va_list vargs;

    va_start( vargs, access );
    if( access & O_CREAT )mode = va_arg( vargs, unsigned );
    va_end( vargs );

    if( sfn == NULL ) sfn = filename;
    return( open( sfn, access, mode ) );
}

int lfn_open( const char *filename, int omode )
{
    char *sfn = getshortfilename( filename );

    if( sfn == NULL ) sfn = filename;
    return( _open( sfn, omode ) );
}

int lfnremove( const char *filename )
{
    char *sfn = getshortfilename( filename );

    if( sfn == NULL ) sfn = filename;
    return( remove( sfn ) );
}

int lfnrename( const char *oldfilename, const char *newfilename )
{   /* Must use the actual interrupt for this */
    _DS = FP_SEG( oldfilename );
    _DX = FP_OFF( oldfilename );
    _ES = FP_SEG( newfilename );
    _DI = FP_OFF( newfilename );
    _AX = 0x7156;
    _STC;

    geninterrupt( 0x21 );

    if( _AX == 0x7100 || _CFLAG ) {
        _AH = 0x56;

        geninterrupt( 0x21 );
        if( _CFLAG ) errno = _AX;
        return( -1 );
    }
    return( 0 );
}

int lfnstat( const char *filename, struct stat * statbuf )
{
    char *sfn = getshortfilename( filename );

    if( sfn == NULL ) sfn = filename;
    return( stat( sfn, statbuf ) );
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
    _DS = FP_SEG( path );
    _DX = FP_OFF( path );       /* path goes in DS:DX */
    _ES = FP_SEG( &lfnblock );
    _DI = FP_OFF( &lfnblock );  /* LFN find block goes in ES:DI */
    _SI = 1;                    /* Use DOS date/time format */
    _CX = attr;
    _AX = 0x714E;               /* LFN Findfirst */
    _STC;

    geninterrupt( 0x21 );

    /*
     * If ax = 7100, there is probably an LFN TSR but no LFN support for
     * whatever drive or directory is being searched. In that case, fall back on
     * the old findfirst.  Also if the function fails, it could be because of
     * no LFN TSR so fall back to the old findfirst.
     */
    if( _AX == 0x7100 || _CFLAG )
        return( findfirst( path, buf, attr ) );

    /*
     * If there was no failure, the next step is to move the values from the
     * LFN block into the non-lfn block
     */
    buf->lfnax = _AX;              /* The find handle for findnext */
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
    if( !buf->lfnsup ) {
        return( findnext( buf ) );
    }

    _ES = FP_SEG( &lfnblock );
    _DI = FP_SEG( &lfnblock );          /* The LFN find block */
    _SI = 1;                            /* Use DOS times */
    _BX = buf->lfnax;                   /* The lfn handle set by findfirst */
    _AX = 0x714F;
    _STC;

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
