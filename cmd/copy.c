/*
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

#include <dfn.h>
#include <supplio.h>

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
// static int destIsDir;      /* destination is directory */

static int optY, optV, optA, optB;

#pragma argsused
optScanFct(opt_copy)
{
  switch(ch) {
  case 'Y': return optScanBool(optY);
  case 'V': return optScanBool(optV);
  case 'A': case 'B': return E_Ignore;
  }
  optErr();
  return E_Useage;
}
#pragma argsused
optScanFct(opt_copy1)
{ int ec, *opt, *optReset;

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

static int copy(char *dst, char *pattern, struct CopySource *src
  , int openMode)
{ char mode[3], *p;
  struct ffblk ff;
  struct CopySource *h;
  char *rDest, *rSrc;
  FILE *fin, *fout;
  int rc, asc;
  char *buf;
  size_t len;
  FLAG keepFTime;
  struct ftime fileTime;
  char *singleFile;

  assert(dst);
  assert(pattern);
  assert(src);

  if(strpbrk(pattern, "*?") == 0) {
  	singleFile = dfnfilename(pattern);
  } else if(FINDFIRST(pattern, &ff, FA_RDONLY | FA_ARCH) != 0) {
    error_sfile_not_found(pattern);
    return 0;
  } else singleFile = &ff.ff_name;

  mode[2] = '\0';

  do {
    if((rDest = fillFnam(dst, singleFile)) == 0)
      return 0;
    h = src;
    do {  /* to prevent to open a source file for writing, e.g.
          for COPY *.c *.?    */
      if((rSrc = fillFnam(h->fnam, singleFile)) == 0) {
        free(rDest);
        return 0;
      }
      rc = samefile(rDest, rSrc);
      free(rSrc);
      if(rc < 0) {
        error_out_of_memory();
        free(rDest);
        return 0;
      } else if(rc) {
        error_selfcopy(rDest);
        free(rDest);
        return 0;
      }
    } while((h = h->app) != 0);

    if(interactive_command		/* Suppress prompt if in batch file */
     && openMode != 'a' && !optY && (fout = fopen(rDest, "rb")) != 0) {
    	int destIsDevice = isadev(fileno(fout));

      fclose(fout);
      if(!destIsDevice) {	/* Devices do always exist */
      	switch(userprompt(PROMPT_OVERWRITE_FILE, rDest)) {
		default:	/* Error */
		case 4:	/* Quit */
			  free(rDest);
			  return 0;
		case 3:	/* All */
			optY = 1;
		case 1: /* Yes */
			break;
		case 2:	/* No */
			free(rDest);
			continue;
		}
	  }
    }
    if(cbreak) {
      free(rDest);
      return 0;
    }
    mode[0] = openMode;
    mode[1] = 'b';
    if((fout = fdevopen(rDest, mode)) == 0) {
      error_open_file(rDest);
      free(rDest);
      return 0;
    }
    mode[0] = 'r';
    h = src;
    keepFTime = (h->app == 0);
    do {
      if((rSrc = fillFnam(h->fnam, singleFile)) == 0) {
        fclose(fout);
        free(rDest);
        return 0;
      }
      mode[1] = (asc = h->flags & ASCII) != 0? 't': 'b';
    reOpenIn:
      if((fin = fdevopen(rSrc, mode)) == 0) {
        error_open_file(rSrc);
        fclose(fout);
        free(rSrc);
        free(rDest);
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
        free(rSrc);
        free(rDest);
        return 0;
      }

      /* Now copy the file */
      rc = 1;
      if(mode[1] != 't') {    /* binary file */
        if(Fcopy(fout, fin) != 0) {
          if(ferror(fin)) {
            error_read_file(rSrc);
          } else if(ferror(fout)) {
            error_write_file(rDest);
          } else error_copy();
          rc = 0;
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
      free(rSrc);
      if(!rc) {
        fclose(fout);
        free(rDest);
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
      free(rDest);
      return 0;
    }
    free(rDest);
  } while(!singleFile && FINDNEXT(&ff) == 0);

  return 1;
}

static int copyFiles(struct CopySource *h)
{ //char *fnam, *ext, *dst;
  int differ, rc;

#if 0
  if(destIsDir) {
    if(!dfnsplit(h->fnam, 0, 0, &fnam, &ext)) {
      error_out_of_memory();
      return 0;
    }
    dst = dfnmerge(0, 0, destFile, fnam, ext);
    free(fnam);
    free(ext);
    if(!dst) {
      error_out_of_memory();
      return 0;
    }
  } else
    dst = destFile;
#endif

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

#if 0
  if(destIsDir)
    free(dst);
#endif
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
//  int freeDestFile = 0;
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
			if(0 == (buf = realloc(argBuffer, (argc + 1) * sizeof(char*)))
			 || !q) {
				free(q);
				error_out_of_memory();
				goto errRet1;
			}
			argBuffer = buf;
			buf[argc] = p->fnam = q;
			buf[++argc] = 0;
//			p->flags |= IS_DIRECTORY;
  		} else if(*s == ':') {		/* Device name?? */
  			if(!isDeviceName(p->fnam)) {
				error_invalid_parameter(p->fnam);
				goto errRet1;
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
#if 0
  /* Now test if a destination was specified */
  if(head != last && !last->app) {  /* Yeah */
    destFile = dfnexpand(last->fnam, 0);
    if(!destFile) {
      error_out_of_memory();
      goto errRet;
    }
    freeDestFile = 1;
    h = head;         /* remove it from argument list */
    while(h->nxt != last) {
      assert(h->nxt);
      h = h->nxt;
    }
    free(last);
    (last = h)->nxt = 0;
    p = strchr(destFile, '\0') - 1;
    if(*p == '\\' || *p == '/')		/* must be a directory */
    	destIsDir = 1;
    else destIsDir = dfnstat(destFile) & DFN_DIRECTORY;
#endif
  } else {              /* Nay */
    destFile = ".\\*.*";
//    destIsDir = 1;
  }

  /* Now copy the files */
  h = head;
  while(copyFiles(h) && (h = h->nxt) != 0);

errRet1:
#if 0
  if(freeDestFile)
    free(destFile);
#endif
errRet:
  killContext();
  freep(argv);
  freep(argBuffer);
  return 0;
}
