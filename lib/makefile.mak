#.AUTODEPEND

CFG_DEPENDENCIES = makefile.mak

TOP=..
!include "$(TOP)/config.mak"

all: freecom.lib

OBJS1 = absfile.obj almemblk.obj alprmblk.obj alsysblk.obj app_get.obj app_set.obj beep_l.obj
OBJS2 = beep_n.obj brk_get.obj brk_set.obj cbreak.obj cbs.obj cd_dir.obj cgetch.obj
OBJS3 = cgettime.obj chgctxt.obj chgdrv.obj chgenv.obj chgenvc.obj chgenvr.obj cmdinput.obj
OBJS4 = comfile.obj compfile.obj critend.obj critrchk.obj ctxt.obj ctxt_adr.obj ctxt_as.obj
OBJS5 = ctxt_chg.obj ctxt_clr.obj ctxt_get.obj ctxt_inf.obj ctxt_mk.obj ctxt_mkb.obj ctxt_mkn.obj
OBJS6 = ctxt_pop.obj ctxt_psh.obj ctxt_rnu.obj ctxt_set.obj ctxt_ss.obj ctxt_vw.obj curdatel.obj
OBJS7 = curtime.obj cwd.obj dateget.obj dateset.obj dbg_c.obj dbg_mem.obj dbg_prnt.obj
OBJS8 = dbg_s.obj dbg_sn.obj devopen.obj dfn_err.obj dispcnt.obj dispexit.obj drvnum.obj
OBJS9 = efct_001.obj exec.obj exec1.obj farread.obj filecomp.obj fdattr.obj fdevopen.obj
OBJS10 = fdsattr.obj fillcomp.obj find.obj freep.obj frsysblk.obj fstpcpy.obj gallstr.obj
OBJS11 = get1mcb.obj getenv.obj goxy.obj grabfcom.obj gumblink.obj hdlrctxt.obj hist_get.obj
OBJS12 = hist_set.obj inputdos.obj is_empty.obj is_fnamc.obj is_fnstr.obj is_pchr.obj isadev.obj
OBJS13 = keyprsd.obj kswap_c.obj lastdget.obj lastdset.obj leadopt.obj lfnfuncs.obj lowexec.obj
OBJS14 = ltrimcl.obj ltrimsp.obj lwr1wd.obj match.obj messages.obj mk_rddir.obj mktmpfil.obj
OBJS15 = msg_dflt.obj msg_dps.obj msg_fstr.obj msg_get.obj msg_gpt.obj msg_mkey.obj msg_prmp.obj
OBJS16 = mux_ae.obj myperror.obj nls.obj nls_date.obj nls_time.obj num_fmt.obj onoff.obj
OBJS17 = openf.obj optsb.obj optsi.obj optss.obj parsenum.obj pr_date.obj pr_prmpt.obj
OBJS18 = pr_time.obj prf.obj prprompt.obj readcmd.obj realnum.obj res.obj res_r.obj
OBJS19 = res_vald.obj res_w.obj resfile.obj rmtmpfil.obj rtrimcl.obj rtrimsp.obj salloc.obj
OBJS20 = samefile.obj scancmd.obj scanopt.obj session.obj showcmds.obj skqwd.obj spfnam.obj
OBJS21 = split.obj sumblink.obj timeget.obj timeset.obj tmpnam.obj trimcl.obj trimsp.obj
OBJS22 = truepath.obj truncate.obj txtlend.obj unquote.obj vcgetch.obj vcgetstr.obj where.obj
OBJS23 = delay.obj
OBJS24 = err1.obj err2.obj err3.obj err4.obj err5.obj err6.obj err7.obj
OBJS25 = err8.obj err9.obj err10.obj err11.obj err12.obj err13.obj err14.obj
OBJS26 = err15.obj err16.obj err17.obj err18.obj err19.obj err20.obj err21.obj
OBJS27 = err22.obj err23.obj err24.obj err25.obj err26.obj err27.obj err28.obj
OBJS28 = err29.obj err30.obj err31.obj err32.obj err33.obj err34.obj err35.obj
OBJS29 = err36.obj err37.obj err38.obj err39.obj err40.obj err41.obj err42.obj
OBJS30 = err43.obj err44.obj err45.obj err46.obj err47.obj err48.obj err49.obj
OBJS31 = err50.obj err51.obj err52.obj err53.obj err54.obj err55.obj err56.obj
OBJS32 = err57.obj err58.obj err59.obj err60.obj err61.obj err62.obj err63.obj
OBJS33 = err64.obj err65.obj err66.obj err67.obj err68.obj err69.obj err70.obj
OBJS34 = err71.obj err72.obj err73.obj err74.obj err75.obj err76.obj err77.obj
OBJS35 = err78.obj err79.obj err80.obj err81.obj err82.obj err83.obj err84.obj
OBJS36 = err85.obj err86.obj err87.obj

freecom.rsp : $(ECHOLIBDEP) makefile.mak
	$(RMFILES) freecom.rsp
	$(ECHOLIB) freecom.rsp $(OBJS1)
	$(ECHOLIB) freecom.rsp $(OBJS2)
	$(ECHOLIB) freecom.rsp $(OBJS3)
	$(ECHOLIB) freecom.rsp $(OBJS4)
	$(ECHOLIB) freecom.rsp $(OBJS5)
	$(ECHOLIB) freecom.rsp $(OBJS6)
	$(ECHOLIB) freecom.rsp $(OBJS7)
	$(ECHOLIB) freecom.rsp $(OBJS8)
	$(ECHOLIB) freecom.rsp $(OBJS9)
	$(ECHOLIB) freecom.rsp $(OBJS10)
	$(ECHOLIB) freecom.rsp $(OBJS11)
	$(ECHOLIB) freecom.rsp $(OBJS12)
	$(ECHOLIB) freecom.rsp $(OBJS13)
	$(ECHOLIB) freecom.rsp $(OBJS14)
	$(ECHOLIB) freecom.rsp $(OBJS15)
	$(ECHOLIB) freecom.rsp $(OBJS16)
	$(ECHOLIB) freecom.rsp $(OBJS17)
	$(ECHOLIB) freecom.rsp $(OBJS18)
	$(ECHOLIB) freecom.rsp $(OBJS19)
	$(ECHOLIB) freecom.rsp $(OBJS20)
	$(ECHOLIB) freecom.rsp $(OBJS21)
	$(ECHOLIB) freecom.rsp $(OBJS22)
	$(ECHOLIB) freecom.rsp $(OBJS23)
	$(ECHOLIB) freecom.rsp $(OBJS24)
	$(ECHOLIB) freecom.rsp $(OBJS25)
	$(ECHOLIB) freecom.rsp $(OBJS26)
	$(ECHOLIB) freecom.rsp $(OBJS27)
	$(ECHOLIB) freecom.rsp $(OBJS28)
	$(ECHOLIB) freecom.rsp $(OBJS29)
	$(ECHOLIB) freecom.rsp $(OBJS30)
	$(ECHOLIB) freecom.rsp $(OBJS31)
	$(ECHOLIB) freecom.rsp $(OBJS32)
	$(ECHOLIB) freecom.rsp $(OBJS33)
	$(ECHOLIB) freecom.rsp $(OBJS34)
	$(ECHOLIB) freecom.rsp $(OBJS35)
	$(ECHOLIB) freecom.rsp $(OBJS36)

freecom_deps1 : $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) $(OBJS7) \
$(OBJS8) $(OBJS9) $(OBJS10) $(OBJS11) $(OBJS12) $(OBJS13) $(OBJS14) $(OBJS15) \
$(OBJS16) $(OBJS17) $(OBJS18)

freecom_deps2 : $(OBJS19) $(OBJS20) $(OBJS21) $(OBJS22) $(OBJS23) $(OBJS24) \
$(OBJS25) $(OBJS26) $(OBJS27) $(OBJS28) $(OBJS29) $(OBJS30) $(OBJS31) $(OBJS32) \
$(OBJS33) $(OBJS34) $(OBJS35) $(OBJS36)

freecom.lib : $(CFG) freecom_deps1 freecom_deps2 freecom.rsp
	$(RMFILES) freecom.lib
	$(AR) freecom.lib @freecom.rsp $(LIBLIST) freecom.lst
