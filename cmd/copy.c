/*
 * COPY.C -- Internal Copy Command
 *
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

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"
#include "../include/misc.h"
#include "../strings.h"
#include "../include/openf.h"

#define ASCII 1
#define BINARY 2

struct CopySource {
  struct CopySource *nxt;   /* next source */
  struct CopySource *app;   /* list of files to append */
  int flags;          /* ASCII / Binary */
  char *fnam;         /* filename */
} *head, *last, *lastApp;


extern int FAR appFile; /* Append the next file rather than new source */
char *destFile;     /* destination file/directory/pattern */
int destIsDir;      /* destination is directory */

static FLAG optY, optV, optA, optB;

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


void initContext(void)
{
  appFile = 0;
  last = lastApp = 0;
}

void killContext(void)
{
  if(last) {
    assert(head);
    do {
      if((lastApp = head->app) != 0) do {
        lastApp = (last = lastApp)->app;
        myfree(last);
      } while(lastApp);
      head = (last = head)->nxt;
      myfree(last);
    } while(head);
  }
}

int copy(char *dst, char *pattern, struct CopySource *src
  , int openMode)
{ char mode[3], *p;
  struct ffblk ff;
  struct CopySource *h;
  char *rDest, *rSrc;
  FILE *fin, *fout;
  int rc, asc;
  char *buf;
  size_t len;

  assert(dst);
  assert(pattern);
  assert(src);

  if(findfirst(pattern, &ff, FA_RDONLY | FA_ARCH) != 0) {
    if((fin = fopen(pattern, "rb")) == 0) {
		error_sfile_not_found(pattern);
		return 0;
	}
	fclose(fin);
  } else
  	pattern = ff.ff_name;

  mode[2] = '\0';

  do {
    if((rDest = fillFnam(dst, pattern)) == 0)
      return 0;
    h = src;
    do {  /* to prevent to open a source file for writing, e.g.
          for COPY *.c *.?    */
      if((rSrc = fillFnam(h->fnam, pattern)) == 0) {
        myfree(rDest);
        return 0;
      }
      rc = samefile(rDest, rSrc);
      myfree(rSrc);
      if(rc < 0) {
        error_out_of_memory();
        myfree(rDest);
        return 0;
      } else if(rc) {
        error_selfcopy(rDest);
        myfree(rDest);
        return 0;
      }
    } while((h = h->app) != 0);

    if(lflag_interactive		/* Suppress prompt if in batch file */
     && openMode != 'a' && !optY && (fout = fopen(rDest, "rb")) != 0) {
    	int destIsDevice = isadev(fileno(fout));

      fclose(fout);
      if(!destIsDevice) {	/* Devices do always exist */
      	switch(userprompt(PROMPT_OVERWRITE_FILE, rDest)) {
		default:	/* Error */
		case 4:	/* Quit */
			  myfree(rDest);
			  return 0;
		case 3:	/* All */
			optY = 1;
		case 1: /* Yes */
			break;
		case 2:	/* No */
			myfree(rDest);
			continue;
		}
	  }
    }

  	rc = E_Operation;		/* Default error code */
    mode[0] = openMode;
    mode[1] = 'b';
    if((fout = fdevopen(rDest, mode)) == 0) {
      error_open_file(rDest);
      goto errRet;
    }
    mode[0] = 'r';
    asc = src->flags & ASCII;
    assert(asc == 0 || asc == 1);
    h = src;
    do {
      if((rSrc = fillFnam(h->fnam, pattern)) == 0) {
		goto errRet;
      }
      mode[1] = (h->flags & ASCII) != 0? 't': 'b';
    reOpenIn:
      if((fin = fdevopen(rSrc, mode)) == 0) {
        error_open_file(rSrc);
        myfree(rSrc);
		goto errRet;
      }
      if(isadev(fileno(fin)) && mode[1] != 't'
       && (h->flags & BINARY) == 0) {
        /* character devices are opened in textmode
          by default */
        fclose(fin);
        mode[1] = 't';
        if(h == src)
        	asc = ASCII;
        goto reOpenIn;
      }

      dispCopy(rSrc, rDest, openMode == 'a' || h != src);

      /* Now copy the file */
		rc = E_None;
		if(Fmaxbuf((byte**)&buf, &len) == 0) {
			int rlen;

			if(len > INT_MAX)
				len = INT_MAX;
			while(rc == E_None && (rlen = fread(p = buf, 1, len, fin)) > 0)
				if(cbreak)		rc = E_CBreak;
				else {
					if(asc) {		/* Output is text */
						char *q;

						++rlen;
						while(--rlen && (q = memchr(p, '\n', rlen)) != 0) {
							int wlen;

							if((wlen = q - p) != 0) {
								if(wlen != fwrite(p, 1, wlen, fout))
									goto writeError;
								rlen -= wlen;
							}
							p = q + 1;
							if(2 != fwrite("\r\n", 1, 2, fout))
								goto writeError;
						}
					}
					if(!rlen || rlen == fwrite(p, 1, rlen, fout))
						continue;
writeError:
					error_write_file(rDest);
					rc = E_Operation;
				}
			myfree(buf);
		} else {
			error_out_of_memory();
			rc = E_NoMem;
		}
		if(rc == E_None) {
			fflush(fout);
			if(ferror(fin)) {
				error_read_file(rSrc);
				rc = E_Operation;
			} else if(ferror(fout)) {
				error_write_file(rDest);
				rc = E_Operation;
			}
		}
      fclose(fin);
      myfree(rSrc);
    } while(rc == E_None && (h = h->app) != 0);
    if(rc == E_None) {
		if(asc) {   /* append the ^Z as we copied in ASCII mode */
			putc(0x1a, fout);
			fflush(fout);
			if(ferror(fout)) {
				error_write_file(rDest);
				rc = E_Operation;
			}
		}
	}
errRet:
    fclose(fout);
    if(rc != E_None) {
    	/* Error message already displayed */
#ifdef DEBUG
		if(rc == E_CBreak)
			dprintf(("[COPY: Terminated on ^Break]\n"));
		else
			dprintf(("[COPY: Erroreous termination]\n"));
#endif

		unlink(rDest);		/* remove the erroreous file */
		myfree(rDest);
		return 0;
    }
	myfree(rDest);
  } while(pattern == ff.ff_name && findnext(&ff) == 0);

  return 1;
}

int copyFiles(struct CopySource *h)
{ char *fnam, *ext, *dst;
  int differ, rc;

  if(destIsDir) {
    if(!dfnsplit(h->fnam, 0, 0, &fnam, &ext)) {
      error_out_of_memory();
      return 0;
    }
    dst = dfnmerge(0, 0, destFile, fnam, ext);
    myfree(fnam);
    myfree(ext);
    if(!dst) {
      error_out_of_memory();
      return 0;
    }
  } else {
  	assert(destFile);
    dst = destFile;
  }

  rc = 0;

  if((differ = samefile(h->fnam, dst)) < 0)
    error_out_of_memory();
  else if(!differ)
    rc = copy(dst, h->fnam, h, 'w');
  else if(h->app)
    rc = copy(dst, h->fnam, h->app, 'a');
  else
    error_selfcopy(dst);

  if(destIsDir)
    myfree(dst);
  return rc;
}

int cpyFlags(void)
{
  if(optA)
  	return ASCII;
  if(optB)
  	return BINARY;
  return 0;
}

static struct CopySource *mkSourceItem(char *q)
{ struct CopySource *h;

    if((h = emalloc(sizeof(struct CopySource))) == 0)
      return 0;
    isDeviceName(h->fnam = q);
    h->flags = cpyFlags();
    h->nxt = h->app = 0;

    return h;
}

static int addSource(char *p)
{ struct CopySource *h;
  char *q;

  assert(p);
  q = strtok(p, "+");
  assert(q && *q);

  if(appFile) {
    appFile = 0;
    if(!lastApp) {
      error_leading_plus();
      return 0;
    }
  } else {      /* New entry */
  	if((h = mkSourceItem(q)) == 0)
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
  	if((lastApp = lastApp->app = mkSourceItem(q)) == 0)
  		return 0;
  } while((q = strtok(0, "+")) != 0);

  return 1;
}


int cmd_copy(char *rest)
{ char **argv, *p;
  int argc, opts, argi;
  struct CopySource *h;
#ifdef DEBUG
	char *tst;
#define _ tst = 
#else
#define _
#endif

  /* Initialize options */
  optA = optB = optV = optY = 0;

  /* read the parameters from env */
  if ((argv = scanCmdline(_ getEnv("COPYCMD"), opt_copy, 0, &argc, &opts))
   == 0)
    return 1;
  chkRegStr(tst);
  freep(argv);    /* ignore any parameter from env var */

  if((argv = scanCmdline(rest, opt_copy, 0, &argc, &opts)) == 0)
    return 1;

  /* scan the trailing '/a' and '/b' option */
  if(isoption(argv[argc - 1])) {
    p = argv[argc - 1];     /* argv[] must not be changed */
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
        appFile = 1;
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

  if(appFile) {
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

  /* Now test if a destination was specified */
  if(head != last && !last->app) {  /* Yeah */
    destFile = dfnexpand(last->fnam, 0);
    if(!destFile) {
      error_out_of_memory();
      goto errRet;
    }
    h = head;         /* remove it from argument list */
    while(h->nxt != last) {
      assert(h->nxt);
      h = h->nxt;
    }
    myfree(last);
    (last = h)->nxt = 0;
    p = strchr(destFile, '\0') - 1;
    if(*p == '\\' || *p == '/')		/* must be a directory */
    	destIsDir = 1;
    else destIsDir = dfnstat(destFile) & DFN_DIRECTORY;
  } else {              /* Nay */
    destFile = 0;
    destIsDir = 1;
  }

  /* Now copy the files */
  h = head;
  while(copyFiles(h) && (h = h->nxt) != 0);

    myfree(destFile);
errRet:
  killContext();
  freep(argv);
  return 0;
}
