/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-99 Steffen Kaiser

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $Id$
   $Locker$	$Name$	$State$

	Declarations for the DOS filename functions.

*/

#ifndef __DFN_H
#define __DFN_H

#include <portable.h>

/* To support UNC paths, all functions are macro'ed from
	dfn*()   --->     dfnu*()
*/
#ifdef SUPPORT_UNC_PATH
#define dfnmerge dfnumerge
#define dfnsplit dfnusplit
#define dfnexpand dfnuexpand
#define dfnsqueeze dfnusqueeze
#define dfnmatch dfnumatch
#endif

#define DFN_FILE		0x4000
#define DFN_DIRECTORY	0x0010
#define DFN_LABEL		0x0008
#define DFN_WRITE		0x0001
#define DFN_HIDDEN		0x0002
#define DFN_SYSTEM		0x0004
/* Derived attributes */
#define DFN_READ		DFN_FILE	/* every file can be read */

int toFUpper(int ch);
/* Upcase character according the filename upcase table of the DOS NLS API.
	The upcase table is retreive only once during the program's run!
*/

int memcmpFI(const byte *buf1, const byte *buf2, unsigned len);
/* Compare both memory area case-insensitvely.

	To make the compare case-insensitve the toFUpper() function is used.

	If (buf1==NULL) || (buf2==NULL), the behaviour is undefined.
	If len == 0, memcmpFI() returns 0.

	Return:	
		 <0: buf1 is less then buf2
		==0: buf1 is equal to buf2
		 >0: buf1 is greater than buf2
*/

#ifdef _MICROC_
int _fmemcmpFI(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length);
#else
int _fmemcmpFI(const byte far * dest, const byte * src, unsigned length);
#endif
/* Compare both far memory area case-insensitvely.

	To make the compare case-insensitve the toFUpper() function is used.

	If (buf1==NULL) || (buf2==NULL), the behaviour is undefined.
	If len == 0, _fmemcmpFI() returns 0.

	Return:	
		 <0: buf1 is less then buf2
		==0: buf1 is equal to buf2
		 >0: buf1 is greater than buf2
*/


char *dfnexpand(const char fname[], char path[]);
/* Expands a filename to it a fully-qualified DOS-style path relative to path.
	The expanded filename is stored into a malloc'ed buffer.
	fname[] is preprocessed by dfnsqueeze().

	If path == NULL, the current working directory is used.

   Return:
   	NULL: malloc failed or fname == NULL
	else: pointer to the malloc'ed buffer
*/

char *dfnmerge(char fname[], const char dr[], const char path[]
 , const char name[], const char ext[]);
/* Merge a file name.

	dr, path, name, ext:
		Pointers of the path components.
		If one of these is NULL (thus, it is not present), it is not merged.
		If (*path=='\0'), this means the root path!

	fname := <dr>:<path>\<name>.<ext>
		':' is added, if <dr> is present;
		'\' is added, if <path>,& <name> or <ext> are present;
		'.' is added, if <ext> is present.

	fname:	Pointer to the memory to receive the result.
			If NULL: the necessary memory is malloc'ed.

   Return:
   	NULL if malloc failed.
   	otherwise: pointer to the beginning of the malloc()'ed result.
*/

int dfnsplit(char fname[], char **dr, char **path, char **name, char **ext);
/* Split a file name.

	fname is the file name to split.

	dr, path, name, ext:	Pointers, which will receive the results.
							If one pointer is NULL, the particular filename
								portion is not saved.
							If one pointer receives NULL, the particular
								portion was not found in fname.
							dr does not has the ':'.
							The trailing '\\' is stripped from path.
								==> нн (**path=='\0') -> Root path!!
									(This is not a bug but a feature ;-)
							ext does not contain the leading '.'; ext itself
								contains no dot at all.
							Multiple backslashes are reduced to a single one.
							name may start with any number of dots.

	The returned pointers in dr, path, name,& ext are malloc()'ed;
	thus, they must be free()'ed by the programmer!

   Return:
   	0: Transformation failed, e.g. malloc() returned NULL
	1: dfnsplit() OK
*/

char *dfnsqueeze(const char fname[]);
/* Preprocess a filename:
		- Flip '/' -> '\\'
		- Squeeze '\\\\' -> '\\'
		- Squeeze '\\.\0' -> '\\'
		- Squeeze '\\.\\' -> '\\'
		- Upcase all characters (using DOS NLS API)

	The preprocessed filename is copied into a malloc'ed buffer.

   Return:
   	NULL: malloc failed or fname == NULL
	else: pointer to the malloc'ed buffer
*/

char *dfnpath(int drive);
/* Retreive the fully-qualified path of: "<drive>:."
	The path is malloc()'ed and has _no_ trailing backslash, except the
	path is the root directory.

	drive may be:
		'A'-'Z', 'a'-'z', or 1..32 (note: 1 == 'A'; supports up to 32 drives)
		0 == current drive

	Return:
		NULL: malloc() failed, non-existing drive, or drive invalid
		else: pointer to the path
*/

char *dfntruename(const char fn[]);
/* Return the truename of the file named fn.

	The truename of a file is a fully-qualified filename according
	the _physical_ access path. That means that the truename can be
	used to exactly determine, if two file specifications match the
	same physical file, e.g.:
			SUBST Z: C:\FREEDOS
		&& C:\FREEDOS\SUBST.EXE exists
		&& the current working directory is C:\FREEDOS ==>
			dfntruename("a:/subst.exe") --> "C:\\FREDOS\\SUBST.EXE"
			dfntruename("subst.exe") --> "C:\\FREEDOS\\SUBST.EXE"
	The filename fn[] need not exist.

	Return:
		NULL: malloc() failed, or DOS API call failed
		else: pointer to the malloc()'ed string

	But beware! The truename must not be used as a function to fully-qualify
	a filename. There are at least two cases when that fails:
		<<see source code "DFNTRUEN.C" for more details>>
*/

char *dfnsearch(char fname[], char *path, char *ext);
/* Search for the file fname[] within the search path *path. If no extension
	is supplied, all extensions in *ext are tried. *ext may contain wildcards.

	If path==NULL, the environment variable "PATH" is retreived.
	If *path==0, no search path is available (thus, scanning in current
	working directory only).
	If ext==NULL, the extensions ".COM.EXE.BAT" are used.
	If *ext==0, the extension is not altered.
	if *ext=='.', this dot is ignored at all.

	If path contains an entry ".", the current working directory is not
	tried first.
	If the program is running in an Windows environment, the default Windows
	pathes are not prepended automatically.

	If fname[] already contains a drive or path specification, path is ignored.
	If fname[] already contains an extension, ext is ignored.

	The returned filename is not necessarily fully-qualified, nor it is
	pre-processed by dfnsqueeze(). All what is asured is that the returned
	filename can be found relative to the current working directory. The
	function does neither try to open the file nor to asure that this file
	is executable in any way. But Hidden or System files are not included
	into the search.

	Return:
		NULL: fname[] == NULL, *fname == 0, no file found, malloc() failed
		else: pointer to fully-qualified filename, it's malloc()'ed
*/

int dfnmatch(char *fnam, char *pattern);
/*	Check if the filename matches the DOS-style pattern.

	Components, like '.' and '..', and the drive are matched literally.
	Further details see dfnmatch.c.
*/

int isUNCpath(const char fname[]);
/* Return, if fname[] is an UNC path. It is preceeded by two backslashes.
*/

char *UNCpath(const char fname[]);
/* Return a pointer to the first character of fname[], that is not a part
	of the UNC drive specification. That is either '\\', the root direc
	specificator, or '\0', if no path is following the UNC drive at all.

	Return:
		NULL: fname[] has no UNC drive spec
		else: pointer to a) '\\' the root direc mark; or b) '\0' if no path
*/

int dfnwrdir(const char * const fnam);
/* Probe if new files can be created in the supplied directory.

	This is currently a hack, as it tries to create a file within
	the directory.

	Return:
		0: On success
		-1: not writeble
		1: malloc() failure
		2: cannot generate filename to probe

*/

int dfnstat(const char * const fnam);
/* Stat the filename and return the following information as
	a combinations of bits:
		DFN_FILE		file is a regular file
		DFN_DIRECTORY	file is a directory
		DFN_LABEL		file is a value label
		DFN_READ		read permissions granted
		DFN_WRITE		write permissions granted (my invalid for dirs)
		DFN_HIDDEN		not for normal display to user
*/

int dfnmkfile(const char * const fnam);
/* Create a _new_ file; fails, if file already exists or
	if path invalid.

	This is an atomic (semphore) operation as it combines the
	test with the set action.

	Return:
		0: on success
*/

void dfn_tmpnam(char * const fnam);
/* Puts exactly eight characters at fnam forming a temporary filename.
	No '\0' byte is appended, neither does the created name contain
	any NULs.
*/

char *dfnmktmp(const char * const path, const char * const ext);
/* Create a temporary file. path is duplicated into dynamic memory and
	is transformed into an absolute path.
	A filename (eight characters) is appended and, if ext != NULL,
	this extension as well.

	The file is created with as a regular file without any special
	attribute set.
*/

char *dfnfilename(const char * const fnam);
/* Return a pointer to the filename component of fnam.

	If there is no such portion (e.g. the path name ends in
	'\\', '/' or ':'), a pointer to the '\0' byte is returned.

	Return:
		NULL: fnam == NULL
		else: pointer to first byte of filename or to the '\0' byte
*/

char *dfnbakname(const char * const fnam);
/* Return a backup name of 'fnam' within the same directory of 'fnam'.
	The path is not processed in any way.
	'fnam' must contain a valid filename, with or without extension.
	Return:
		NULL: out-of-memory error
		else: pointer to dynamically allocated backup filename
*/

int dfnfreplace(const char * const newname, const char * const oldname);
/* Replace the file 'newname' by 'oldname' and backup the file 'newname'
	before it.

	Return:
		0: on success
		1: out-of-memory error
		2: access error (rename or unlink failed)
*/

#endif
