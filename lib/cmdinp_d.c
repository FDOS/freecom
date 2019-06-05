/* readcommandEnhanced for DBCS */

#if !defined(DBCS)
#define MbLen(s) (1)
#define isDbcsLead(ch) (0)
#endif

unsigned curposToXY(const char * const str_top, unsigned curpos, unsigned *offset_x0, unsigned *offset_y0)
{
	unsigned x_max0 = MAX_X - 1;
	unsigned x = offset_x0 ? *offset_x0 : 0, y = offset_y0 ? *offset_y0 : 0;
	unsigned pos = 0, spccnt = 0;
	unsigned n, nx;

	while(str_top[pos]) {
	  n = MbLen(&str_top[pos]);
	  nx = x + n;
	  if (nx > x_max0 + 1) { /* double-byte characher wraparound */
	    spccnt += (x_max0 - x + 1);
	    x = 0;
	    nx = n;
	    ++y;
	  }
	  if (pos >= curpos) break;
	  if (nx > x_max0) {
	    spccnt += (x_max0 - x + 1);
	    x = 0;
	    nx = n;
	    ++y;
	  } else {
	    x = nx;
	    spccnt += n;
	  }
	  pos += n;
	}
	if (offset_x0) *offset_x0 = x;
	if (offset_y0) *offset_y0 = y;
	
	return spccnt;
}
unsigned curposCalc(const char *const str_top, unsigned curpos, unsigned orgx, unsigned orgy, int do_move)
{
	unsigned x = orgx - 1, y = orgy - 1;
	unsigned rc;
	
	rc = curposToXY(str_top, curpos, &x, &y);
	if (do_move) goxy(x+1, y+1);
	return rc;
}

#define curposUpdate(s,p,x,y) curposCalc(s,p,x,y,1)

unsigned curposNext(const char * const str_top, unsigned curpos)
{
	return curpos + MbLen(&str_top[curpos]);
}

unsigned curposPrev(const char *const str_top, unsigned curpos)
{
	unsigned pos = 0;
	unsigned n;
	
	while(str_top[pos]) {
		n = MbLen(&str_top[pos]);
		if (pos + n >= curpos) break;
		pos += n;
	}
	return pos;
}

unsigned eraseLine(const char * const str_top, unsigned posfrom, unsigned posto, unsigned orgx, unsigned orgy)
{
	unsigned rc;
	unsigned cpfrom, cpto, count;

	cpto = curposCalc(str_top, posto, orgx, orgy, 0);
	cpfrom = curposCalc(str_top, posfrom, orgx, orgy, 1);
	
	count = rc = cpfrom < cpto ? (cpto - cpfrom) : 0;
	while(count--) outc(' ');
	return rc;
}


static void clrcmdline_oxy_d(char * const str, const int maxlen, const unsigned charcount, unsigned orgx, unsigned orgy)
{
	eraseLine(str, 0, charcount, orgx, orgy);
	goxy(orgx, orgy);
	memset(str, 0, maxlen);
}

#undef clrcmdline
#define clrcmdline(s,m,cc,ox,oy,cpos)	clrcmdline_oxy_d(s,m,cc,ox,oy)


void readcommandEnhanced(char * const str, const int maxlen)
{
	static unsigned orgx, orgy;		/* start of current line */
	unsigned char insert = 1;
	unsigned ch;
#ifdef FEATURE_FILENAME_COMPLETION
	unsigned lastch = 0;
#endif
#ifdef FEATURE_HISTORY
	int histLevel = 0;
	char prvLine[MAX_INTERNAL_COMMAND_SIZE];
#endif
	unsigned curx;
	unsigned cury;
	int count;
	unsigned current = 0;
	unsigned charcount = 0;
	unsigned leadch = 0;
	unsigned prev, clen;
	unsigned char do_insert;

	assert(str);
	assert(maxlen <= MAX_INTERNAL_COMMAND_SIZE);

	/* if echo off, don't print prompt */
	if(echo)
		printprompt();

	orgx = wherex();
	orgy = wherey();
	memset(str, 0, maxlen);

	setcursorstate(insert);

#ifdef FEATURE_HISTORY
	histGet(histLevel - 1, prvLine, sizeof(prvLine));
#endif
#if defined(NEC98)
	prepareSpecialKeys();
#endif

	do {
		cury = wherey();
		ch = cgetchar();
		if (cury > wherey())
		  orgy -= cury - wherey(); /* workaround for scroll-up by some FEP(IME)s */

		if(cbreak)
			ch = KEY_CTL_C;

		if (leadch == 0 && ch >= 0x20 && ch <= 0xff && isDbcsLead((unsigned char)ch)) {
		  leadch = ch;
		  continue;
		}
		do_insert = insert;

		switch(ch) {
		case KEY_BS:               /* delete character to left of cursor */

			if(current > 0 && charcount > 0) {
			  prev = curposPrev(str, current);
			  clen = current - prev;
			  eraseLine(str, prev, charcount, orgx, orgy);
			  if (clen > 0) {
			    for(count = prev; count <= (charcount - clen); ++count)
			      str[count] = str[count + clen];
			    str[count] = '\0';
			    curposUpdate(str, prev, orgx, orgy);
			    outs(&str[prev]);
			    current = prev;
			    charcount -= clen;
			    curposUpdate(str, current, orgx, orgy);
			  }
			}
			break;

		case KEY_INSERT:           /* toggle insert/overstrike mode */
			insert ^= 1;
				setcursorstate(insert);
			break;

		case KEY_DELETE:           /* delete character under cursor */

			if (current != charcount && charcount > 0)
			{
			  unsigned clen = MbLen(&str[current]);
			  eraseLine(str, current, charcount, orgx, orgy);
			  charcount = (charcount > clen) ? charcount - clen : 0;
			  for (count = current; count <= charcount; count++)
			    str[count] = str[count + clen];
			  str[charcount] = '\0';
			  curposUpdate(str, current, orgx, orgy);
			  curx = wherex();
			  cury = wherey();
			  outs(&str[current]);
			  goxy(curx, cury);
			}
			break;

		case KEY_HOME:             /* goto beginning of string */

			if (current != 0)
			{
			  goxy(orgx, orgy);
			  current = 0;
			}
			break;

		case KEY_END:              /* goto end of string */

			if (current != charcount)
			{
			  goxy(orgx, orgy);
			  outs(str);
			  current = charcount;
			}
			break;

#ifdef FEATURE_FILENAME_COMPLETION
		case KEY_TAB:		 /* expand current file name */
			if(current == charcount) {      /* only works at end of line */
			  if(lastch != KEY_TAB) { /* if first TAB, complete filename */
				complete_filename(str, charcount);
				charcount = strlen(str);
				current = charcount;

				goxy(orgx, orgy);
				outs_xyfix(str, &orgx, &orgy);
			  } else {                 /* if second TAB, list matches */
				if (show_completion_matches(str, charcount))
				{
				  printprompt();
				  orgx = wherex();
				  orgy = wherey();
				  outs(str);
				}
			  }
			}
			else
			  beep();
			break;
#endif

		case KEY_ENTER:            /* end input, return to main */

#ifdef FEATURE_HISTORY
			if(str[0])
			  histSet(0, str);      /* add to the history */
#endif

			outc('\n');
			break;

		case KEY_CTL_C:       		/* ^C */
		case KEY_ESC:              /* clear str  Make this callable! */

			clrcmdline(str, maxlen, charcount, orgx, orgy, current);
			current = charcount = 0;

			if(ch == KEY_CTL_C && !echo) {
			  /* enable echo to let user know that's this
				is the command line */
			  echo = 1;
			  printprompt();
			}
			break;

#ifdef FEATURE_HISTORY
			  
		case KEY_F3:               /* get previous command from buffer */
			if(charcount < strlen(prvLine)) {
				outs(strcpy(&str[charcount], &prvLine[charcount]));
			   current = charcount = strlen(str);
		   }
		   break;

		case KEY_UP:               /* get previous command from buffer */
			if(!histGet(--histLevel, prvLine, sizeof(prvLine)))
				++histLevel;		/* failed -> keep current command line */
			else {
				clrcmdline(str, maxlen, charcount, orgx, orgy, current);
				strcpy(str, prvLine);
				current = charcount = strlen(str);
				outs_xyfix(str, &orgx, &orgy); // outs(str);
				histGet(histLevel - 1, prvLine, sizeof(prvLine));
			}
			break;

		case KEY_DOWN:             /* get next command from buffer */
			if(histLevel) {
				clrcmdline(str, maxlen, charcount, orgx, orgy, current);
				strcpy(prvLine, str);
				histGet(++histLevel, str, maxlen);
				current = charcount = strlen(str);
				outs_xyfix(str, &orgx, &orgy); // outs(str);
			}
			break;

		case KEY_F5: /* keep cmdline in F3/UP buffer and move to next line */
			strcpy(prvLine, str);
			clrcmdline(str, maxlen, charcount, orgx, orgy, current);
			outc('@');
			if(orgy >= MAX_Y) {
				outc('\n');			/* Force scroll */
				orgy = MAX_Y;
			} else {
				++orgy;
			}
			goxy(orgx, orgy);
			current = charcount = 0;

			break;

#endif

		case KEY_LEFT:             /* move cursor left */
			if(current > 0) {
			  current = curposPrev(str, current);
			  curposUpdate(str, current, orgx, orgy);
			}
			break;

#if defined(KEY_CTRL_LEFT)
		case KEY_CTRL_LEFT:	/* move cursor left to begin of word */
			while(current > 0) {
			  current = curposPrev(str, current);
			  if (current == 0) break;

			  if(isworddelimiter(str[current-1])	/* ignore current == 0 */
			   && !isworddelimiter(str[current]))
			     break;
			}
			curposUpdate(str, current, orgx, orgy);
			break;
#endif

#if defined(KEY_CTRL_RIGHT)
		case KEY_CTRL_RIGHT:	/* move cursor right to begin of word */
			while(current < charcount) {
			  current = curposNext(str, current);

			  if(isworddelimiter(str[current-1])
			   && !isworddelimiter(str[current]))
			     break;
			}
			curposUpdate(str, current, orgx, orgy);
			break;
#endif

		case KEY_RIGHT:            /* move cursor right */

			if (current != charcount)
			{
			  current = curposNext(str, current);
			  curposUpdate(str, current, orgx, orgy);
				break;
			}
#ifndef FEATURE_HISTORY
			break;
#endif
			/* cursor-right at end of string grabs the next character
				from the previous line */
			/* FALL THROUGH to KEY_F1 */

#ifdef FEATURE_HISTORY
		case KEY_F1:       /* get character from last command buffer */
			{
			  unsigned curdbc = 0;
			  for(count = 0; str[count];) {
			    if (count >= current) break;
			    count += MbLen(&str[count]);
			    ++curdbc;
			  }
			  for(prev = 0; prvLine[prev];) {
			    if ((int)curdbc <= 0) break;
			    prev += MbLen(&prvLine[prev]);
			    --curdbc;
			  }
			  /* get keyvalue from prevLine */
			  ch = prvLine[prev];
			  if (ch == 0) break;
			  
			  if (MbLen(&prvLine[prev]) > 1) {
			    leadch = ch;
			    ch = prvLine[prev + 1];
			  } else {
			    leadch = 0;
			  }
			  do_insert = 0;
			}
			/* fallthrough to default keyinput */
#endif

		default:                 /* insert character into string... */

			if (leadch && (ch < 0x20 || ch >= 0xfe)) {
			  beep();
			  break;
			}
			clen = leadch ? 2 : 1;
			if (ch < 32 || ch > 255 || charcount >= maxlen - clen) {
			  beep();
			}
			else {
			  if (current >= charcount) do_insert = 1;
			  if (!do_insert) { /* overwrite */
			    unsigned clen_org = MbLen(&str[current]);
			    if (clen == clen_org) {
			      /* same width: replace simply */
			      if (leadch) {
			        str[current] = leadch;
			        str[current + 1] = ch;
			      } else {
			        str[current] = ch;
			      }
			      /* curposUpdate(str, current, orgx, orgy); */
			      outs(&str[current]);
			      current += clen;
			      curposUpdate(str, current, orgx, orgy);
			    }
			    else {
			      /* replace double-byte char with single-byte one, or vice versa */
			      eraseLine(str, current, charcount, orgx, orgy);
			      for(count = current + clen_org; count < charcount; ++count)
			        str[count - clen_org] = str[count];
			      charcount -= clen_org;
			      str[charcount] = '\0';
			      do_insert = 1;
			      /* fallthrough to insert routine */
			    }
			  }
			  if (do_insert) {
			    unsigned cpto, cpfrom;
			    prev = curposPrev(str, current);
			    for (count = charcount; count > current; --count)
			      str[count + clen - 1] = str[count - 1];
			    if (leadch) str[current++] = leadch;
			    str[current++] = ch;
			    charcount += clen;
			    str[charcount] = '\0';
			    cpto = curposCalc(str, charcount, orgx, orgy, 0);
			    cpfrom = curposCalc(str, prev, orgx, orgy, 1);
			    count = cpfrom < cpto ? (cpto - cpfrom) : 0;
			    while(count--) {
			      curx = wherex();
			      cury = wherey();
			      outc(' ');
			      if (wherex() < curx && wherey() <= cury)
			        orgy -= cury - wherey() + 1; /* wrap-around (and scrolling up) */
			    }
			    curposUpdate(str, prev, orgx, orgy);
			    outs(&str[prev]);
			    curposUpdate(str, current, orgx, orgy);
			  }
			}
			break;
		}
#ifdef FEATURE_FILENAME_COMPLETION
		lastch = ch;
#endif
#if defined(DBCS)
		leadch = 0;
#endif
	} while(ch != KEY_ENTER);

#if defined(NEC98)
	restoreSpecialKeys();
#endif
	setcursorstate(insert);
}

