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
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>

#include <portable.h>

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
static int destFlags;

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
  int ec, *opt = NULL, *optReset = NULL;

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

/*
	a) this copies data, using a 60K buffer
	b) if transfer is slow (or on a huge file),
	   indicate some progress
*/

static int BIGcopy(int fdout, int fdin, int asc)
{
	char far *buffer;
	unsigned size;
	unsigned rd;
	int retval = 0;
								/* stat stuff */
	unsigned startTime, lastTime=0, now, doStat = 0, deviceIn;
	unsigned long copied = 0, toCopy = filelength(fdin);
	char *statString;
	char far *ctrlz;
	
	
	/* Fetch the largest available buffer */
	for(size = 60*1024u; size != 0; size -= 4*1024) {
#ifdef FARDATA
		/* use last-fit allocation to work well with large model */
		buffer = MK_SEG_PTR(void, DOSalloc(size/16,2));
#else
		buffer = MK_SEG_PTR(void, DOSalloc(size/16,0));
#endif
		if(buffer != NULL)
			goto ok;
	}
	return 3;	/* out of memory error */

ok:
	dprintf( ("[MEM: BIGcopy() allocate %u bytes @ 0x%04x]\n"
	 , size, FP_SEG(buffer)) );
	deviceIn = isadev(fdin);
	statString = getString(deviceIn
		? TEXT_COPY_COPIED_NO_END
		: TEXT_COPY_COPIED);
	startTime = *(unsigned far *)MK_FP(0x40,0x6c);

	ctrlz = 0;
	while((rd = farread(fdin, buffer, size)) != 0) {
		if(rd == 0xffff) {
			retval = 1;
			goto _exit;
		}

		if(asc) {
			ctrlz = _fmemchr(buffer, 0x1a, rd);
			if(ctrlz != 0)
				rd = (unsigned)(ctrlz - buffer);
		}
		
		if(farwrite(fdout, buffer, rd) != rd) {
			if(!isadev(fdout)) retval = 2;
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
		if(ctrlz || (rd < size && !(deviceIn && asc))) break;
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

static int is_valid_disk(int tstdsk)
{
  int savdsk = getdisk();
  int newdsk;

  /* Change to new disk */
  setdisk(tstdsk);
  newdsk = getdisk();

  /* Restore */
  setdisk(savdsk);

  return (newdsk == tstdsk);
}

static int copy(char *dst, char *pattern, struct CopySource *src
  , int openMode)
{ struct dos_ffblk ff;
  struct CopySource *h;
  char rDest[MAXPATH], rSrc[MAXPATH];
  int fdin, fdout;
  int rc;
  FLAG keepFTime;
#if defined(__WATCOMC__) && __WATCOMC__ < 1280
  unsigned short date, time;
#elif defined(__TURBOC__)
  struct ftime fileTime;
#else
  unsigned date, time;
#endif
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
  } else if(dos_findfirst(pattern, &ff, FA_RDONLY | FA_ARCH) != 0) {
    error_sfile_not_found(pattern);
    return 0;
  } else {
  	srcFile = ff.ff_name;
  	wildcarded = 1;
  }

  do {
/*    if( wildcarded && !strpbrk( dst, "*?" ) && !isfirst ) openMode = O_APPEND; */
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

    /* Concenation of files uses ASCII by default */
    if(src->app) {
      for(h = src; h && !h->flags; h = h->app)
        h->flags = ASCII;
      if(!destFlags) destFlags = ASCII;
    }

    if(interactive_command		/* Suppress prompt if in batch file */
       && openMode != O_APPEND && !optY
       && (fdout = dos_open(rDest, O_RDONLY)) >= 0) {
    	int destIsDevice = isadev(fdout);

      dos_close(fdout);
      if(!destIsDevice) {	/* Devices do always exist */
        if((fdin = devopen(rSrc, O_RDONLY)) < 0) { /* Source doesn't exist */
            error_open_file( rSrc );
            return 0;
        } else {
	        dos_close(fdin);
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
    if((fdout = devopen(rDest, openMode)) < 0) {
      error_open_file(rDest);
      return 0;
    }
    keepFTime = 1;
    if(isadev(fdout)) {
      if(destFlags & BINARY)  {
        /* in forced binary mode character devices are set to raw */
        fdsetattr(fdout, (fdattr(fdout) & 0xff) | 0x20);
      }
      keepFTime = 0;
    }
    h = src;
    keepFTime = (keepFTime && h->app == 0);
    do {
      fillFnam(rSrc, h->fnam, srcFile);
      if(rSrc[0] == 0) {
        dos_close(fdout);
        unlink(rDest);		/* if device -> no removal, ignore error */
        return 0;
      }
      if((fdin = devopen(rSrc, O_RDONLY)) < 0) {
        error_open_file(rSrc);
        dos_close(fdout);
        unlink(rDest);		/* if device -> no removal, ignore error */
        return 0;
      }
      if(isadev(fdin)) {
		keepFTime = 0;		/* Cannot keep file time of devices */
      	if(h->flags & BINARY)
		  /* in forced binary mode character devices are set to raw */
		  fdsetattr(fdin, (fdattr(fdin) & 0xff) | 0x20);
      	else
		  /* make sure to stop at Ctrl-Z */
		  h->flags |= ASCII;
      }
      if(keepFTime)
#ifdef __TURBOC__
        if(getftime(fdin , &fileTime))
#else
        if(_dos_getftime(fdin , &date , &time))
#endif
          keepFTime = 0; /* if error: turn it off */

      displayString(TEXT_MSG_COPYING, rSrc
	   , (openMode == 'a' || h != src)? "=>>": "=>", rDest);
      if(cbreak) {
        dos_close(fdin);
        dos_close(fdout);
        unlink(rDest);		/* if device -> no removal, ignore error */
        return 0;
      }

      /* Now copy the file */
      rc = 1;
      {
      	FLAG sizeChanged = !(h->flags & ASCII) && singleFileCopy &&
			!isadev(fdin) && !isadev(fdout);
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
        	lseek(fdout, filelength(fdin), SEEK_SET);
        	if(truncate(fdout) != 0
        	 || lseek(fdout, 0, SEEK_SET) == -1) {
				error_write_file_disc_full(rDest, filelength(fdin));
        		rc = 0;
			} else {
				dprintf( ("[COPY chsize(%s, %lu)]\n", rDest,
				 filelength(fdin)) );
			}
		}
      
        if(rc != 0)
			switch(BIGcopy(fdout, fdin, h->flags & ASCII)) {
			case 0: 
				if(sizeChanged)
					/* probably the source file got truncated */
					/* we silently ignore any failure here, because it is
						assumed that we never extend, but truncate the file
						only (or do not change the length at all) */
					truncate(fdout);
				break;
			case 1:  error_read_file(rSrc);   rc = 0; break;
			case 2:  error_write_file(rDest); rc = 0; break;
			default: error_copy();            rc = 0; break;
			}
      }
      if(cbreak)
        rc = 0;
      dos_close(fdin);
      if(!rc) {
        dos_close(fdout);
        unlink(rDest);		/* if device -> no removal, ignore error */
        return 0;
      }
    } while((h = h->app) != 0);
    rc = 0;
    if((destFlags & ASCII) && !isadev(fdout)) {   /* append the ^Z as we copied in ASCII mode */
      if (dos_write(fdout, "\x1a", 1) != 1)
		rc = 1;
    }
    if(keepFTime)
#ifdef __TURBOC__
      setftime(fdout, &fileTime);
#else
      _dos_setftime(fdout, date, time);
#endif
    if(dos_close(fdout) != 0)
      rc = 1;
    if(rc) {
      error_write_file(rDest);
      unlink(rDest);		/* if device -> no removal, ignore error */
      return 0;
    }
  } while (wildcarded && dos_findnext(&ff) == 0);
  /*} while(wildcarded && FINDNEXT(&ff) == 0 && !(isfirst = 0)); */

  dos_findclose(&ff);

  return 1;
}

static int copyFiles(struct CopySource *h)
{ int differ, rc;

  rc = 0;

#define dst destFile
  if((differ = samefile(h->fnam, dst)) < 0)
    error_out_of_memory();
  else if(!differ)
    rc = copy(dst, h->fnam, h, O_WRONLY|O_TRUNC|O_CREAT);
  else if(h->app)
    rc = copy(dst, h->fnam, h->app, O_WRONLY|O_APPEND);
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
  if ((argv = scanCmdline(p = getEnv("COPYCMD"), opt_copy, 0, &argc, &opts))
   == 0) {
    free(p);
    return 1;
  }
  free(p);
  freep(argv);    /* ignore any parameter from env var */

  if((argv = scanCmdline(rest, opt_copy, 0, &argc, &opts)) == 0)
    return 1;

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
		} else if(*s == ':' && (s - p->fnam) > 1) {		/* Device name LPT1:, but not X: */
  			if(!isDeviceName(p->fnam)) {
				error_invalid_parameter(p->fnam);
				goto errRet;
			}
  		}
  	} while(0 != (p = p->app));
  } while(0 != (h = h->nxt));

  destFlags = last->flags;
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

#define dst destFile
  /* If the destination specifies a drive, check that it is valid */
  if (dst[0] && dst[1] == ':' && !is_valid_disk(dst[0] - 'A')) {
    error_invalid_drive(dst[0] - 'A');
    return 0;
  }
#undef dst

  /* Now copy the files */
  h = head;
  while(copyFiles(h) && (h = h->nxt) != 0);

errRet:
  killContext();
  freep(argv);
  freep(argBuffer);
  return 0;
}
