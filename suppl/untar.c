/* untar.c */

#define VERSION "1.4"

/* DESCRIPTION:
 *	Untar extracts files from an uncompressed tar archive, or one which
 *	has been compressed with gzip. Usually such archives will have file
 *	names that end with ".tar" or ".tgz" respectively, although untar
 *	doesn't depend on any naming conventions.  For a summary of the
 *	command-line options, run untar with no arguments.
 *
 * HOW TO COMPILE:
 *	Untar doesn't require any special libraries or compile-time flags.
 *	A simple "cc untar.c -o untar" (or the local equivalent) is
 *	sufficient.  Even "make untar" works, without needing a Makefile.
 *	For Microsoft Visual C++, the command is "cl /D_WEAK_POSIX untar.c"
 *	(for 32 bit compilers) or "cl /F 1400 untar.c" (for 16-bit).
 *
 *	IF YOU SEE COMPILER WARNINGS, THAT'S NORMAL; you can ignore them.
 *	Most of the warnings could be eliminated by adding #include <string.h>
 *	but that isn't portable -- some systems require <strings.h> and
 *	<malloc.h>, for example.  Because <string.h> isn't quite portable,
 *	and isn't really necessary in the context of this program, it isn't
 *	included.
 *
 * PORTABILITY:
 *	Untar only requires the <stdio.h> header.  It uses old-style function
 *	definitions.  It opens all files in binary mode.  Taken together,
 *	this means that untar should compile & run on just about anything.
 *
 *	If your system supports the POSIX chmod(2), utime(2), link(2), and
 *	symlink(2) calls, then you may wish to compile with -D_POSIX_SOURCE,
 *	which will enable untar to use those system calls to restore the
 *	timestamp and permissions of the extracted files, and restore links.
 *	(For Linux, _POSIX_SOURCE is always defined.)
 *
 *	For systems which support some POSIX features but not enough to support
 *	-D_POSIX_SOURCE, you might be able to use -D_WEAK_POSIX.  This allows
 *	untar to restore time stamps and file permissions, but not links.
 *	This should work for Microsoft systems, and hopefully others as well.
 *
 * AUTHOR & COPYRIGHT INFO:
 *  Changed to let Turbo C++ v1.01 compile the source by Steffen Kaiser
 *  still in public domain, 07 October 2004
 *
 *	Written by Steve Kirkendall, kirkenda@cs.pdx.edu
 *	Placed in public domain, 6 October 1995
 *
 *	Portions derived from inflate.c -- Not copyrighted 1992 by Mark Adler
 *	version c10p1, 10 January 1993
 */

#ifdef __TURBOC__
/* Turn of those noisy warnings */
#pragma warn -nod
#pragma warn -pro
#pragma warn -cln
#endif

#include <stdio.h>
#ifndef SEEK_SET
# define SEEK_SET 0
#endif

#ifdef _WEAK_POSIX
# ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE
# endif
#endif

#ifdef _POSIX_SOURCE
# include <sys/types.h>
# include <sys/stat.h>
# include <utime.h>
# ifdef _WEAK_POSIX
#  define mode_t int
# else
#  include <unistd.h>
# endif
#endif

#define WSIZE	32768	/* size of decompression buffer */
#define TSIZE	512	/* size of a "tape" block */
#define CR	13	/* carriage-return character */
#define LF	10	/* line-feed character */

typedef unsigned char	Uchar_t;
typedef unsigned short	Ushort_t;
typedef unsigned long	Ulong_t;

typedef struct
{
	Uchar_t	magic[2];	/* magic: 0x1F, 0x8b */
	Uchar_t	compression;	/* compression method: 8=deflated */
	Uchar_t	flags;		/* content flags: 0x08 bit -> name present */
	Uchar_t	mtime[4];	/* time_t when archive created */
	Uchar_t	extraflags;	/* ? */
	Uchar_t	os;		/* operating system: 3=UNIX */
	/* if flags&0x08, then original file name goes here, '\0'-terminated */
} gzhdr_t;
#define MAGIC0	0x1f
#define MAGIC1	0x8b
#define DEFLATE	0x08
#define NAME	0x08

typedef struct
{
	char	filename[100];	/*   0  name of next file */
	char	mode[8];	/* 100  Permissions and type (octal digits) */
	char	owner[8];	/* 108  Owner ID (ignored) */
	char	group[8];	/* 116  Group ID (ignored) */
	char	size[12];	/* 124  Bytes in file (octal digits) */
	char	mtime[12];	/* 136  Modification time stamp (octal digits)*/
	char	checksum[8];	/* 148  Header checksum (ignored) */
	char	type;		/* 156  File type (see below) */
	char	linkto[100];	/* 157  Linked-to name */
	char	brand[8];	/* 257  Identifies tar version (ignored) */
	char	ownername[32];	/* 265  Name of owner (ignored) */
	char	groupname[32];	/* 297  Name of group (ignored) */
	char	devmajor[8];	/* 329  Device major number (ignored) */
	char	defminor[8];	/* 337  Device minor number (ignored) */
	char	prefix[155];	/* 345  Prefix of name (optional) */
	char	RESERVED[12];	/* 500  Pad header size to 512 bytes */
} tar_t;
#define ISREGULAR(hdr)	((hdr).type < '1' || (hdr).type > '6')

typedef struct huft {
	Uchar_t e;	/* number of extra bits or operation */
	Uchar_t b;	/* number of bits in this code or subcode */
	union {
		Ushort_t	n; /* literal, length base, or distance base */
		struct huft	*t;/* pointer to next level of table */
	} v;
} huft_t;

int wp;	/* output counter */
Uchar_t slide[WSIZE];
#define error(desc)	{fprintf(stderr, "%s:%s", inname, (desc)); exit(1);}

/* Tables for deflate from PKZIP's appnote.txt. */
static unsigned border[] = {	/* Order of the bit length code lengths */
	16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
static Ushort_t cplens[] = {	/* Copy lengths for literal codes 257..285 */
	3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
	35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
	/* note: see note #13 above about the 258 in this list. */
static Ushort_t cplext[] = {	/* Extra bits for literal codes 257..285 */
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
	3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99}; /* 99==invalid */
static Ushort_t cpdist[] = {	/* Copy offsets for distance codes 0..29 */
	1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
	257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
	8193, 12289, 16385, 24577};
static Ushort_t cpdext[] = {	 /* Extra bits for distance codes */
	0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
	7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
	12, 12, 13, 13};


char	*inname;/* name of input archive */
FILE	*infp;	/* input byte stream */
FILE	*outfp;	/* output stream, for file currently being extracted */
Ulong_t	outsize;/* number of bytes remainin in file currently being extracted */
FILE	*tarfp;	/* usually NULL; else file for writing gunzipped data */
int	maketar;/* -d: 1 to gunzip only, 0 to gunzip and extract tar files */
int	listing;/* -t: 1 if listing, 0 if extracting */
int	quiet;	/* -q: 1 to write nothing to stdout, 0 for normal chatter */
int	verbose;/* -v: 1 to write extra information to stdout */
int	force;	/* -f: 1 to overwrite existing files, 0 to skip them */
int	abspath;/* -p: 1 to allow leading '/', 0 to strip leading '/' */
int	convert;/* -c: 1 to convert newlines, 0 to leave unchanged */
int	noname;	/* -n: 1 to ignore gzip name, 0 to use gzip name */
char	**only;	/* array of filenames to extract/list */
int	nonlys;	/* number of filenames in "only" array; 0=extract all */
int	didabs;	/* were any filenames affected by the absence of -p? */

Ulong_t bb; 	/* bit buffer */
unsigned bk;   	/* bits in bit buffer */

Ushort_t mask_bits[] = {
		0x0000,
		0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
		0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};

#define NEXTBYTE()  (Uchar_t)getc(infp)
#define NEEDBITS(n) {while(k<(n)){b|=((Ulong_t)NEXTBYTE())<<k;k+=8;}}
#define DUMPBITS(n) {b>>=(n);k-=(n);}

int lbits = 9;	/* bits in base literal/length lookup table */
int dbits = 6;	/* bits in base distance lookup table */


/* If BMAX needs to be larger than 16, then h and x[] should be Ulong_t. */
#define BMAX	16	 /* maximum bit length of any code (16 for explode) */
#define N_MAX	288	 /* maximum number of codes in any set */


unsigned hufts;		 /* track memory usage */

/*----------------------------------------------------------------------------*/

/* create a file for writing.  If necessary, create the directories leading up
 * to that file as well.
 */
static FILE *createpath(name)
	char	*name;	/* pathname of file to create */
{
	FILE	*fp;
	int	i;

	/* if we aren't allowed to overwrite and this file exists, return NULL */
	if (!force && access(name, 0) == 0)
	{
		fprintf(stderr, "%s: exists, will not overwrite without \"-f\"\n", name);
		return NULL;
	}

	/* first try creating it the easy way */
	fp = fopen(name, convert ? "w" : "wb");
	if (fp)
		return fp;

	/* Else try making all of its directories, and then try creating
	 * the file again.
	 */
	for (i = 0; name[i]; i++)
	{
		/* If this is a slash, then temporarily replace the '/'
		 * with a '\0' and do a mkdir() on the resulting string.
		 * Ignore errors for now.
		 */
		if (name[i] == '/')
		{
			name[i] = '\0';
			(void)mkdir(name, 0777);
			name[i] = '/';
		}
	}
	fp = fopen(name, convert ? "w" : "wb");
	if (!fp)
		perror(name);
	return fp;
}

/* Create a link, or copy a file.  If the file is copied (not linked) then
 * give a warning.
 */
static void linkorcopy(src, dst, sym)
	char	*src;	/* name of existing source file */
	char	*dst;	/* name of new destination file */
	int	sym;	/* use symlink instead of link */
{
	FILE	*fpsrc;
	FILE	*fpdst;
	int	c;

	/* Open the source file.  We do this first to make sure it exists */
	fpsrc = fopen(src, "rb");
	if (!fpsrc)
	{
		perror(src);
		return;
	}

	/* Create the destination file.  On POSIX systems, this is just to
	 * make sure the directory path exists.
	 */
	fpdst = createpath(dst);
	if (!fpdst)
		/* error message already given */
		return;

#ifdef _POSIX_SOURCE
# ifndef _WEAK_POSIX
	/* first try to link it over, instead of copying */
	fclose(fpdst);
	unlink(dst);
	if (sym)
	{
		if (symlink(src, dst))
		{
			perror(dst);
		}
		fclose(fpsrc);
		return;
	}
	if (!link(src, dst))
	{
		/* This story had a happy ending */
		fclose(fpsrc);
		return;
	}

	/* Dang.  Reopen the destination again */
	fpdst = fopen(dst, "wb");
	/* This *can't* fail */

# endif /* _WEAK_POSIX */
#endif /* _POSIX_SOURCE */

	/* Copy characters */
	while ((c = getc(fpsrc)) != EOF)
		putc(c, fpdst);

	/* Close the files */
	fclose(fpsrc);
	fclose(fpdst);

	/* Give a warning */
	printf("%s: copy instead of link\n", dst);
}

/* This calls fwrite(), possibly after converting CR-LF to LF */
static void cvtwrite(blk, size, fp)
	Uchar_t	*blk;	/* the block to be written */
	Ulong_t	size;	/* number of characters to be written */
	FILE	*fp;	/* file to write to */
{
	int	i, j;
	static Uchar_t mod[TSIZE];

	if (convert)
	{
		for (i = j = 0; i < size; i++)
		{
			/* convert LF to local newline convention */
			if (blk[i] == LF)
				mod[j++] = '\n';
			/* If CR-LF pair, then delete the CR */
			else if (blk[i] == CR && (i+1 >= size || blk[i+1] == LF))
				;
			/* other characters copied literally */
			else
				mod[j++] = blk[i];
		}
		size = j;
		blk = mod;
	}

	fwrite(blk, (size_t)size, sizeof(Uchar_t), fp);
}


/* Compute the checksum of a tar header block, and return it as a long int.
 * The checksum can be computed using either POSIX rules (unsigned bytes)
 * or Sun rules (signed bytes).
 */
static long checksum(tblk, sunny)
	tar_t	*tblk;	/* buffer containing the tar header block */
	int	sunny;	/* Boolean: Sun-style checksums? (else POSIX) */
{
	long	sum;
	char	*scan;

	/* compute the sum of the first 148 bytes -- everything up to but not
	 * including the checksum field itself.
	 */
	sum = 0L;
	for (scan = (char *)tblk; scan < tblk->checksum; scan++)
	{
		sum += (*scan) & 0xff;
		if (sunny && (*scan & 0x80) != 0)
			sum -= 256;
	}

	/* for the 8 bytes of the checksum field, add blanks to the sum */
	sum += ' ' * sizeof tblk->checksum;
	scan += sizeof tblk->checksum;

	/* finish counting the sum of the rest of the block */
	for (; scan < (char *)tblk + sizeof *tblk; scan++)
	{
		sum += (*scan) & 0xff;
		if (sunny && (*scan & 0x80) != 0)
			sum -= 256;
	}

	return sum;
}



/* list files in an archive, and optionally extract them as well */
static void untar(blk)
	Uchar_t	*blk;	/* a tape block */
{
	static char	nbuf[256];/* storage space for prefix+name, combined */
	static char	*name,*n2;/* prefix and name, combined */
	static int	first = 1;/* Boolean: first block of archive? */
	long		sum;	  /* checksum for this block */
	int		i;
	tar_t		tblk[1];

#ifdef _POSIX_SOURCE
	static mode_t		mode;		/* file permissions */
	static struct utimbuf	timestamp;	/* file timestamp */
#endif

	/* make a local copy of the block, and treat it as a tar header */
	tblk[0] = *(tar_t *)blk;

	/* process each type of tape block differently */
	if (outsize > TSIZE)
	{
		/* data block, but not the last one */
		if (outfp)
			cvtwrite(blk, (Ulong_t)TSIZE, outfp);
		outsize -= TSIZE;
	}
	else if (outsize > 0)
	{
		/* last data block of current file */
		if (outfp)
		{
			cvtwrite(blk, outsize, outfp);
			fclose(outfp);
			outfp = NULL;
#ifdef _POSIX_SOURCE
			utime(nbuf, &timestamp);
			chmod(nbuf, mode);
#endif
		}
		outsize = 0;
	}
	else if ((tblk)->filename[0] == '\0')
	{
		/* end-of-archive marker */
		if (didabs)
			fprintf(stderr, "WARNING: Removed leading slashes because \"-p\" wasn't given.\n");
		exit(0);
	}
	else
	{
		/* file header */
	
		/* half-assed verification -- does it look like header? */
		if ((tblk)->filename[99] != '\0'
		 || ((tblk)->size[0] < '0'
			&& (tblk)->size[0] != ' ')
		 || (tblk)->size[0] > '9')
		{
			if (first)
			{
				fprintf(stderr, "%s: not a valid tar file\n", inname);
				exit(2);
			}
			else
			{
				printf("WARNING: Garbage detected; preceding file may be damaged\n");
				exit(2);
			}
		}

		/* combine prefix and filename */
		memset(nbuf, 0, sizeof nbuf);
		name = nbuf;
		if ((tblk)->prefix[0])
		{
			strncpy(name, (tblk)->prefix, sizeof ((tblk)->prefix));
			strcat(name, "/");
			strncat(name + strlen(name), (tblk)->filename,
				sizeof ((tblk)->filename));
		}
		else
		{
			strncpy(name, (tblk)->filename,
				sizeof ((tblk)->filename));
		}

		/* Convert any backslashes to forward slashes, and guard
		 * against doubled-up slashes. (Some DOS versions of "tar"
		 * get this wrong.)  Also strip off leading slashes.
		 */
		if (!abspath && (*name == '/' || *name == '\\'))
			didabs = 1;
		for (n2 = nbuf; *name; name++)
		{
			if (*name == '\\')
				*name = '/';
			if (*name != '/'
			 || (abspath && n2 == nbuf)
			 || (n2 != nbuf && n2[-1] != '/'))
				*n2++ = *name;
		}
		if (n2 == nbuf)
			*n2++ = '/';
		*n2 = '\0';

		/* verify the checksum */
		for (sum = 0L, i = 0; i < sizeof((tblk)->checksum); i++)
		{
			if ((tblk)->checksum[i] >= '0'
						&& (tblk)->checksum[i] <= '7')
				sum = sum * 8 + (tblk)->checksum[i] - '0';
		}
		if (sum != checksum(tblk, 0) && sum != checksum(tblk, 1))
		{
			if (!first)
				printf("WARNING: Garbage detected; preceding file may be damaged\n");
			fflush(stdout);
			fprintf(stderr, "%s: header has bad checksum for %s\n", inname, nbuf);
			exit(2);
		}

		/* From this point on, we don't care whether this is the first
		 * block or not.  Might as well reset the "first" flag now.
		 */
		first = 0;

		/* if last character of name is '/' then assume directory */
		if (*nbuf && nbuf[strlen(nbuf) - 1] == '/')
			(tblk)->type = '5';

		/* convert file size */
		for (outsize = 0L, i = 0; i < sizeof((tblk)->size); i++)
		{
			if ((tblk)->size[i] >= '0' && (tblk)->size[i] <= '7')
				outsize = outsize * 8 + (tblk)->size[i] - '0';
		}

#ifdef _POSIX_SOURCE
		/* convert file timestamp */
		for (timestamp.modtime=0L, i=0; i < sizeof((tblk)->mtime); i++)
		{
			if ((tblk)->mtime[i] >= '0' && (tblk)->mtime[i] <= '7')
				timestamp.modtime = timestamp.modtime * 8
						+ (tblk)->mtime[i] - '0';
		}
		timestamp.actime = timestamp.modtime;

		/* convert file permissions */
		for (mode = i = 0; i < sizeof((tblk)->mode); i++)
		{
			if ((tblk)->mode[i] >= '0' && (tblk)->mode[i] <= '7')
				mode = mode * 8 + (tblk)->mode[i] - '0';
		}
#endif

		/* If we have an "only" list, and this file isn't in it,
		 * then skip it.
		 */
		if (nonlys > 0)
		{
			for (i = 0;
			     i < nonlys
				&& strcmp(only[i], nbuf)
				&& (strncmp(only[i], nbuf, strlen(only[i]))
					|| nbuf[strlen(only[i])] != '/');
				i++)
			{
			}
			if (i >= nonlys)
			{
				outfp = NULL;
				return;
			}
		}

		/* list the file */
		if (verbose)
			printf("%c %s",
				ISREGULAR(*tblk) ? '-' : ("hlcbdp"[(tblk)->type - '1']),
				nbuf);
		else if (!quiet)
			printf("%s\n", nbuf);

		/* if link, then do the link-or-copy thing */
		if (tblk->type == '1' || tblk->type == '2')
		{
			if (verbose)
				printf(" -> %s\n", tblk->linkto);
			if (!listing)
				linkorcopy(tblk->linkto, nbuf, tblk->type == '2');
			outsize = 0L;
			return;
		}

		/* If directory, then make a weak attempt to create it.
		 * Ideally we would do the "create path" thing, but that
		 * seems like more trouble than it's worth since traditional
		 * tar archives don't contain directories anyway.
		 */
		if (tblk->type == '5')
		{
			if (listing)
				n2 = " directory";
#ifdef _POSIX_SOURCE
			else if (mkdir(nbuf, mode) == 0)
#else
			else if (mkdir(nbuf, 0755) == 0)
#endif
				n2 = " created";
			else
				n2 = " ignored";
			if (verbose)
				printf("%s\n", n2);
			return;
		}

		/* if not a regular file, then skip it */
		if (!ISREGULAR(*tblk))
		{
			if (verbose)
				printf(" ignored\n");
			outsize = 0L;
			return;
		}

		/* print file statistics */
		if (verbose)
		{
			printf(" (%ld byte%s, %ld tape block%s)\n",
				outsize,
				outsize == 1 ? "" : "s",
				(outsize + TSIZE - 1) / TSIZE,
				(outsize > 0  && outsize <= TSIZE) ? "" : "s");
		}

		/* if extracting, then try to create the file */
		if (!listing)
			outfp = createpath(nbuf);
		else
			outfp = NULL;

		/* if file is 0 bytes long, then we're done already! */
		if (outsize == 0 && outfp)
		{
			fclose(outfp);
#ifdef _POSIX_SOURCE
			utime(nbuf, &timestamp);
			chmod(nbuf, mode);
#endif
		}
	}
}

/* send decompressed tape blocks to untar() */
void flush_output(w)
	unsigned	w;	/* number of bytes in slide[] */
{
	unsigned	i;

	if (tarfp)
	{
		cvtwrite(slide, (Ulong_t)w, tarfp);
	}
	else
	{
		/* send each block to untar() */
		for (i = 0; i + TSIZE <= w; i += TSIZE)
		{
			untar(&slide[i]);
		}
	}
}

/*----------------------------------------------------------------------------*/

/* Given a list of code lengths and a maximum table size, make a set of
 * tables to decode that set of codes.	Return zero on success, one if
 * the given code set is incomplete (the tables are still built in this
 * case), two if the input is invalid (all zero length codes or an
 * oversubscribed set of lengths), and three if not enough memory.
 */
int huft_build(b, n, s, d, e, t, m)
	unsigned	*b;	/* code lengths in bits (all assumed <= BMAX) */
	unsigned	n;	/* number of codes (assumed <= N_MAX) */
	unsigned	s;	/* number of simple-valued codes (0..s-1) */
	Ushort_t	*d;	/* list of base values for non-simple codes */
	Ushort_t	*e;	/* list of extra bits for non-simple codes */
	huft_t		**t;	/* result: starting table */
	int		*m;	/* maximum lookup bits, returns actual */
{
	unsigned	  a;		/* counter for codes of length k */
	unsigned	  c[BMAX+1];	/* bit length count table */
	unsigned	  f;		/* i repeats in table every f entries */
	int		  g;		/* maximum code length */
	int		  h;		/* table level */
	register unsigned i;		/* counter, current code */
	register unsigned j;		/* counter */
	register int	  k;		/* number of bits in current code */
	int		  l;		/* bits per table (returned in m) */
	register unsigned *p;		/* pointer into c[], b[], or v[] */
	register huft_t   *q;		/* points to current table */
	huft_t		  r;		/* table entry for structure assignment */
	huft_t		  *u[BMAX];	/* table stack */
	unsigned	  v[N_MAX];	/* values in order of bit length */
	register int	  w;		/* bits before this table == (l * h) */
	unsigned	  x[BMAX+1];	/* bit offsets, then code stack */
	unsigned	  *xp;		/* pointer into x */
	int		  y;		/* number of dummy codes added */
	unsigned	  z;		/* number of entries in current table */


	/* Generate counts for each bit length */
	memset(c, 0, sizeof(c));
	p = b;	i = n;
	do {
		c[*p++]++;	/* assume all entries <= BMAX */
	} while (--i);
	if (c[0] == n)		/* null input--all zero length codes */
	{
		*t = (huft_t *)NULL;
		*m = 0;
		return 0;
	}


	/* Find minimum and maximum length, bound *m by those */
	l = *m;
	for (j = 1; j <= BMAX; j++)
		if (c[j])
			break;
	k = j;		/* minimum code length */
	if ((unsigned)l < j)
		l = j;
	for (i = BMAX; i; i--)
		if (c[i])
			break;
	g = i;		/* maximum code length */
	if ((unsigned)l > i)
		l = i;
	*m = l;


	/* Adjust last length count to fill out codes, if needed */
	for (y = 1 << j; j < i; j++, y <<= 1)
		if ((y -= c[j]) < 0)
			return 2;	/* bad input: more codes than bits */
	if ((y -= c[i]) < 0)
		return 2;
	c[i] += y;


	/* Generate starting offsets into the value table for each length */
	x[1] = j = 0;
	p = c + 1;	xp = x + 2;
	while (--i) {			 /* note that i == g from above */
		*xp++ = (j += *p++);
	}

	/* Make a table of values in order of bit lengths */
	p = b;	i = 0;
	do {
		if ((j = *p++) != 0)
			v[x[j]++] = i;
	} while (++i < n);


	/* Generate the Huffman codes and for each, make the table entries */
	x[0] = i = 0;	/* first Huffman code is zero */
	p = v;		/* grab values in bit order */
	h = -1;		/* no tables yet--level -1 */
	w = -l;		/* bits decoded == (l * h) */
	u[0] = (huft_t *)NULL;	/* just to keep compilers happy */
	q = (huft_t *)NULL;	/* ditto */
	z = 0;			/* ditto */

	/* go through the bit lengths (k already is bits in shortest code) */
	for (; k <= g; k++)
	{
		a = c[k];
		while (a--)
		{
			/* here i is the Huffman code of length k bits for value *p */
			/* make tables up to required level */
			while (k > w + l)
			{
				h++;
				w += l;	/* previous table always l bits */

				/* compute minimum size table less than or equal to l bits */
				z = (z = g - w) > (unsigned)l ? l : z;	/* upper limit on table size */
				if ((f = 1 << (j = k - w)) > a + 1)	/* try a k-w bit table */
				{											 /* too few codes for k-w bit table */
					f -= a + 1;	 /* deduct codes from patterns left */
					xp = c + k;
					while (++j < z)	 /* try smaller tables up to z bits */
					{
						if ((f <<= 1) <= *++xp)
							break;						/* enough codes to use up j bits */
						f -= *xp;	/* else deduct codes from patterns */
					}
				}
				z = 1 << j;						 /* table entries for j-bit table */

				/* allocate and link in new table */
				q = (huft_t *)malloc((z + 1)*sizeof(huft_t));
				hufts += z + 1;	 /* track memory usage */
				*t = q + 1;						 /* link to list for huft_free() */
				*(t = &(q->v.t)) = (huft_t *)NULL;
				u[h] = ++q;						 /* table starts after link */

				/* connect to last table, if there is one */
				if (h)
				{
					x[h] = i;		/* save pattern for backing up */
					r.b = (Uchar_t)l;	/* bits to dump before this table */
					r.e = (Uchar_t)(16 + j);/* bits in this table */
					r.v.t = q;		/* pointer to this table */
					j = i >> (w - l);	/* (get around Turbo C bug) */
					u[h-1][j] = r;		/* connect to last table */
				}
			}

			/* set up table entry in r */
			r.b = (Uchar_t)(k - w);
			if (p >= v + n)
				r.e = 99;/* out of values--invalid code */
			else if (*p < s)
			{
				r.e = (Uchar_t)(*p < 256 ? 16 : 15);/* 256 is end-of-block code */
				r.v.n = *p++;			 /* simple code is just the value */
			}
			else
			{
				r.e = (Uchar_t)e[*p - s];	 /* non-simple--look up in lists */
				r.v.n = d[*p++ - s];
			}

			/* fill code-like entries with r */
			f = 1 << (k - w);
			for (j = i >> w; j < z; j += f)
				q[j] = r;

			/* backwards increment the k-bit code i */
			for (j = 1 << (k - 1); i & j; j >>= 1)
				i ^= j;
			i ^= j;

			/* backup over finished tables */
			while ((i & ((1 << w) - 1)) != x[h])
			{
				h--;		/* don't need to update q */
				w -= l;
			}
		}
	}


	/* Return true (1) if we were given an incomplete table */
	return y != 0 && g != 1;
}



/* Free the malloc'ed tables built by huft_build(), which makes a linked
 * list of the tables it made, with the links in a dummy first entry of
 * each table.
 */
int huft_free(t)
	huft_t	*t;	 /* table to free */
{
	register huft_t *p, *q;


	/* Go through linked list, freeing from the malloced (t[-1]) address. */
	p = t;
	while (p != (huft_t *)NULL)
	{
		q = (--p)->v.t;
		free(p);
		p = q;
	} 
	return 0;
}


/* Inflate (decompress) the codes in a deflated (compressed) block.
 * Return an error code or zero if it all goes ok.
 */
int inflate_codes(tl, td, bl, bd)
	huft_t	  *tl, *td;	/* literal/length and distance decoder tables */
	int	  bl, bd;	/* number of bits decoded by tl[] and td[] */
{
	register unsigned e;		/* table entry flag/number of extra bits */
	unsigned	  n, d;		/* length and index for copy */
	unsigned	  w;		/* current window position */
	huft_t		  *t;		/* pointer to table entry */
	unsigned	  ml, md;	/* masks for bl and bd bits */
	register Ulong_t  b;		/* bit buffer */
	register unsigned k;		/* number of bits in bit buffer */


	/* make local copies of globals */
	b = bb;	/* initialize bit buffer */
	k = bk;
	w = wp;	/* initialize window position */

	/* inflate the coded data */
	ml = mask_bits[bl]; /* precompute masks for speed */
	md = mask_bits[bd];
	for (;;)	/* do until end of block */
	{
		NEEDBITS((unsigned)bl)
		if ((e = (t = tl + ((unsigned)b & ml))->e) > 16)
			do
			{
				if (e == 99)
					return 1;
				DUMPBITS(t->b)
				e -= 16;
				NEEDBITS(e)
			} while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
		DUMPBITS(t->b)
		if (e == 16)	/* then it's a literal */
		{
			slide[w++] = (Uchar_t)t->v.n;
			if (w == WSIZE)
			{
				flush_output(w);
				w = 0;
			}
		}
		else	/* it's an EOB or a length */
		{
			/* exit if end of block */
			if (e == 15)
				break;

			/* get length of block to copy */
			NEEDBITS(e)
			n = t->v.n + ((unsigned)b & mask_bits[e]);
			DUMPBITS(e);

			/* decode distance of block to copy */
			NEEDBITS((unsigned)bd)
			if ((e = (t = td + ((unsigned)b & md))->e) > 16)
				do
				{
					if (e == 99)
						return 1;
					DUMPBITS(t->b)
					e -= 16;
					NEEDBITS(e)
				} while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
			DUMPBITS(t->b)
			NEEDBITS(e)
			d = w - t->v.n - ((unsigned)b & mask_bits[e]);
			DUMPBITS(e)

			/* do the copy */
			do
			{
				n -= (e = (e = WSIZE - ((d &= WSIZE-1) > w ? d : w)) > n ? n : e);
#if !defined(NOMEMCPY) && !defined(DEBUG)
				if (w - d >= e)	 /* (this test assumes unsigned comparison) */
				{
					memcpy(slide + w, slide + d, e);
					w += e;
					d += e;
				}
				else	/* do it slow to avoid memcpy() overlap */
#endif /* !NOMEMCPY */
					do
					{
						slide[w++] = slide[d++];
					} while (--e);
				if (w == WSIZE)
				{
					flush_output(w);
					w = 0;
				}
			} while (n);
		}
	}

	/* restore the globals from the locals */
	wp = w;	/* restore global window pointer */
	bb = b;	/* restore global bit buffer */
	bk = k;

	/* done */
	return 0;
}



/* "decompress" an inflated type 0 (stored) block. */
int inflate_stored()
{
	unsigned	  n;	/* number of bytes in block */
	unsigned	  w;	/* current window position */
	register Ulong_t  b;	/* bit buffer */
	register unsigned k;	/* number of bits in bit buffer */

	/* make local copies of globals */
	b = bb;	/* initialize bit buffer */
	k = bk;
	w = wp;	/* initialize window position */

	/* go to byte boundary */
	n = k & 7;
	DUMPBITS(n);

	/* get the length and its complement */
	NEEDBITS(16)
	n = ((unsigned)b & 0xffff);
	DUMPBITS(16)
	NEEDBITS(16)
	if (n != (unsigned)((~b) & 0xffff))
		return 1;	 /* error in compressed data */
	DUMPBITS(16)

	/* read and output the compressed data */
	while (n--)
	{
		NEEDBITS(8)
		slide[w++] = (Uchar_t)b;
		if (w == WSIZE)
		{
			flush_output(w);
			w = 0;
		}
		DUMPBITS(8)
	}


	/* restore the globals from the locals */
	wp = w;	/* restore global window pointer */
	bb = b;	/* restore global bit buffer */
	bk = k;

	return 0;
}

/* Decompress an inflated type 1 (fixed Huffman codes) block.	We should
 * either replace this with a custom decoder, or at least precompute the
 * Huffman tables.
 */
int inflate_fixed()
{
	int		i;	/* temporary variable */
	huft_t		*tl;	/* literal/length code table */
	huft_t		*td;	/* distance code table */
	int		bl;	/* lookup bits for tl */
	int		bd;	/* lookup bits for td */
	unsigned	l[288];	/* length list for huft_build */


	/* set up literal table */
	for (i = 0; i < 144; i++)
		l[i] = 8;
	for (; i < 256; i++)
		l[i] = 9;
	for (; i < 280; i++)
		l[i] = 7;
	for (; i < 288; i++)	/* make a complete, but wrong code set */
		l[i] = 8;
	bl = 7;
	if ((i = huft_build(l, 288, 257, cplens, cplext, &tl, &bl)) != 0)
		return i;

	/* set up distance table */
	for (i = 0; i < 30; i++)	/* make an incomplete code set */
		l[i] = 5;
	bd = 5;
	if ((i = huft_build(l, 30, 0, cpdist, cpdext, &td, &bd)) > 1)
	{
		huft_free(tl);

		return i;
	}

	/* decompress until an end-of-block code */
	if (inflate_codes(tl, td, bl, bd))
		return 1;

	/* free the decoding tables, return */
	huft_free(tl);
	huft_free(td);
	return 0;
}



/* decompress an inflated type 2 (dynamic Huffman codes) block. */
int inflate_dynamic()
{
	int		i;	   /* temporary variables */
	unsigned	j;
	unsigned	l;	   /* last length */
	unsigned	m;	   /* mask for bit lengths table */
	unsigned	n;	   /* number of lengths to get */
	huft_t		*tl;	   /* literal/length code table */
	huft_t		*td;	   /* distance code table */
	int		bl;	   /* lookup bits for tl */
	int		bd;	   /* lookup bits for td */
	unsigned	nb;	   /* number of bit length codes */
	unsigned	nl;	   /* number of literal/length codes */
	unsigned	nd;	   /* number of distance codes */
	unsigned	ll[286+30];/* literal/length and distance code lengths */
	register Ulong_t b;	   /* bit buffer */
	register unsigned k;	   /* number of bits in bit buffer */


	/* make local bit buffer */
	b = bb;
	k = bk;

	/* read in table lengths */
	NEEDBITS(5)
	nl = 257 + ((unsigned)b & 0x1f);/* number of literal/length codes */
	DUMPBITS(5)
	NEEDBITS(5)
	nd = 1 + ((unsigned)b & 0x1f);	/* number of distance codes */
	DUMPBITS(5)
	NEEDBITS(4)
	nb = 4 + ((unsigned)b & 0xf);	/* number of bit length codes */
	DUMPBITS(4)
	if (nl > 286 || nd > 30)
		return 1;		/* bad lengths */

	/* read in bit-length-code lengths */
	for (j = 0; j < nb; j++)
	{
		NEEDBITS(3)
		ll[border[j]] = (unsigned)b & 7;
		DUMPBITS(3)
	}
	for (; j < 19; j++)
		ll[border[j]] = 0;

	/* build decoding table for trees--single level, 7 bit lookup */
	bl = 7;
	if ((i = huft_build(ll, 19, 19, NULL, NULL, &tl, &bl)) != 0)
	{
		if (i == 1)
			huft_free(tl);
		return i;		/* incomplete code set */
	}

	/* read in literal and distance code lengths */
	n = nl + nd;
	m = mask_bits[bl];
	i = l = 0;
	while ((unsigned)i < n)
	{
		NEEDBITS((unsigned)bl)
		j = (td = tl + ((unsigned)b & m))->b;
		DUMPBITS(j)
		j = td->v.n;
		if (j < 16)		/* length of code in bits (0..15) */
			ll[i++] = l = j;/* save last length in l */
		else if (j == 16)	/* repeat last length 3 to 6 times */
		{
			NEEDBITS(2)
			j = 3 + ((unsigned)b & 3);
			DUMPBITS(2)
			if ((unsigned)i + j > n)
				return 1;
			while (j--)
				ll[i++] = l;
		}
		else if (j == 17) /* 3 to 10 zero length codes */
		{
			NEEDBITS(3)
			j = 3 + ((unsigned)b & 7);
			DUMPBITS(3)
			if ((unsigned)i + j > n)
				return 1;
			while (j--)
				ll[i++] = 0;
			l = 0;
		}
		else /* j == 18: 11 to 138 zero length codes */
		{
			NEEDBITS(7)
			j = 11 + ((unsigned)b & 0x7f);
			DUMPBITS(7)
			if ((unsigned)i + j > n)
				return 1;
			while (j--)
				ll[i++] = 0;
			l = 0;
		}
	}


	/* free decoding table for trees */
	huft_free(tl);

	/* restore the global bit buffer */
	bb = b;
	bk = k;

	/* build the decoding tables for literal/length and distance codes */
	bl = lbits;
	if ((i = huft_build(ll, nl, 257, cplens, cplext, &tl, &bl)) != 0)
	{
		if (i == 1) {
			error(" incomplete literal tree\n");
			huft_free(tl);
		}
		return i;	/* incomplete code set */
	}
	bd = dbits;
	if ((i = huft_build(ll + nl, nd, 0, cpdist, cpdext, &td, &bd)) != 0)
	{
		if (i == 1) {
			error(" incomplete distance tree\n");
			huft_free(td);
		}
		huft_free(tl);
		return i;	/* incomplete code set */
	}

	/* decompress until an end-of-block code */
	if (inflate_codes(tl, td, bl, bd))
		return 1;

	/* free the decoding tables, return */
	huft_free(tl);
	huft_free(td);

	return 0;
}



/* decompress an inflated block */
int inflate_block(e)
	int			*e;	/* last block flag */
{
	unsigned		t;	/* block type */
	register Ulong_t	b;	/* bit buffer */
	register unsigned	k;	/* number of bits in bit buffer */

	/* make local bit buffer */
	b = bb;
	k = bk;

	/* read in last block bit */
	NEEDBITS(1)
	*e = (int)b & 1;
	DUMPBITS(1)

	/* read in block type */
	NEEDBITS(2)
	t = (unsigned)b & 3;
	DUMPBITS(2)

	/* restore the global bit buffer */
	bb = b;
	bk = k;

	/* inflate that block type */
	if (t == 2)
		return inflate_dynamic();
	if (t == 0)
		return inflate_stored();
	if (t == 1)
		return inflate_fixed();

	/* bad block type */
	return 2;
}



/* decompress an inflated entry */
int inflate()
{
	int	e;	/* last block flag */
	int	r;	/* result code */
	unsigned h;	/* maximum huft_t's malloc'ed */


	/* initialize window, bit buffer */
	wp = 0;
	bk = 0;
	bb = 0;

	/* decompress until the last block */
	h = 0;
	do
	{
		hufts = 0;
		if ((r = inflate_block(&e)) != 0)
			return r;
		if (hufts > h)
			h = hufts;
	} while (!e);

	/* Undo too much lookahead. The next read will be byte aligned so we
	 * can discard unused bits in the last meaningful byte.
	 */
	while (bk >= 8)
	{
		bk -= 8;
		/* inptr--;*/
	}

	/* flush out slide */
	flush_output(wp);

	/* return success */
	return 0;
}


/* Process an archive file.  This involves reading the blocks one at a time
 * (decompressing if necessary as it goes along) and passing them to a untar()
 * function.
 */
static void doarchive(filename)
	char	*filename;	/* name of the archive file */
{
	char	gunzipname[300];
	int	ch, len;

	/* open the archive */
	inname = filename;
	infp = fopen(filename, "rb");
	if (!infp)
	{
		perror(filename);
		return;
	}

	/* read the first few bytes, so we can determine whether to decompress */
	fread(slide, 1, sizeof(gzhdr_t), infp);
	if (((gzhdr_t *)slide)->magic[0] == MAGIC0
	 && ((gzhdr_t *)slide)->magic[1] == MAGIC1)
	{
		/* COMPRESSED WITH GZIP */

		/* Check for unsupported compression types */
		if (((gzhdr_t *)slide)->compression != DEFLATE)
		{
			fprintf(stderr, "Unsupported compression type\n");
			exit(1);
		}

		/* If original file name present, use it (unless noname) */
		if (!noname && (((gzhdr_t *)slide)->flags & NAME) != 0)
		{
			for (len = 0; (ch = getc(infp)) != '\0'; len++)
			{
				gunzipname[len] = ch;
			}
			gunzipname[len] = '\0';
		}
		else if (maketar)
		{
			/* skip the original file name, if any */
			if ((((gzhdr_t *)slide)->flags & NAME) != 0)
				while ((ch = getc(infp)) != '\0')
				{
				}

			/* we need to make up a name */
			strcpy(gunzipname, filename);
			len = strlen(filename);
			if (len > 3 && (!strcmp(filename + len - 3, ".gz")
					|| !strcmp(filename + len - 3, ".GZ")))
			{
				gunzipname[len - 3] = '\0';
			}
			else if (len > 2 && (!strcmp(filename + len - 2, ".z")
					|| !strcmp(filename + len - 2, ".Z")))
			{
				gunzipname[len - 2] = '\0';
			}
			else if (len > 4 && (!strcmp(filename + len - 4, ".tgz")
					|| !strcmp(filename + len - 4, ".TGZ")))
			{
				strcpy(&gunzipname[len - 4], ".tar");
			}
			else
			{
				strcpy(gunzipname, "untar.out");
			}
		}

		/* if we're writing the gunzip output, then create the output file */
		if (maketar)
		{
			if (!quiet && listing)
			{
				printf("%s: would be gunzipped to %s\n", filename, gunzipname);
				fclose(infp);
				return;
			}

			/* if not allowed to overwrite and file exists, complain */
			if (!force && access(gunzipname, 0) == 0)
			{
				fprintf(stderr, "%s: exists, will not overwrite without \"-f\"\n", gunzipname);
				exit(2);
			}
			tarfp = fopen(gunzipname, convert ? "w" : "wb");
			if (!tarfp)
			{
				perror(gunzipname);
				exit(2);
			}
		}

		/* inflate the blocks */
		if (inflate() != 0)
		{
			fprintf(stderr, "%s: bad compression data\n", filename);
			exit(2);
		}
	}
	else
	{
		/* UNCOMPRESSED */

		/* if we were supposed to just decompress, complain */
		if (maketar)
		{
			fprintf(stderr, "%s: isn't gzipped\n", filename);
			fclose(infp);
			return;
		}

		/* read the rest of the first block */
		fread(&slide[sizeof(gzhdr_t)], 1, TSIZE - sizeof(gzhdr_t), infp);

		/* send each block to the untar() function */
		do
		{
			untar(slide);
		} while (fread(slide, 1, TSIZE, infp) == TSIZE);
	}

	/* close the archive file. */
	fclose(infp);
	if (tarfp)
	{
		fclose(tarfp);
		tarfp = NULL;
		if (!quiet)
		{
			printf("%s: gunzipped to %s\n", filename, gunzipname);
		}
	}
	if (outsize > 0)
	{
		printf("WARNING: Last file might be truncated!\n");
		fclose(outfp);
		outfp = NULL;
	}
}

static void usage(argv0, exitcode)
	char	*argv0;	/* name of program */
	int	exitcode;/* exit status -- 0 for success, non-0 for failure */
{
	/* Give a usage message and exit */
	printf("Usage: %s [options] archive.tgz [filename] ...\n", argv0);
	printf("   or: %s [options] -d filename.gz ...\n", argv0);
	printf("\n");
	printf("Options: -t   Test -- list contents but don't extract\n");
	printf("         -f   Force -- allow existing files to be overwritten\n");
	printf("         -q   Quiet -- suppress the normal chatter\n");
	printf("         -v   Verbose -- output extra information about each file\n");
	printf("         -p   Path -- allow absolute pathnames (don't strip leading '/')\n");
	printf("         -c   Convert -- convert files to local text format\n");
	printf("         -d   Decompress -- perform \"gunzip\" but not \"tar x\"\n");
	printf("         -n   No-name -- with \"-d\", ignore original name in gzip header\n");
	printf("\n");
	printf("This program lists/extracts files from a \"*.tar\" or \"*.tgz\" archive.  You can\n");
	printf("optionally specify certain files or directories to list/extract; otherwise it\n");
#ifdef _POSIX_SOURCE
# ifdef _WEAK_POSIX
	printf("will list/extract them all.  File attributes are preserved fairly well, but\n");
	printf("linked files are restored via COPYING.  This program can also be used (with -d)\n");
	printf("to gunzip non-tar files.\n");
# else /* not _WEAK_POSIX */
	printf("will list/extract them all.  File attributes are preserved, and linked files\n");
	printf("will be restored as links.  This program can also be used (with -d) to gunzip\n");
	printf("non-tar files.\n");
# endif /* not _WEAK_POSIX */
#else /* not _POSIX_SOURCE */
	printf("will list/extract them all.  File attributes are NOT preserved.  Linked files\n");
	printf("will be restored via COPYING.  This program can also be used (with -d) to\n");
	printf("gunzip non-tar files.\n");
#endif /* not _POSIX_SOURCE */
	printf("\n");
	printf("THIS PROGRAM IS IN THE PUBLIC DOMAIN, AND IS FREELY REDISTRIBUTABLE.\n");
	printf("Report bugs to kirkenda@cs.pdx.edu\n");
	exit(exitcode);
}

/* parse command-line arguments, and process each file */
int main(argc, argv)
	int	argc;
	char	**argv;
{
	int	i, j;

	/* GNUish flags */
	if (argc < 2 || (argc == 2 && !strcmp(argv[1], "--help")))
		usage(argv[0], 0);
	if (argc == 2 && !strcmp(argv[1], "--version"))
	{
		printf("untar %s\n", VERSION);
		printf("Placed in public domain by the author, Steve Kirkendall\n");
		exit(0);
	}

	/* listing? extracting? */
	for (i = 1; i < argc && argv[i][0] == '-'; i++)
	{
		if (!argv[i][1])
			usage(argv[0], 2);
		for (j = 1; argv[i][j]; j++)
		{
			switch (argv[i][j])
			{
			  case 'd':	maketar = 1;	break;
			  case 'n':	noname = 1;	break;
			  case 't':	listing = 1;	break;
			  case 'f':	force = 1;	break;
			  case 'q':	quiet = 1;	break;
			  case 'c':	convert = 1;	break;
			  case 'p':	abspath = 1;	break;
			  case 'v':	verbose = 1;	break;
			  default:
				usage(argv[0], 2);
			}
		}
	}
	if (i >= argc)
	{
		usage(argv[0], 2);
	}

	if (maketar)
	{
		/* decompress each argument */
		for (; i < argc; i++)
		{
			doarchive(argv[i]);
		}
	}
	else
	{
		/* detect whether we have an extraction list */
		if (i + 1 < argc)
		{
			only = &argv[i + 1];
			nonlys = argc - (i + 1);
		}
		else
		{
			nonlys = 0;
		}
	
		/* list/extract files from archive */
		doarchive(argv[i]);
	}

	if (didabs)
		fprintf(stderr, "WARNING: Removed leading slashes because \"-p\" wasn't given.\n");
	exit(0);
}
