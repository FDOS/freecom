/*  $Id$

    Search for an external program and return it in a static buffer,
    which is overwritten the next time this function is called.

    This file bases on OPENF.C of FreeCOM v0.81 beta 1.

    $Log$
    Revision 1.9  2006/08/06 05:04:45  blairdude
    Fixed (hopefully) bug in %PATH% parsing routing and A:/B: random accessing bug.

    Revision 1.8  2006/06/27 07:09:26  blairdude
    find_which now uses simpler functions to parse %PATH% to find executables.
    
    Revision 1.7  2006/06/26 18:36:36  blairdude
    FreeCOM can now execute long filenamed executable files.
    
    Revision 1.6  2005/12/10 10:09:43  perditionc
    based on patches from Blair Campbell, additional LFN support (slim print,
    add initial cd,rd,md support, make compile time optional), remove some
    compiler warnings, and prevent extra linebreak for compatibility
    
    Revision 1.5  2004/09/13 18:59:40  skaus
    add: CRITER: Repeat check autofail magic {Tom Ehlert/Eric Auer}
    
    Revision 1.4  2004/02/01 13:52:17  skaus
    add/upd: CVS $id$ keywords to/of files
    
    Revision 1.3  2004/02/01 13:24:22  skaus
    bugfix: misidentifying unspecific failures from within SUPPL
    
    Revision 1.2  2002/11/05 19:27:37  skaus
    bugfix: FreeCOM is to pass the fully-qualified path to DOS-4B-00
    
    Revision 1.1  2001/04/12 00:33:53  skaus
    chg: new structure
    chg: If DEBUG enabled, no available commands are displayed on startup
    fix: PTCHSIZE also patches min extra size to force to have this amount
       of memory available on start
    bugfix: CALL doesn't reset options
    add: PTCHSIZE to patch heap size
    add: VSPAWN, /SWAP switch, .SWP resource handling
    bugfix: COMMAND.COM A:\
    bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
    add: command MEMORY
    bugfix: runExtension(): destroys command[-2]
    add: clean.bat
    add: localized CRITER strings
    chg: use LNG files for hard-coded strings (hangForEver(), init.c)
        via STRINGS.LIB
    add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
    add: fixstrs.c: prompts & symbolic keys
    add: fixstrs.c: backslash escape sequences
    add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
    chg: splitted code apart into LIB\*.c and CMD\*.c
    bugfix: IF is now using error system & STRINGS to report errors
    add: CALL: /N
    
 */

#include "../config.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>

#include <dfn.h>

#include "../include/lfnfuncs.h"
#include "../err_fcts.h"
#include "../include/misc.h"

static char *__addcomexebat( char * file )
{
    static char tmpname[ MAXPATH ];

    sprintf( tmpname, "%s.COM", file );
    if( access( tmpname, 0 ) == 0 ) return( tmpname );
    sprintf( tmpname, "%s.EXE", file );
    if( access( tmpname, 0 ) == 0 ) return( tmpname );
    sprintf( tmpname, "%s.BAT", file );
    if( access( tmpname, 0 ) == 0 ) return( tmpname );
    if( access( file, 0 ) == 0 ) return( strcpy( tmpname, file ) );
    return( NULL );
}

/* The return value is overwritten on subsequent calls */
static char *_searchpath( const char *name )
{
    char *envptr = getEnv( "PATH" );
    static char file[ MAXPATH ], *retpt;

    strcpy( file, name );
    if( ( retpt = __addcomexebat( file ) ) != NULL ) return( retpt );
    while( envptr && envptr[0] ) {    /* While there are paths left to parse */
        char *tok = strchr( envptr, ';' );
        int toklen;
        if( tok == NULL ) toklen = strlen( envptr );
        else toklen = tok - envptr;
        memcpy( file, envptr, toklen );
        strcpy( file+toklen, "\\" );
        strcat( file, name );
        if( ( retpt = __addcomexebat( file ) ) != NULL ) return( retpt );
        envptr = tok;
        if( envptr ) envptr++;    /* Move on to the next one */
    }
    return( NULL );
}

char *find_which(const char * const fname)
{   char *p;
    static char *buf = 0;

#if 0
    critEnableRepeatCheck();
    if(0 == (p = dfnsearch(fname, 0, 0))) {
        if(errno == ENOMEM) {
            error_out_of_memory();
        }
        critEndRepCheck();
        return 0;
    }

#else
    if( 0 == ( p = _searchpath( fname ) ) ) return( 0 );
#endif
    free(buf);
#ifdef FEATURE_LONG_FILENAMES
    buf = abspath(getshortfilename(p), 1);
#else
    buf = abspath(p, 1);
#endif
#if 0
    free(p);

    critEndRepCheck();
#endif
    return buf;
}
