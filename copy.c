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
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>
#include <io.h>
#include <fcntl.h>
#include <limits.h>

#include "config.h"

#ifdef INCLUDE_CMD_COPY

/*#define DEBUG*/

#include "dfn.h"
#include "supplio.h"

#include "command.h"
#include "cmdline.h"
#include "strings.h"
#include "openf.h"

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


void initContext(void)
{
  appFile = 0;
  last = lastApp = NULL;
}

void killContext(void)
{
  if(last) {
    assert(head);
    do {
      if((lastApp = head->app) != NULL) do {
        lastApp = (last = lastApp)->app;
        free(last);
      } while(lastApp);
      head = (last = head)->nxt;
      free(last);
    } while(head);
  }
}

int samefile(char *f1, char *f2)
{ char *t1, *t2;
  int differ;

  t1 = dfntruename(f1);
  t2 = dfntruename(f2);

  if(!t1 || !t2)
    differ = -1;
  else differ = strcmp(t1, t2) == 0;

  free(t1);
  free(t2);

  return differ;
}

void fillComp(char *dst, char *pattern, char *src, int len)
{ char *s;

  assert(len);
  assert(dst);

  if(!pattern)
    pattern = "";
  if(!src)
    src = "";

  s = dst;
  while(--len) {
    switch(*s = *pattern++) {
    case '\0':
      goto ende;
    case '?':
      if(*src)
        *s = *src;
    default:
      ++s;
      break;
    case '*':
      memcpy(s, src, len);
      s[len] = '\0';
      return;
    }
    if(*src)
      ++src;
  }
ende:
  while(--s >= dst && *s == '?');
  s[1] = '\0';
}

char *fillFnam(char *fnam, char *pattern)
{ char *dr, *pa, *fn, *ex;
  char *pfn, *pex;
  char dfn[MAXFILE], dex[MAXEXT];
  char *p;

  assert(fnam);
  assert(pattern);

  if(strchr(fnam, '?') == NULL && strchr(fnam, '*') == NULL)
    return strdup(fnam);

  if(!dfnsplit(fnam, &dr, &pa, &fn, &ex))
    return NULL;

  if(!dfnsplit(pattern, NULL, NULL, &pfn, &pex)) {
    free(fn);
    free(ex);
    free(dr);
    free(pa);
    return NULL;
  }

  fillComp(dfn, fn, pfn, MAXFILE);
  fillComp(dex, ex, pex, MAXEXT);

  p = dfnmerge(NULL, dr, pa, dfn, dex);

  free(pfn);
  free(pex);
  free(dr);
  free(pa);
  free(fn);
  free(ex);

  return p;
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

  if(FINDFIRST(pattern, &ff, FA_RDONLY | FA_ARCH) != 0) {
    error_sfile_not_found(pattern);
    return 0;
  }

  mode[2] = '\0';

  do {
    if((rDest = fillFnam(dst, ff.ff_name)) == NULL)
      return 0;
    h = src;
    do {  /* to prevent to open a source file for writing, e.g.
          for COPY *.c *.?    */
      if((rSrc = fillFnam(h->fnam, ff.ff_name)) == NULL) {
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
    } while((h = h->app) != NULL);

    if(openMode != 'a' && !optY && (fout = fopen(rDest, "rb")) != NULL) {
    	int destIsDevice = isadev(fileno(fout));

      fclose(fout);
      if(!destIsDevice) {	/* Devices do always exist */
		  printf("Overwrite %s (Yes/No/All)?", rDest);
		  switch (vcgetcstr("YNA"))
		  {
			case CTL_C:
			  free(rDest);
			  return 0;
		  case 'A':
			optY = 1;
		  case 'Y':
			break;
		  default:
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
    if((fout = fdevopen(rDest, mode)) == NULL) {
      error_open_file(rDest);
      free(rDest);
      return 0;
    }
    mode[0] = 'r';
    h = src;
    do {
      if((rSrc = fillFnam(h->fnam, ff.ff_name)) == NULL) {
        fclose(fout);
        free(rDest);
        return 0;
      }
      mode[1] = (asc = h->flags & ASCII) != 0? 't': 'b';
    reOpenIn:
      if((fin = fdevopen(rSrc, mode)) == NULL) {
        error_open_file(rSrc);
        fclose(fout);
        free(rSrc);
        free(rDest);
        return 0;
      }
      if(isadev(fileno(fin)) && mode[1] != 't'
       && (h->flags & BINARY) == 0) {
        /* character devices are opened in textmode
          by default */
        fclose(fin);
        mode[1] = 't';
        goto reOpenIn;
      }

      dispCopy(rSrc, rDest, openMode == 'a' || h != src);
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
    } while((h = h->app) != NULL);
    if(asc) {   /* append the ^Z as we copied in ASCII mode */
      putc(0x1a, fout);
    }
    rc = ferror(fout);
    fclose(fout);
    if(rc) {
      error_write_file(rDest);
      free(rDest);
      return 0;
    }
    free(rDest);
  } while(FINDNEXT(&ff) == 0);

  return 1;
}

int copyFiles(struct CopySource *h)
{ char *fnam, *ext, *dst;
  int differ, rc;

  if(destIsDir) {
    if(!dfnsplit(h->fnam, NULL, NULL, &fnam, &ext)) {
      error_out_of_memory();
      return 0;
    }
    dst = dfnmerge(NULL, NULL, destFile, fnam, ext);
    free(fnam);
    free(ext);
    if(!dst) {
      error_out_of_memory();
      return 0;
    }
  } else
    dst = destFile;

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
    free(dst);
  return rc;
}

int cpyFlags(void)
{
  return (optA? ASCII: 0) | (optB? BINARY: 0);
}

int addSource(char *p)
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
    if((h = malloc(sizeof(struct CopySource))) == NULL) {
      error_out_of_memory();
      return 0;
    }
    if(!last)
      last = lastApp = head = h;
    else
      last = lastApp = last->nxt = h;

    h->nxt = h->app = NULL;
    h->fnam = q;
    h->flags = cpyFlags();
    if((q = strtok(NULL, "+")) == NULL)   /* no to-append file */
      return 1;
  }

  /* all the next files are to be appended to the source in "last" */
  assert(q);
  assert(lastApp);
  do {
    if((h = malloc(sizeof(struct CopySource))) == NULL) {
      error_out_of_memory();
      return 0;
    }
    lastApp->app = h;
    h->fnam = q;
    h->flags = cpyFlags();
    h->app = NULL;
  } while((q = strtok(NULL, "+")) != NULL);

  return 1;
}


int cmd_copy(char *rest)
{ char **argv, *p;
  int argc, opts, argi;
  int freeDestFile = 0;
  struct CopySource *h;

  /* Initialize options */
  optA = optB = optV = optY = 0;

  if((argv = scanCmdline(rest, opt_copy, NULL, &argc, &opts)) == NULL)
    return 1;

  /* scan the trailing '/a' and '/b' options */
  while(argc > 0 && isoption(argv[argc - 1])) {
    p = argv[--argc];     /* argv[] must not be changed */
    if(leadOptions(&p, opt_copy1, NULL) != E_None) {
      freep(argv);
      return 1;
    }
  }

  initContext();

  /* Now parse the remaining arguments into the copy file
    structure */
  for(argi = 0; argi < argc; ++argi)
    if(isoption(p = argv[argi])) {    /* infix /a or /b */
      if(leadOptions(&p, opt_copy1, NULL) != E_None) {
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
    destFile = dfnexpand(last->fnam, NULL);
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
    (last = h)->nxt = NULL;
    p = strchr(destFile, '\0') - 1;
    if(*p == '\\' || *p == '/')		/* must be a directory */
    	destIsDir = 1;
    else destIsDir = dfnstat(destFile) & DFN_DIRECTORY;
  } else {              /* Nay */
    destFile = ".";
    destIsDir = 1;
  }

  /* Now copy the files */
  h = head;
  while(copyFiles(h) && (h = h->nxt) != NULL);

  if(freeDestFile)
    free(destFile);
errRet:
  killContext();
  freep(argv);
  return 0;
}

#endif
