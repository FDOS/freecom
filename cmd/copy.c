/* $Id$
 * COPY.C -- Internal Copy Command
 *
 * 1999/05/10 ska
 * rewritten, based upon previous COPY.C of FreeCom v0.76b
 *
 * Known bugs:
 *  + Multiple '+' plus signs are scanned as a single one.
 *
 * 1999/07/08 ska
 * bugfix: destination is a drive letter only
 *
 * 2000/07/17 Ron Cemer
 * bugfix: destination ending in "\\" must be a directory, but fails
 *	within dfnstat()
 *
 * 2000/07/24 Ron Cemer
 * bugfix: Suppress "Overwrite..." prompt if destination is device
 *
 * 2001/02/17 ska
 * add: interactive command flag
 
 * bugfix: copy 1 + 2 + 3 <-> only first and last file is stored
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>
#include <io.h>
#include <fcntl.h>
#include <limits.h>

/*#define DEBUG*/

#include "dfn.h"
#include "suppl.h"
#include "supplio.h"

#include "../include/lfnfuncs.h"
#include "../include/command.h"
#include "../include/cmdline.h"
#include "../err_fcts.h"
#include "../include/misc.h"
#include "../strings.h"
#include "../include/openf.h"

#define ASCII 1
#define BINARY 2
#define IS_DIRECTORY 5

static struct CopySource {
  struct CopySource *nxt;   /* next source */
  struct CopySource *app;   /* list of files to append */
  int flags;          /* ASCII / Binary */
  char *fnam;         /* filename */
} *head, *last, *lastApp;


static int appendToFile; /* Append the next file rather than new source */
static char *destFile;     /* destination file/directory/pattern */

static int optY, optV, optA, optB;

optScanFct(opt_copy)
{
  (void)arg;
  switch(ch) {
  case 'Y': return optScanBool(optY);
  case 'V': return optScanBool(optV);
  case 'A': case 'B': return E_Ignore;
  }
  optErr();
  return E_Useage;
}

optScanFct(opt_copy1)
{
  int ec, *opt, *optReset;

  (void)arg;
  switch(ch) {
#ifndef NDEBUG
  default:
    fprintf(stderr, "Invalid option: file '%s' in line %d\n"
      , __FILE__, __LINE__);
    abort();
#endif
  case 'A': opt = &optA; optReset = &optB; break;
  case 'B': opt = &optB; optReset = &optA; break;
  }
  if((ec = optScanBool(*opt)) == E_None
   && *opt)
    *optReset = 0;

  return ec;
}


static void initContext(void)
{
  appendToFile = 0;
  last = lastApp = 0;
}

static void killContext(void)
{
  if(last) {
    assert(head);
    do {
      if((lastApp = head->app) != 0) do {
        lastApp = (last = lastApp)->app;
        free(last);
      } while(lastApp);
      head = (last = head)->nxt;
      free(last);
    } while(head);
  }
}

/*
	faster copy, using large (far) buffers
*/

#define DOSread(x, y, z) DOSreadwrite( x, y, z, 0x3F00 )
#define DOSwrite(x, y, z) DOSreadwrite( x, y, z, 0x4000 )

unsigned DOSreadwrite(int fd, void far *buffer, unsigned size,
                             unsigned short func )
{
	struct REGPACK r;

	r.r_ax = func;
	r.r_bx = fd;
	r.r_cx = size;
	r.r_dx = FP_OFF(buffer);
    r.r_ds = FP_SEG(buffer);
	intr(0x21, &r);
    return( ( r.r_flags & 1 ) ? 0xFFFF : r.r_ax );
}


/*
	a) this copies data, using a 60K buffer
	b) if transfer is slow (or on a huge file),
	   indicate some progress
*/

static int BIGcopy(FILE *fout, FILE *fin)
{
	int fdin  = fileno(fin);
	int fdout = fileno(fout);

	char far *buffer;
	unsigned size;
	unsigned rd;
	int retval = 0;
								/* stat stuff */
	unsigned startTime, lastTime=0, now, doStat = 0;
	unsigned long copied = 0, toCopy = filelength(fdin);
	char *statString;
	
	
	/* Fetch the largest available buffer */
	for(size = 60*1024u; size != 0; size -= 4*1024) {
		buffer = (void _seg*)DOSalloc(size/16,0);
		if(buffer != NULL)
			goto ok;
	}
	return 3;	/* out of memory error */

ok:
	dprintf( ("[MEM: BIGcopy() allocate %u bytes @ 0x%04x]\n"
	 , size, FP_SEG(buffer)) );
	statString = getString(isadev(fdin)
		? TEXT_COPY_COPIED_NO_END
		: TEXT_COPY_COPIED);
	startTime = *(unsigned far *)MK_FP(0x40,0x6c);

	while((rd = DOSread(fdin, buffer, size)) != 0) {
		if(rd == 0xffff) {
			retval = 1;
			goto _exit;
		}
		
		if(DOSwrite(fdout, buffer, rd) != rd) {
			retval = 2;
			goto _exit;
		}
			
						/* statistics */
		copied += rd;	
			
		now = *(unsigned far *)MK_FP(0x40,0x6c);
		
		if(!doStat
		 && now - startTime > 15 * 18
		 && isatty(fileno(stdout)))
			doStat = TRUE;
		
		if(now - lastTime > 18) {
			if(doStat)
				printf(statString, copied/1024, toCopy/1024);
				
			if(cbreak) {
				retval = 3;
				goto _exit;
			}	
				
			lastTime = now;
		}
	}	
		
_exit:		
	if(doStat)
		printf("%30s\r","");
		
	dprintf( ("[MEM: BIGcopy() release memory @ 0x%04x]\n"
	 , FP_SEG(buffer)) );
	DOSfree(FP_SEG(buffer));
	free(statString);
	return retval;
}


static int copy(char *dst, char *pattern, struct CopySource *src
  , int openMode)
{ char mode[3], *p;
  struct ffblk ff;
  struct CopySource *h;
  char rDest[MAXPATH], rSrc[MAXPATH];
  FILE *fin, *fout;
  int rc, asc;
  char *buf;
  size_t len;
  FLAG keepFTime;
  struct ftime fileTime;
  char *srcFile;
  FLAG wildcarded;
  /*FLAG isfirst = 1;*/
  FLAG singleFileCopy = src->app == NULL;

  assert(dst);
  assert(pattern);
  assert(src);

  if(strpbrk(pattern, "*?") == 0) {
  	srcFile = dfnfilename(pattern);
  	wildcarded = 0;
  } else if(FINDFIRST(pattern, &ff, FA_RDONLY | FA_ARCH) != 0) {
    error_sfile_not_found(pattern);
    return 0;
  } else {
  	srcFile = ff.ff_name;
  	wildcarded = 1;
  }

  mode[2] = '\0';

  do {
/*    if( wildcarded && !strpbrk( dst, "*?" ) && !isfirst ) openMode = 'a'; */
    fillFnam(rDest, dst, srcFile);
    if(rDest[0] == 0)
      return 0;
    h = src;
    do {  /* to prevent to open a source file for writing, e.g.
          for COPY *.c *.?    */
      fillFnam(rSrc, h->fnam, srcFile);
      if(rSrc[0] == 0) {
        return 0;
      }
      rc = samefile(rDest, rSrc);
      if(rc < 0) {
        error_out_of_memory();
        return 0;
      } else if(rc) {
        error_selfcopy(rDest);
        return 0;
      }
    } while((h = h->app) != 0);

    if(interactive_command		/* Suppress prompt if in batch file */
       && openMode != 'a' && !optY && (fout = fopen(rDest, "rb")) != 0) {
    	int destIsDevice = isadev(fileno(fout));

      fclose(fout);
      if(!destIsDevice) {	/* Devices do always exist */
        if( dfnstat( rSrc ) == 0) { /* Source doesn't exist */
            error_open_file( rSrc );
            return 0;
        } else {
          	switch(userprompt(PROMPT_OVERWRITE_FILE, rDest)) {
	    	default:	/* Error */
		    case 4:	/* Quit */
	    		  return 0;
		    case 3:	/* All */
			    optY = 1;
    		case 1: /* Yes */
	    		break;
		    case 2:	/* No */
    			continue;
    		}
        }
	  }
    }
    if(cbreak) {
      return 0;
    }
    mode[0] = openMode;
    mode[1] = 'b';
    if((fout = fdevopen(rDest, mode)) == 0) {
      error_open_file(rDest);
      return 0;
    }
    mode[0] = 'r';
    h = src;
    keepFTime = (h->app == 0);
    do {
      fillFnam(rSrc, h->fnam, srcFile);
      if(rSrc[0] == 0) {
        fclose(fout);
        unlink(rDest);		/* if device -> no removal, ignore error */
        return 0;
      }
      mode[1] = (asc = h->flags & ASCII) != 0? 't': 'b';
    reOpenIn:
      if((fin = fdevopen(rSrc, mode)) == 0) {
        error_open_file(rSrc);
        fclose(fout);
        unlink(rDest);		/* if device -> no removal, ignore error */
        return 0;
      }
      if(isadev(fileno(fin))) {
		keepFTime = 0;		/* Cannot keep file time of devices */
      	if(mode[1] != 't'					/* in binary mode currently */
		   && (h->flags & BINARY) == 0) {	/* no forced binary mode */
			/* character devices are opened in textmode by default */
			fclose(fin);
			mode[1] = 't';
			goto reOpenIn;
		  }
      }
      if(keepFTime)
        if(getftime(fileno(fin) , &fileTime))
          keepFTime = 0; /* if error: turn it off */

      displayString(TEXT_MSG_COPYING, rSrc
	   , (openMode == 'a' || h != src)? "=>>": "=>", rDest);
      if(cbreak) {
        fclose(fin);
        fclose(fout);
        unlink(rDest);		/* if device -> no removal, ignore error */
        return 0;
      }

      /* Now copy the file */
      rc = 1;
      if(mode[1] != 't') {    /* binary file */
      	FLAG sizeChanged = singleFileCopy && !isadev(fileno(fin))
      	 && !isadev(fileno(fout));
        if(sizeChanged) {	/* faster copy, *MUCH* faster on floppies
								 change destination filesize to wanted size.
								 this a) writes all required entries to the
								 FAT (faster) determines, if there is enough
								 space on the destination device
								 no need to copy file, if it won't fit */
        					/* No test if chsize() fails for MS DOS 5/6 bug
        						see RBIL DOS-40 */
        					/* Don't use chsize() as Turbo RTL fills with
        						'\0' bytes, which is not useful here */
        	lseek(fileno(fout), filelength(fileno(fin)), SEEK_SET);
        	if(truncate(fileno(fout)) != 0
        	 || lseek(fileno(fout), 0, SEEK_SET) == -1) {
				error_write_file_disc_full(rDest, filelength(fileno(fin)));
        		rc = 0;
			} else {
				dprintf( ("[COPY chsize(%s, %lu)]\n", rDest,
				 filelength(fileno(fin))) );
			}
		}
      
        if(rc != 0)
			switch(BIGcopy(fout, fin)) {
			case 0: 
				if(sizeChanged)
					/* probably the source file got truncated */
					/* we silently ignore any failure here, because it is
						assumed that we never extend, but truncate the file
						only (or do not change the length at all) */
					truncate(fileno(fout));
				break;
			case 1:  error_read_file(rSrc);   rc = 0; break;
			case 2:  error_write_file(rDest); rc = 0; break;
			default: error_copy();            rc = 0; break;
			}
      } else {      /* text file, manually transform '\n' */
        if(Fmaxbuf((byte**)&buf, &len) == 0) {
          if(len > INT_MAX)
            len = INT_MAX;
          while(fgets(buf, len, fin)) {
            p = strchr(buf, '\0');
            if(*--p == '\n') {
              *p = '\0';
              fputs(buf, fout);
              putc('\r', fout);
              putc('\n', fout);
            } else
              fputs(buf, fout);
          }
          free(buf);
        } else {
          error_out_of_memory();
          rc = 0;
        }
      }
      if(rc)
        if(ferror(fin)) {
          error_read_file(rSrc);
          rc = 0;
        } else if(ferror(fout)) {
          error_write_file(rDest);
          rc = 0;
        }
      if(cbreak)
        rc = 0;
      fclose(fin);
      if(!rc) {
        fclose(fout);
        unlink(rDest);		/* if device -> no removal, ignore error */
        return 0;
      }
    } while((h = h->app) != 0);
    if(asc) {   /* append the ^Z as we copied in ASCII mode */
      putc(0x1a, fout);
    }
    fflush(fout);
    if(keepFTime)
      setftime(fileno(fout), &fileTime);
    rc = ferror(fout);
    fclose(fout);
    if(rc) {
      error_write_file(rDest);
      unlink(rDest);		/* if device -> no removal, ignore error */
      return 0;
    }
  } while(wildcarded && FINDNEXT(&ff) == 0);
  /*} while(wildcarded && FINDNEXT(&ff) == 0 && !(isfirst = 0)); */

  FINDSTOP(&ff);

  return 1;
}

static int copyFiles(struct CopySource *h)
{ int differ, rc;

  rc = 0;

#define dst destFile
  if((differ = samefile(h->fnam, dst)) < 0)
    error_out_of_memory();
  else if(!differ)
    rc = copy(dst, h->fnam, h, 'w');
  else if(h->app)
    rc = copy(dst, h->fnam, h->app, 'a');
  else
    error_selfcopy(dst);
#undef dst

  return rc;
}

static int cpyFlags(void)
{
  return (optA? ASCII: 0) | (optB? BINARY: 0);
}

static struct CopySource *srcItem(char *fnam)
{	struct CopySource *h;

    if((h = malloc(sizeof(struct CopySource))) == 0) {
      error_out_of_memory();
      return 0;
    }

    h->fnam = fnam;
    h->nxt = h->app = 0;
    h->flags = cpyFlags();

    return h;
}

static int addSource(char *p)
{ struct CopySource *h;
  char *q;

  assert(p);
  q = strtok(p, "+");
  assert(q && *q);

  if(appendToFile) {
    appendToFile = 0;
    if(!lastApp) {
      error_leading_plus();
      return 0;
    }
  } else {      /* New entry */
    if(0 == (h = srcItem(q)))
      return 0;
    if(!last)
      last = lastApp = head = h;
    else
      last = lastApp = last->nxt = h;

    if((q = strtok(0, "+")) == 0)   /* no to-append file */
      return 1;
  }

  /* all the next files are to be appended to the source in "last" */
  assert(q);
  assert(lastApp);
  do {
    if(0 == (h = srcItem(q)))
      return 0;
    lastApp = lastApp->app = h;
  } while((q = strtok(0, "+")) != 0);

  return 1;
}


int cmd_copy(char *rest)
{ char **argv, *p;
  int argc, opts, argi;
  struct CopySource *h;
  char **argBuffer = 0;

  /* Initialize options */
  optA = optB = optV = optY = 0;

  /* read the parameters from env */
  if ((argv = scanCmdline(getEnv("COPYCMD"), opt_copy, 0, &argc, &opts))
   == 0)
    return 1;
  freep(argv);    /* ignore any parameter from env var */

  if((argv = scanCmdline(rest, opt_copy, 0, &argc, &opts)) == 0)
    return 1;

  /* scan the trailing '/a' and '/b' options */
  while(argc > 0 && isoption(argv[argc - 1])) {
    p = argv[--argc];     /* argv[] must not be changed */
    if(leadOptions(&p, opt_copy1, 0) != E_None) {
      freep(argv);
      return 1;
    }
  }

  initContext();

  /* Now parse the remaining arguments into the copy file
    structure */
  for(argi = 0; argi < argc; ++argi)
    if(isoption(p = argv[argi])) {    /* infix /a or /b */
      if(leadOptions(&p, opt_copy1, 0) != E_None) {
        killContext();
        freep(argv);
        return 1;
      }
      /* Change the flags of the previous argument */
      if(lastApp)
        lastApp->flags = cpyFlags();
    } else {            /* real argument */
      if(*p == '+') {       /* to previous argument */
        appendToFile = 1;
        while(*++p == '+');
        if(!*p)
          continue;
      }

      if(!addSource(p)) {
        killContext();
        freep(argv);
        return 1;
      }

    }

  if(appendToFile) {
    error_trailing_plus();
    killContext();
    freep(argv);
    return 1;
  }

  if(!last) {   /* Nothing to do */
    error_nothing_to_do();
    killContext();
    freep(argv);
    return 1;
  }

  assert(head);

  /* Check whether the source items are files or directories */
  h = head;
  argc = 0;		/* argBuffer entries */
  do {
	struct CopySource *p = h;
  	do {
  		char *s = strchr(p->fnam, '\0') - 1;
  		if(*s == '/' || *s == '\\'		/* forcedly be directory */
  		 || 0 != (dfnstat(p->fnam) & DFN_DIRECTORY)) {
			char **buf;
			char *q;
			if(*s == ':') 
				q = dfnmerge(0, p->fnam, 0, "*", "*");
			else
				q = dfnmerge(0, 0, p->fnam, "*", "*");
			if(0 == (buf = realloc(argBuffer, (argc + 2) * sizeof(char*)))
			 || !q) {
				free(q);
				error_out_of_memory();
				goto errRet;
			}
			argBuffer = buf;
			buf[argc] = p->fnam = q;
			buf[++argc] = 0;
  		} else if(*s == ':') {		/* Device name?? */
  			if(!isDeviceName(p->fnam)) {
				error_invalid_parameter(p->fnam);
				goto errRet;
			}
  		}
  	} while(0 != (p = p->app));
  } while(0 != (h = h->nxt));

	if(last != head) {
		/* The last argument is to be the destination */
		if(last->app) {	/* last argument is a + b syntax -> no dst! */
			error_copy_plus_destination();
			goto errRet;
		}
		destFile = last->fnam;
		h = head;         /* remove it from argument list */
		while(h->nxt != last) {
		  assert(h->nxt);
		  h = h->nxt;
		}
		free(last);
		(last = h)->nxt = 0;
  } else {              /* Nay */
    destFile = ".\\*.*";
  }

  /* Now copy the files */
  h = head;
  while(copyFiles(h) && (h = h->nxt) != 0);

errRet:
  killContext();
  freep(argv);
  freep(argBuffer);
  return 0;
}
