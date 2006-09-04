/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-2000 Steffen Kaiser

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

	Declarations for the supplemental I/O functions.

	To overcome the difference between Micro-C and ANSI C:
	All identifiers starts with an uppercase 'F', the leading
	lowercase 'f' is stripped, if present.

	Functions that will automatically terminate the program with an
	appropriate error message if the action fails. They have an uppercase
	'E' instead of the 'F'. The Eopen(), Ecreate() and Eclose() functions
	use the Fxopen() and Fxclose() to save the extended attributes;
	they use the filename to issue with the error messages.

	Three functions support the "extended attributes per file" feature:
	Fxopen(): open a file and generate & store the extended attributes
	Fxclose(): close the file and destroy the extended attributes
	Fxinfo(): retrieve the extended attributes

	The implementation is such that Fxopen() returns a normal (FILE*),
	in order to be passed to the normal F*() functions, but also sets
	up an association between the (FILE*) pointer and the extended
	attributes.

	If the macro SUPPL_USE_EXTENDED_ATTRIBUTES is defined prior
	#include'ing this header file, Fxopen() and Fxclose() replace
	Fopen() and Fclose() by default.

*/

#ifndef __SUPPLIO_H
#define __SUPPLIO_H

#include <stdio.h>
#include <supl_def.h>
#ifndef __PORTABLE_H
#include <portable.h>
#endif

#ifdef _PAC_NOCLIB_
/*
 *	Missing fpos_t and assoc. functions
 */

typedef long fpos_t;

#define fsetpos(stream,position) fseek((stream), *(position), 0)
int fgetpos(FILE * const fp, fpos_t * const pos);
/* fsetpos() is similar to fseek() and fgetpos() to ftell(), though,
	better portabel. Both return 0 (zero) on success.
*/
#endif

/*
 *	Macros passed into Fsetmode()
 */
#define FF_RD 1		/* read access */
#define FF_WR 2		/* write access */
#define FF_RW 3		/* read & write access */

/*
 *	Replacement of stdio functions
 *	All of these macros evaluate all parameter exactly once.
 */
#define Fflush fflush
#define Fyclose fclose
#define Frewind rewind
#define Fgetc(fp) getc((fp))
#define Fputc(c,fp) putc((c),(fp))
#define Ffgetc(fp) fgetc((fp))
#define Ffputc(c,fp) fputc((c),(fp))
#define Fgets fgets
#define Fputs fputs
#define Ferror ferror
#define Fclearerr clearerr
#ifndef _MICROC_
#define Fget(buf,size,fp) fread((buf), 1, (size), (fp))
#define Fput(buf,size,fp) fwrite((buf), 1, (size), (fp))
#define Fgetpos(fp,pos) fgetpos((fp), (pos))
#define Fsetpos(fp,pos) fsetpos((fp), (pos))
#define Fsetmode(fp,mode) 	/* overcome Micro-C bug with R&W files */
#define Fseeki(fp,ofs)	fseek((fp), (long)(ofs), SEEK_CUR)
#define Fseekc(fp,pos)	Fseeki((fp), (ofs))
#define Fpseekc(fp,pos)	Fseeki((fp), *(ofs))
#define Fseeke(fp)	fseek((fp), (long)0, SEEK_END)
#define Fyopen fopen
#define Fseek(fp,pos,whence)	fseek((fp), (long)(pos), (whence))
#define Fpseek(fp,pos,whence)	Fseek((fp), *(pos), (whence))
#define Fileno(fp) fileno((fp))
#define Fdopen fdopen
#define Feof feof
#else
#include <file.h>
#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif
#define fpos_t dword
#define Fput fput
#define Fget fget
void Fsetmode(FILE *fp, int mode);
int Fgetpos(FILE *fp, fpos_t *pos);
#define Fsetpos(fp,pos)	Fseek((fp), (pos), SEEK_SET)
int Fseeki(FILE *fp, int ofs);
#define Fseekc(fp,pos)	Fseek((fp), (pos), SEEK_CUR)
#define Fpseekc(fp,pos)	Fseekc((fp),(pos))
#define Fseeke(fp)	fseek((fp), 0, 0, SEEK_END)
FILE *Fyopen(char *fnam, char *mode);
FILE *Fdopen(int fd, char *mode);
int Fseek(FILE *fp, dword *pos, int whence);
#define Fpseek Fseek
#define Fileno(fp)	(fp)->FILE_handle
int Feof(FILE *fp);
#endif
#define FFgetpos(fp,pos) Fgetpos((fp), aS(pos))
#define FFsetpos(fp,pos) Fsetpos((fp), aS(pos))

/* Read/write a buffer to the file.
 *	Is a function in order to evaluate all parameters only once.
 *	Return:
 *		0: on success
 */
int FFget(void * const buf, size_t len, FILE *f);
int FFput(const void * const buf, size_t len, FILE *f);

/*
 * Commit a file
 *
 *	Flushes the buffers cached by the C library, and
 *	Flushes the buffers cached by the system and also makes sure
 *	that the directory structure is updated.
 */
#define Fcommit fcommit
int fcommit(FILE *fp);
int commit(int fd);

/* Read a line into dynamical memory.
 *
 *	There is always an '\n' (newline) character at the end of the line,
 *	even at the end of file without one.
 *
 *	Return:
 *		NULL: on failure (out of memory)
 *		else: pointer to malloc()'ed buffer, must be free()'ed by the user
 *			if the returned buffer is a pointer to "", EOF or a read
 *			error was encountered.
 *			Note: The "" must also be free()'ed.
 */
char *Fgetline(FILE * const f);


FILE *Ftmpfile(void);
/* Create and open a temporary file in binary mode

	Should be closed by Fxclose() in order to remove the file.

	Return:
		NULL: on failure (e.g. malloc() failed, no writeble directory found)
		else: opened FILE* pointer to be closed with Fxclose()
*/

FILE *F_tmpfile(const char * const fnam);
/* Create and open a temporary file in binary mode

	Should be closed by Fxclose() in order to remove the file.

	Return:
		NULL: on failure (e.g. malloc() failed, or fnam == NULL)
			removes external file 'fnam' if it exists
		else: opened FILE* pointer to be closed with Fxclose()
*/

char *Ftmpnam(const char * const ext);
/* Create a temporary filename, create it as a zero-length file and
	return its name.

	The name is overwritten the next time Ftmpnam(), Ftmpdir(),
	Etmpnam() or Etmpdir() is called.

	Return:
		NULL: on failure, e.g. malloc() failed, no writeble directory found
*/

char *Ftmpdir(void);
/* Return the current writeble temporary directory with a trailing
	backslash.

	The user must free() the returned string manually.
	This function overwrites the string returned by Ftmpnam() or
	Etmpnam().

	Return:
		NULL: on failure, e.g. malloc() failed, no directory found
		else: pointer to pathname
*/

#ifdef _MICROC_
#define Fposcpy(dst,src)	longcpy(dst, src)
#else
#define Fposcpy(dst,src)	memcpy((dst), (src), sizeof(fpos_t))
#endif
/* Copy the value of one position to another location
*/

	/* if debug enabled --> use the function and provide the test
		if fpos_t==long */
int Fposcmp(const fpos_t * const pos1, const fpos_t * const pos2);

#ifdef NDEBUG
#define Fposcmp(pos1,pos2)	\
	longcmp1(nM(*(unsigned long*))pos1, nM(*(unsigned long*))pos2)
#endif
/* Compare both positions
	Return:
		<0: if pos1 < pos2
		=0: if pos1 = pos2
		>0: if pos1 > pos2
*/

#ifdef _MICROC_
#define Fpos2dword(fpos,longPos) Fposcpy(fpos, longPos)
#define Fppos2dword(fpos,longPos) Fposcpy(fpos, longPos)
#else
#define Fpos2dword(fpos,longPos) (longPos) = (dword)(fpos);
#define Fppos2dword(fpos,longPos) *(longPos) = (dword)(fpos);
#endif
/* Extract the file position from a (fpos_t) type

	Note: There is _NO_ reverse function.

	Also note: (fpos_t) is known to contain more than just the
	file position, when special text handling code is enabled, such
	as multi-byte characters.
*/

int Fmaxbuf(byte ** const buf, size_t * const len);
/* Allocate are rather large amount of memory, usually a power of 2
	Return: 0 on success
 */

int Fcopyto(FILE * const fdst, FILE * const fsrc, const fpos_t * const pos);
/* Copy (a part of) a file to another.
	Both files must be already opened with the proper permissions and
	file modes (e.g. both either binary or text, fdst has write permission,
	fsrc has read permission).
	Any bytes from fsrc are copied to fdst upto, but excluding to the
	position 'pos'.
	If the soruce file is in text mode, it is possible that 'pos' points
	to an invalid file position. In this case the behaviour is undefined.

	Return:
		0: OK
		else: on failure
*/

int Fcopy(FILE * const fdst, FILE * const fsrc);
/* Copy any byte from fsrc into fdst
	Return: 0 on success
*/

int Fcopyi(FILE * const fdst, FILE * const fsrc, size_t len);
/* Copy len byte from fsrc into fdst
	Return: 0 on success
*/

int Fcopyl(FILE * const fdst, FILE * const fsrc, dword iM(*) len);
/* Copy len byte from fsrc into fdst
	Return: 0 on success
*/

int Fcopybuf(FILE * const fdst, FILE * const fsrc, byte * const buf
	, const size_t len, size_t * const num);
/* Copies one block from one stream into another one.
	Return: 0 on success
*/

/*
 *	Replace for the F*() functions to automatically terminate the
 *	program if the action fails.
 *
 *	If the FILE* pointer is associated with an extended attribute,
 *	the message contains the name of the file.
 */

FILE *Eopen(const char * const fnam, const char * const mode);
/* Open file as Fopen(), on failure return: "Cannot open file"
*/

FILE *Ecreate(const char * const fnam, const char * const mode);
/* Open file as Fopen(), on failure return: "Cannot create file"
*/

void Eclose(FILE *f);
/* Close the file and free all allocated resources. If the error flag
	is true, the program terminates with: "Error to access file"
*/

#define Erewind	Frewind
#define Egetc Fgetc
#define Efgetc Ffgetc
#define Efputc Ffputc
#define Eputc Fputc
#define Esetmode	Fsetmode
#define Efileno Fileno
/* Definitions that don't generate error states or, if they would be
	checked, it could be drastically performance drop */


char *Egets(char * const str, size_t size, FILE * const fp);
/* Inputs a line from the stream; on failure the program
	is terminated with: "Cannot read from file"
	Return: NULL: if EOF
			else: str
*/

char *Eputs(const char * const str, FILE * const fp);
/* Outputs a line into the stream; on failure the program
	is terminated with: "Cannot write into file"
	Return: str
*/

void *Eget(void * const buf, size_t size, FILE * const fp);
/* Read size bytes from the stream into the buffer; on failure
	terminate the program with: "Cannot read from file"
	Return: buf
*/

void Eput(const void * const buf, size_t size, FILE * const fp);
/* Write size bytes from the buffer into the stream; on failure
	terminate the program with: "Cannot write into file"
*/

#define EEgetpos(fp,pos) Egetpos((fp), aS(pos))
void Egetpos(FILE * const fp, fpos_t * const pos);
/* Retreive the file position from the stream; on failure terminate
	the program with: "Cannot access file"
*/

#define EEsetpos(fp,pos) Esetpos((fp), aS(pos))
void Esetpos(FILE * const fp, const fpos_t * const pos);
/* Seek to the position saved in *pos; on failure terminate the
	program with: "Cannot access file"
*/

#define Eclearerr Fclearerr
void Eerror(const FILE * const fp);
/* Terminate the program with "Cannot access file" if the ferror()
	statue is true.
*/

void Ereaderr(const FILE * const fp);
void Ewriteerr(const FILE * const fp);
/* Terminate the program with "Cannot read from file" or "Cannot
	write to file" if the ferror() status is true
*/

#ifndef _MICROC_
#define Eseeki(fp,ofs)	Eseek((fp), (long)(ofs), SEEK_CUR)
#define Eseekc(fp,pos)	Eseeki((fp), (ofs))
#define Epseekc(fp,pos)	Eseeki((fp), *(ofs))
#define Eseeke(fp)	Eseek((fp), (long)0, SEEK_END)
#define Epseek(fp,pos,whence)	Eseek((fp), *(pos), (whence))
void Eseek(FILE * const fp, dword pos, int whence);
#else
#define Eseeki(fp,ofs)	Efseek((fp), (ofs), SEEK_CUR)
#define Eseeke(fp)	Efseek((fp), 0, SEEK_END)
void Efseek(FILE *fp, int pos, int whence);
#define Eseekc(fp,pos)	Eseek((fp), (ofs), SEEK_CUR)
#define Epseekc(fp,pos)	Eseek((fp), (ofs), SEEK_CUR)
#define Epseek Eseek
void Eseek(FILE * fp, dword *pos, int whence);
#endif
/* On failure terminate with: "Cannot access file"
*/

/* Read a line into dynamical memory.
 *
 *	On error, an appropriate message is issued and the program
 *	is terminated.
 *
 *	There is always an '\n' (newline) character at the end of the line,
 *	even at the end of file without one.
 *
 *	Return:
 *		NULL: EOF (or read error)
 *		else: pointer to malloc()'ed buffer, must be free()'ed by the user
 */
char *Egetline(FILE * const f);

FILE *Etmpfile(void);
/* Create and open a binary temporary file. On failure the program
	is terminated with: Cannot create temporary file
	The file is create with mode "w+b"
*/

char *Etmpnam(const char * const ext);
/* Create a temporary filename with the extension ext. On failure
	terminate the program.

	The returned string is overwritten by the next call to Ftmpnam(),
	Ftmpdir(), Etmpnam() or Etmpdir().
*/

char *Etmpdir(void);
/* Return the current writeble temporary directory with a trailing backslash.
	On failure the program is terminated
	The returned string must be free()'ed by the user of this function.
	This function overwrites the string returned by Ftmpnam() or Etmpnam().
*/


/*
 *	Extended attributes
 */
struct Suppl_ExtAttr_ {	/* extended attributes */
	struct Suppl_ExtAttr_ *sea_nxt;
	FILE *sea_fp;		/* associated FILE* pointer; if NULL this is the
							default entry */
	char *sea_fnam;		/* absolute filename; dflt: "<<unknown filename>>" */
	FLAG sea_istmp;		/* is temporary file */
};
#ifdef _MICROC_
#define Suppl_ExtAttr struct Suppl_ExtAttr_
#else
typedef struct Suppl_ExtAttr_ Suppl_ExtAttr;
#endif

extern Suppl_ExtAttr suppl_dfltExtAttr;	/* Returned by Fxinfo() on failure */

Suppl_ExtAttr *F_xopen(const char * const fnam, const char * const mode);
FILE *Fxopen(const char * const fnam, const char * const mode);
FILE *Exopen(const char * const fnam, const char * const mode);
void Fxclose(FILE *fp);
void Exclose(FILE *fp);
Suppl_ExtAttr *F_xinfo(const FILE * const fp, Suppl_ExtAttr ** const prev);
Suppl_ExtAttr *Fxinfo(const FILE * const fp);
#define E_xinfo F_xinfo
#define Exinfo Fxinfo
/* Return the extended attributes associated with the FILE* pointer fp;

	If fp is not associated, a default entry is returned, which 'fp' is
	NULL, but all other members contain some sort of useful data

	Note: F_xinfo() is for internal puprose only.
*/

void Fxcloseall(void);
/* Close all functions opened by Fxopen()
 */

#ifdef SUPPL_USE_EXTENDED_ATTRIBUTES
#define Fopen Fxopen
#define Fclose Fxclose
#define Eopen Exopen
#define Eclose Exclose
#else
#define Fopen Fyopen
#define Fclose Fyclose
#endif

#endif
