#.AUTODEPEND

CFG_DEPENDENCIES = makefile.mak

TOP=..
!include "$(TOP)/config.mak"

all: freecom.lib

OBJ1 =	absfile.obj\
	almemblk.obj\
	alprmblk.obj\
	alsysblk.obj\
	app_get.obj\
	app_set.obj\
	beep_l.obj\
	beep_n.obj
OBJ2 =	brk_get.obj\
	brk_set.obj\
	cbreak.obj\
	cbs.obj\
	cd_dir.obj\
	cgetch.obj\
	cgettime.obj\
	chgctxt.obj
OBJ3 =	chgdrv.obj\
	chgenv.obj\
	chgenvc.obj\
	chgenvr.obj\
	cmdinput.obj\
	comfile.obj\
	compfile.obj
OBJ4 =	critend.obj\
	critrchk.obj\
	ctxt.obj\
	ctxt_adr.obj\
	ctxt_as.obj\
	ctxt_chg.obj\
	ctxt_clr.obj\
	ctxt_get.obj
OBJ5 =	ctxt_inf.obj\
	ctxt_mk.obj\
	ctxt_mkb.obj\
	ctxt_mkn.obj\
	ctxt_pop.obj\
	ctxt_psh.obj\
	ctxt_rnu.obj\
	ctxt_set.obj
OBJ6 =	ctxt_ss.obj\
	ctxt_vw.obj\
	curdatel.obj\
	curtime.obj\
	cwd.obj\
	dateget.obj\
	dateset.obj\
	dbg_c.obj
OBJ7 =	dbg_mem.obj\
	dbg_prnt.obj\
	dbg_s.obj\
	dbg_sn.obj\
	devopen.obj\
	dfn_err.obj\
	dispcnt.obj\
	dispexit.obj
OBJ8 =	drvnum.obj\
	efct_001.obj\
	exec.obj\
	exec1.obj\
	farread.obj\
	filecomp.obj\
	fdattr.obj
OBJ9 =	fdevopen.obj\
	fdsattr.obj\
	fillcomp.obj\
	find.obj\
	freep.obj\
	frsysblk.obj\
	fstpcpy.obj\
	gallstr.obj
OBJ10 =	get1mcb.obj\
	getenv.obj\
	goxy.obj\
	grabfcom.obj\
	gumblink.obj\
	hdlrctxt.obj\
	hist_get.obj\
	hist_set.obj
OBJ11 =	inputdos.obj\
	is_empty.obj\
	is_fnamc.obj\
	is_fnstr.obj\
	is_pchr.obj\
	isadev.obj\
	keyprsd.obj\
	kswap_c.obj
OBJ12 =	lastdget.obj\
	lastdset.obj\
	leadopt.obj\
	lfnfuncs.obj\
	lowexec.obj\
	ltrimcl.obj\
	ltrimsp.obj\
	lwr1wd.obj
OBJ13 =	match.obj\
	messages.obj\
	mk_rddir.obj\
	mktmpfil.obj\
	msg_dflt.obj\
	msg_dps.obj\
	msg_fstr.obj\
	msg_get.obj
OBJ14 =	msg_gpt.obj\
	msg_mkey.obj\
	msg_prmp.obj\
	mux_ae.obj\
	myperror.obj\
	nls.obj\
	nls_date.obj\
	nls_time.obj
OBJ15 =	num_fmt.obj\
	onoff.obj\
	openf.obj\
	optsb.obj\
	optsi.obj\
	optss.obj\
	parsenum.obj\
	pr_date.obj
OBJ16 =	pr_prmpt.obj\
	pr_time.obj\
	prf.obj\
	prprompt.obj\
	readcmd.obj\
	realnum.obj\
	res.obj\
	res_r.obj\
	res_vald.obj
OBJ17 =	res_w.obj\
	resfile.obj\
	rmtmpfil.obj\
	rtrimcl.obj\
	rtrimsp.obj\
	salloc.obj\
	samefile.obj\
	scancmd.obj
OBJ18 =	scanopt.obj\
	session.obj\
	showcmds.obj\
	skqwd.obj\
	spfnam.obj\
	split.obj\
	sumblink.obj
OBJ19 =	timeget.obj\
	timeset.obj\
	tmpnam.obj\
	trimcl.obj\
	trimsp.obj\
	truepath.obj\
	truncate.obj\
	txtlend.obj
OBJ20 =	unquote.obj\
	vcgetch.obj\
	vcgetstr.obj\
	where.obj\
	delay.obj
OBJ21 =	err1.obj\
	err2.obj\
	err3.obj\
	err4.obj\
	err5.obj\
	err6.obj\
	err7.obj\
	err8.obj\
	err9.obj\
	err10.obj
OBJ22 =	err11.obj\
	err12.obj\
	err13.obj\
	err14.obj\
	err15.obj\
	err16.obj\
	err17.obj\
	err18.obj\
	err19.obj\
	err20.obj
OBJ23 =	err21.obj\
	err22.obj\
	err23.obj\
	err24.obj\
	err25.obj\
	err26.obj\
	err27.obj\
	err28.obj\
	err29.obj\
	err30.obj
OBJ24 =	err31.obj\
	err32.obj\
	err33.obj\
	err34.obj\
	err35.obj\
	err36.obj\
	err37.obj\
	err38.obj\
	err39.obj\
	err40.obj
OBJ25 =	err41.obj\
	err42.obj\
	err43.obj\
	err44.obj\
	err45.obj\
	err46.obj\
	err47.obj\
	err48.obj\
	err49.obj\
	err50.obj
OBJ26 =	err51.obj\
	err52.obj\
	err53.obj\
	err54.obj\
	err55.obj\
	err56.obj\
	err57.obj\
	err58.obj\
	err59.obj\
	err60.obj
OBJ27 =	err61.obj\
	err62.obj\
	err63.obj\
	err64.obj\
	err65.obj\
	err66.obj\
	err67.obj\
	err68.obj\
	err69.obj\
	err70.obj
OBJ28 =	err71.obj\
	err72.obj\
	err73.obj\
	err74.obj\
	err75.obj\
	err76.obj\
	err77.obj\
	err78.obj\
	err79.obj\
	err80.obj
OBJ29 =	err81.obj\
	err82.obj\
	err83.obj\
	err84.obj\
	err85.obj\
	err86.obj\
	err87.obj

echolib.bat : ../scripts/echolib.bat
	$(CP) ..$(DIRSEP)scripts$(DIRSEP)echolib.bat .

freecom.rsp : echolib.bat $(OBJ16) $(OBJ17) $(OBJ18) $(OBJ19) $(OBJ20) \
$(OBJ21) $(OBJ22) $(OBJ23) $(OBJ24) $(OBJ25) $(OBJ26) $(OBJ27) $(OBJ28) \
$(OBJ29) makefile.mak
	$(RMFILES) freecom.rsp
	$(ECHOLIB) freecom.rsp $(OBJ1)
	$(ECHOLIB) freecom.rsp $(OBJ2)
	$(ECHOLIB) freecom.rsp $(OBJ3)
	$(ECHOLIB) freecom.rsp $(OBJ4)
	$(ECHOLIB) freecom.rsp $(OBJ5)
	$(ECHOLIB) freecom.rsp $(OBJ6)
	$(ECHOLIB) freecom.rsp $(OBJ7)
	$(ECHOLIB) freecom.rsp $(OBJ8)
	$(ECHOLIB) freecom.rsp $(OBJ9)
	$(ECHOLIB) freecom.rsp $(OBJ10)
	$(ECHOLIB) freecom.rsp $(OBJ11)
	$(ECHOLIB) freecom.rsp $(OBJ12)
	$(ECHOLIB) freecom.rsp $(OBJ13)
	$(ECHOLIB) freecom.rsp $(OBJ14)
	$(ECHOLIB) freecom.rsp $(OBJ15)
	$(ECHOLIB) freecom.rsp $(OBJ16)
	$(ECHOLIB) freecom.rsp $(OBJ17)
	$(ECHOLIB) freecom.rsp $(OBJ18)
	$(ECHOLIB) freecom.rsp $(OBJ19)
	$(ECHOLIB) freecom.rsp $(OBJ20)
	$(ECHOLIB) freecom.rsp $(OBJ21)
	$(ECHOLIB) freecom.rsp $(OBJ22)
	$(ECHOLIB) freecom.rsp $(OBJ23)
	$(ECHOLIB) freecom.rsp $(OBJ24)
	$(ECHOLIB) freecom.rsp $(OBJ25)
	$(ECHOLIB) freecom.rsp $(OBJ26)
	$(ECHOLIB) freecom.rsp $(OBJ27)
	$(ECHOLIB) freecom.rsp $(OBJ28)
	$(ECHOLIB) freecom.rsp $(OBJ29)

freecom.lib : $(CFG) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) \
$(OBJ8) $(OBJ9) $(OBJ10) $(OBJ11) $(OBJ12) $(OBJ13) $(OBJ14) $(OBJ15) \
freecom.rsp
	$(RMFILES) freecom.lib
	$(AR) freecom.lib @freecom.rsp $(LIBLIST) freecom.lst
