/* $Id$
   $Locker$	$Name$	$State$

	DOS NLS implementation -- "Country" information DOS-65-XX
	See DOC\CNTRY.H

	These functions provide an easy access to the information
	stored in the DOS NLS, but does not actually offer any
	functions to transform/compare etc.

   $Log$
   Revision 1.2  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 1.3  2000/03/31 09:09:32  ska
   add: DBG_CLOSELOG, suppl_l_openmode
   add: SYSLOG_ALWAYS_FLUSHLOG
   add: fcommit(), Fcommit(), Fflush(), commit()
   add: suppl_log_flush(), DBG_FLUSHLOG, DBG_ALWAYS_FLUSHLOG
   fix: dfnsearch(): DBG_ARGUMENT()
   chg: F[gs]etpos() -> true replacements of f[gs]etpos(); removed
   	Fp[gs]etpos(); added FF[gs]etpos() using a non-pointer argument
   bugfix: secure string functions: memory functions don't accept length == 0
   add: MKSTRFCT.PL: generate DOC\SSTR.FCT
   fix: dfnsplit(): a pathname without path components returns the root
   	directory as path portion
   add: dfnsplit(): debug output of return values of found drive/path/name/ext
   fix: dfnsqueeze(): DBG_ENTER() contains wrong names
   fix: dfnsplit(): chkHeap in drive spec detection routine breaks if/else
   chg: moved NLS-depended stuff from DFN.H into NLS_F.H
   add: integer SUPPL error codes -- all functions returning (int) error codes
   	return unique codes used throughout all SUPPL, see ERRCODES.H

   Revision 1.2  1999/07/02 05:16:11  ska
   bugfix: DFN_LABEL wrong; DFN_SYSTEM missing
   fix: cfgASBoolean: bad function name
   fix: cntry.h: 'char' members could be treated signed
   add: syslogo(), syslogv()
   add: SUPPL_LOG_MEMORY & *alloc/free/strdup replacements
   add: SUPPLDBG.H	as last #include into all *.C files
   fix: openlog() prevents recursive calls
   fix: syslog_printv() accepts syslog_logfile == NULL
   chg: syslog_openmode --> []

   Revision 1.1  1998/12/04 07:30:36  ska
   Initial revision

*/

#ifndef __CNTRY_H
#define __CNTRY_H

#ifndef __PORTABLE_H
#include <portable.h>
#endif
#include "suppl.h"

/* All information from Ralf Brown's interrupt list v5.6 */
struct Country_ {		/* local copy of country information */
	/* pointers to tables */
	fartbl uppercaseTbl;	/* uppercase for "normal" characters */
	fartbl lowercaseTbl;	/* lowercase for "normal" chars (maybe NULL) */
	fartbl fupcaseTbl;	/* uppercase for filename characters */
	fartbl collateTbl;	/* collating table */

	fartbl illegalChars;		/* illegal characters in filenames */
	int illegalLen;			/* illegal characters in filenames */

	/* length of the tables
		If length <= 0x80 the table does not contain values for
		the indexs 0..0x7F, but the pointers are adjusted that index 0x80
		really points to the first entry of the table. */
	int uppercaseLen;	/* uppercase for "normal" characters */
	int lowercaseLen;	/* lowercase for "normal" chars (maybe NULL) */
	int fupcaseLen;		/* uppercase for filename characters */
	int collateLen;		/* collating table */

	/* Extended country information */
	word country;		/* currently active DOS country code */
	word charset;		/* currently active DOS codepage */
	word datefmt;		/* format of date: 0/1/2 == USA/Europe/Japan */
	byte curSymbol[6];	/* ASCIZ currency symbol */
	byte thousendsSep[3];	/* ASCIZ decimal separator */
	byte decimalSep[3];	/* ASCIZ decimal separator */
	byte dateSep[3];	/* ASCIZ date separator */
	byte timeSep[3];	/* ASCIZ time separator */
	byte listSep[3];	/* ASCIZ list separator */
	byte curFormat;		/*	bit 0: 0/1 == preceed/follow value
							bit 1: separate cur sym from value by one space
							bit 2: cur sym replaces decimal point */
	byte precision;		/* number of digits after decimal point */
	byte timefmt;		/* bit 0: 0/1 == 12/24 hour clock */

	/* valid filename characters */
	byte inclFirst, inclLast;	/* permissible character range */
	byte exclFirst, exclLast;	/* excluded character range */

	byte initialized;
};

#ifdef _MICROC_
#define Country struct Country_
#else /*#	!(defined(_MICROC_)) */
typedef struct Country_ Country;
#endif /*#	defined(_MICROC_) */

Country *nlsInfo(void);
/*
 *	Return a pointer to a filled copy of above structure
 *
 *	During the life-time of a program the information is retrieved once.
 *
 *	Return: Always != NULL, but maybe ome fields are NULL.
 */

Country *nlsNewInfo(void);
/*
 *	As nlsInfo(), but causes to reread the information cached by
 *	nlsInfo().
 */

#endif /*#	!defined(__CNTRY_H) */
