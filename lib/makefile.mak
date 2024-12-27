#.AUTODEPEND

CFG_DEPENDENCIES = makefile.mak

TOP=..
!include "$(TOP)/config.mak"

all: freecom.lib

OBJS1 = absfile.obj almemblk.obj alprmblk.obj alsysblk.obj app_get.obj app_set.obj beep_l.obj
LOBJS1 = $(LIBPLUS)absfile.obj $(LIBPLUS)almemblk.obj $(LIBPLUS)alprmblk.obj $(LIBPLUS)alsysblk.obj $(LIBPLUS)app_get.obj $(LIBPLUS)app_set.obj $(LIBPLUS)beep_l.obj
OBJS2 = beep_n.obj brk_get.obj brk_set.obj cbreak.obj cbs.obj cd_dir.obj cgetch.obj
LOBJS2 = $(LIBPLUS)beep_n.obj $(LIBPLUS)brk_get.obj $(LIBPLUS)brk_set.obj $(LIBPLUS)cbreak.obj $(LIBPLUS)cbs.obj $(LIBPLUS)cd_dir.obj $(LIBPLUS)cgetch.obj
OBJS3 = cgettime.obj chgctxt.obj chgdrv.obj chgenv.obj chgenvc.obj chgenvr.obj cmdinput.obj
LOBJS3 = $(LIBPLUS)cgettime.obj $(LIBPLUS)chgctxt.obj $(LIBPLUS)chgdrv.obj $(LIBPLUS)chgenv.obj $(LIBPLUS)chgenvc.obj $(LIBPLUS)chgenvr.obj $(LIBPLUS)cmdinput.obj
OBJS4 = comfile.obj compfile.obj critend.obj critrchk.obj ctxt.obj ctxt_adr.obj ctxt_as.obj
LOBJS4 = $(LIBPLUS)comfile.obj $(LIBPLUS)compfile.obj $(LIBPLUS)critend.obj $(LIBPLUS)critrchk.obj $(LIBPLUS)ctxt.obj $(LIBPLUS)ctxt_adr.obj $(LIBPLUS)ctxt_as.obj
OBJS5 = ctxt_chg.obj ctxt_clr.obj ctxt_get.obj ctxt_inf.obj ctxt_mk.obj ctxt_mkb.obj ctxt_mkn.obj
LOBJS5 = $(LIBPLUS)ctxt_chg.obj $(LIBPLUS)ctxt_clr.obj $(LIBPLUS)ctxt_get.obj $(LIBPLUS)ctxt_inf.obj $(LIBPLUS)ctxt_mk.obj $(LIBPLUS)ctxt_mkb.obj $(LIBPLUS)ctxt_mkn.obj
OBJS6 = ctxt_pop.obj ctxt_psh.obj ctxt_rnu.obj ctxt_set.obj ctxt_ss.obj ctxt_vw.obj curdatel.obj
LOBJS6 = $(LIBPLUS)ctxt_pop.obj $(LIBPLUS)ctxt_psh.obj $(LIBPLUS)ctxt_rnu.obj $(LIBPLUS)ctxt_set.obj $(LIBPLUS)ctxt_ss.obj $(LIBPLUS)ctxt_vw.obj $(LIBPLUS)curdatel.obj
OBJS7 = curtime.obj cwd.obj dateget.obj dateset.obj dbg_c.obj dbg_mem.obj dbg_prnt.obj
LOBJS7 = $(LIBPLUS)curtime.obj $(LIBPLUS)cwd.obj $(LIBPLUS)dateget.obj $(LIBPLUS)dateset.obj $(LIBPLUS)dbg_c.obj $(LIBPLUS)dbg_mem.obj $(LIBPLUS)dbg_prnt.obj
OBJS8 = dbg_s.obj dbg_sn.obj devopen.obj dfn_err.obj dispcnt.obj dispexit.obj drvnum.obj
LOBJS8 = $(LIBPLUS)dbg_s.obj $(LIBPLUS)dbg_sn.obj $(LIBPLUS)devopen.obj $(LIBPLUS)dfn_err.obj $(LIBPLUS)dispcnt.obj $(LIBPLUS)dispexit.obj $(LIBPLUS)drvnum.obj
OBJS9 = efct_001.obj exec.obj exec1.obj farread.obj filecomp.obj fdattr.obj fdevopen.obj
LOBJS9 = $(LIBPLUS)efct_001.obj $(LIBPLUS)exec.obj $(LIBPLUS)exec1.obj $(LIBPLUS)farread.obj $(LIBPLUS)filecomp.obj $(LIBPLUS)fdattr.obj $(LIBPLUS)fdevopen.obj
OBJS10 = fdsattr.obj fillcomp.obj find.obj freep.obj frsysblk.obj fstpcpy.obj gallstr.obj
LOBJS10 = $(LIBPLUS)fdsattr.obj $(LIBPLUS)fillcomp.obj $(LIBPLUS)find.obj $(LIBPLUS)freep.obj $(LIBPLUS)frsysblk.obj $(LIBPLUS)fstpcpy.obj $(LIBPLUS)gallstr.obj
OBJS11 = get1mcb.obj getenv.obj goxy.obj grabfcom.obj gumblink.obj hdlrctxt.obj hist_get.obj
LOBJS11 = $(LIBPLUS)get1mcb.obj $(LIBPLUS)getenv.obj $(LIBPLUS)goxy.obj $(LIBPLUS)grabfcom.obj $(LIBPLUS)gumblink.obj $(LIBPLUS)hdlrctxt.obj $(LIBPLUS)hist_get.obj
OBJS12 = hist_set.obj inputdos.obj is_empty.obj is_fnamc.obj is_fnstr.obj is_pchr.obj isadev.obj
LOBJS12 = $(LIBPLUS)hist_set.obj $(LIBPLUS)inputdos.obj $(LIBPLUS)is_empty.obj $(LIBPLUS)is_fnamc.obj $(LIBPLUS)is_fnstr.obj $(LIBPLUS)is_pchr.obj $(LIBPLUS)isadev.obj
OBJS13 = keyprsd.obj kswap_c.obj lastdget.obj lastdset.obj leadopt.obj lfnfuncs.obj lowexec.obj
LOBJS13 = $(LIBPLUS)keyprsd.obj $(LIBPLUS)kswap_c.obj $(LIBPLUS)lastdget.obj $(LIBPLUS)lastdset.obj $(LIBPLUS)leadopt.obj $(LIBPLUS)lfnfuncs.obj $(LIBPLUS)lowexec.obj
OBJS14 = ltrimcl.obj ltrimsp.obj lwr1wd.obj match.obj messages.obj mk_rddir.obj mktmpfil.obj
LOBJS14 = $(LIBPLUS)ltrimcl.obj $(LIBPLUS)ltrimsp.obj $(LIBPLUS)lwr1wd.obj $(LIBPLUS)match.obj $(LIBPLUS)messages.obj $(LIBPLUS)mk_rddir.obj $(LIBPLUS)mktmpfil.obj
OBJS15 = msg_dflt.obj msg_dps.obj msg_fstr.obj msg_get.obj msg_gpt.obj msg_mkey.obj msg_prmp.obj
LOBJS15 = $(LIBPLUS)msg_dflt.obj $(LIBPLUS)msg_dps.obj $(LIBPLUS)msg_fstr.obj $(LIBPLUS)msg_get.obj $(LIBPLUS)msg_gpt.obj $(LIBPLUS)msg_mkey.obj $(LIBPLUS)msg_prmp.obj
OBJS16 = mux_ae.obj myperror.obj nls.obj nls_date.obj nls_time.obj num_fmt.obj onoff.obj
LOBJS16 = $(LIBPLUS)mux_ae.obj $(LIBPLUS)myperror.obj $(LIBPLUS)nls.obj $(LIBPLUS)nls_date.obj $(LIBPLUS)nls_time.obj $(LIBPLUS)num_fmt.obj $(LIBPLUS)onoff.obj
OBJS17 = openf.obj optsb.obj optsi.obj optss.obj parsenum.obj pr_date.obj pr_prmpt.obj
LOBJS17 = $(LIBPLUS)openf.obj $(LIBPLUS)optsb.obj $(LIBPLUS)optsi.obj $(LIBPLUS)optss.obj $(LIBPLUS)parsenum.obj $(LIBPLUS)pr_date.obj $(LIBPLUS)pr_prmpt.obj
OBJS18 = pr_time.obj prf.obj prprompt.obj readcmd.obj realnum.obj res.obj res_r.obj
LOBJS18 = $(LIBPLUS)pr_time.obj $(LIBPLUS)prf.obj $(LIBPLUS)prprompt.obj $(LIBPLUS)readcmd.obj $(LIBPLUS)realnum.obj $(LIBPLUS)res.obj $(LIBPLUS)res_r.obj
OBJS19 = res_vald.obj res_w.obj resfile.obj rmtmpfil.obj rtrimcl.obj rtrimsp.obj salloc.obj
LOBJS19 = $(LIBPLUS)res_vald.obj $(LIBPLUS)res_w.obj $(LIBPLUS)resfile.obj $(LIBPLUS)rmtmpfil.obj $(LIBPLUS)rtrimcl.obj $(LIBPLUS)rtrimsp.obj $(LIBPLUS)salloc.obj
OBJS20 = samefile.obj scancmd.obj scanopt.obj session.obj showcmds.obj skqwd.obj spfnam.obj
LOBJS20 = $(LIBPLUS)samefile.obj $(LIBPLUS)scancmd.obj $(LIBPLUS)scanopt.obj $(LIBPLUS)session.obj $(LIBPLUS)showcmds.obj $(LIBPLUS)skqwd.obj $(LIBPLUS)spfnam.obj
OBJS21 = split.obj sumblink.obj timeget.obj timeset.obj tmpnam.obj trimcl.obj trimsp.obj
LOBJS21 = $(LIBPLUS)split.obj $(LIBPLUS)sumblink.obj $(LIBPLUS)timeget.obj $(LIBPLUS)timeset.obj $(LIBPLUS)tmpnam.obj $(LIBPLUS)trimcl.obj $(LIBPLUS)trimsp.obj
OBJS22 = truepath.obj truncate.obj txtlend.obj unquote.obj vcgetch.obj vcgetstr.obj where.obj
LOBJS22 = $(LIBPLUS)truepath.obj $(LIBPLUS)truncate.obj $(LIBPLUS)txtlend.obj $(LIBPLUS)unquote.obj $(LIBPLUS)vcgetch.obj $(LIBPLUS)vcgetstr.obj $(LIBPLUS)where.obj
OBJS23 = delay.obj
LOBJS23 = $(LIBPLUS)delay.obj
OBJS24 = err1.obj err2.obj err3.obj err4.obj err5.obj err6.obj err7.obj
LOBJS24 = $(LIBPLUS)err1.obj $(LIBPLUS)err2.obj $(LIBPLUS)err3.obj $(LIBPLUS)err4.obj $(LIBPLUS)err5.obj $(LIBPLUS)err6.obj $(LIBPLUS)err7.obj
OBJS25 = err8.obj err9.obj err10.obj err11.obj err12.obj err13.obj err14.obj
LOBJS25 = $(LIBPLUS)err8.obj $(LIBPLUS)err9.obj $(LIBPLUS)err10.obj $(LIBPLUS)err11.obj $(LIBPLUS)err12.obj $(LIBPLUS)err13.obj $(LIBPLUS)err14.obj
OBJS26 = err15.obj err16.obj err17.obj err18.obj err19.obj err20.obj err21.obj
LOBJS26 = $(LIBPLUS)err15.obj $(LIBPLUS)err16.obj $(LIBPLUS)err17.obj $(LIBPLUS)err18.obj $(LIBPLUS)err19.obj $(LIBPLUS)err20.obj $(LIBPLUS)err21.obj
OBJS27 = err22.obj err23.obj err24.obj err25.obj err26.obj err27.obj err28.obj
LOBJS27 = $(LIBPLUS)err22.obj $(LIBPLUS)err23.obj $(LIBPLUS)err24.obj $(LIBPLUS)err25.obj $(LIBPLUS)err26.obj $(LIBPLUS)err27.obj $(LIBPLUS)err28.obj
OBJS28 = err29.obj err30.obj err31.obj err32.obj err33.obj err34.obj err35.obj
LOBJS28 = $(LIBPLUS)err29.obj $(LIBPLUS)err30.obj $(LIBPLUS)err31.obj $(LIBPLUS)err32.obj $(LIBPLUS)err33.obj $(LIBPLUS)err34.obj $(LIBPLUS)err35.obj
OBJS29 = err36.obj err37.obj err38.obj err39.obj err40.obj err41.obj err42.obj
LOBJS29 = $(LIBPLUS)err36.obj $(LIBPLUS)err37.obj $(LIBPLUS)err38.obj $(LIBPLUS)err39.obj $(LIBPLUS)err40.obj $(LIBPLUS)err41.obj $(LIBPLUS)err42.obj
OBJS30 = err43.obj err44.obj err45.obj err46.obj err47.obj err48.obj err49.obj
LOBJS30 = $(LIBPLUS)err43.obj $(LIBPLUS)err44.obj $(LIBPLUS)err45.obj $(LIBPLUS)err46.obj $(LIBPLUS)err47.obj $(LIBPLUS)err48.obj $(LIBPLUS)err49.obj
OBJS31 = err50.obj err51.obj err52.obj err53.obj err54.obj err55.obj err56.obj
LOBJS31 = $(LIBPLUS)err50.obj $(LIBPLUS)err51.obj $(LIBPLUS)err52.obj $(LIBPLUS)err53.obj $(LIBPLUS)err54.obj $(LIBPLUS)err55.obj $(LIBPLUS)err56.obj
OBJS32 = err57.obj err58.obj err59.obj err60.obj err61.obj err62.obj err63.obj
LOBJS32 = $(LIBPLUS)err57.obj $(LIBPLUS)err58.obj $(LIBPLUS)err59.obj $(LIBPLUS)err60.obj $(LIBPLUS)err61.obj $(LIBPLUS)err62.obj $(LIBPLUS)err63.obj
OBJS33 = err64.obj err65.obj err66.obj err67.obj err68.obj err69.obj err70.obj
LOBJS33 = $(LIBPLUS)err64.obj $(LIBPLUS)err65.obj $(LIBPLUS)err66.obj $(LIBPLUS)err67.obj $(LIBPLUS)err68.obj $(LIBPLUS)err69.obj $(LIBPLUS)err70.obj
OBJS34 = err71.obj err72.obj err73.obj err74.obj err75.obj err76.obj err77.obj
LOBJS34 = $(LIBPLUS)err71.obj $(LIBPLUS)err72.obj $(LIBPLUS)err73.obj $(LIBPLUS)err74.obj $(LIBPLUS)err75.obj $(LIBPLUS)err76.obj $(LIBPLUS)err77.obj
OBJS35 = err78.obj err79.obj err80.obj err81.obj err82.obj err83.obj err84.obj
LOBJS35 = $(LIBPLUS)err78.obj $(LIBPLUS)err79.obj $(LIBPLUS)err80.obj $(LIBPLUS)err81.obj $(LIBPLUS)err82.obj $(LIBPLUS)err83.obj $(LIBPLUS)err84.obj
OBJS36 = err85.obj err86.obj err87.obj
LOBJS36 = $(LIBPLUS)err85.obj $(LIBPLUS)err86.obj $(LIBPLUS)err87.obj

echolib.bat : ../scripts/echolib.bat
	$(CP) ..$(DIRSEP)scripts$(DIRSEP)echolib.bat .

freecom.rsp : echolib.bat makefile.mak
	$(RMFILES) freecom.rsp
	$(ECHOLIB) freecom.rsp $(LOBJS1) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS2) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS3) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS4) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS5) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS6) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS7) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS8) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS9) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS10) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS11) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS12) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS13) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS14) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS15) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS16) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS17) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS18) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS19) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS20) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS21) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS22) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS23) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS24) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS25) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS26) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS27) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS28) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS29) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS30) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS31) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS32) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS33) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS34) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS35) $(LIBCONT)
	$(ECHOLIB) freecom.rsp $(LOBJS36)

freecom_deps1 : $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) $(OBJS7) \
$(OBJS8) $(OBJS9) $(OBJS10) $(OBJS11) $(OBJS12) $(OBJS13) $(OBJS14) $(OBJS15) \
$(OBJS16) $(OBJS17) $(OBJS18)

freecom_deps2 : $(OBJS19) $(OBJS20) $(OBJS21) $(OBJS22) $(OBJS23) $(OBJS24) \
$(OBJS25) $(OBJS26) $(OBJS27) $(OBJS28) $(OBJS29) $(OBJS30) $(OBJS31) $(OBJS32) \
$(OBJS33) $(OBJS34) $(OBJS35) $(OBJS36)

freecom.lib : $(CFG) freecom_deps1 freecom_deps2 freecom.rsp
	$(RMFILES) freecom.lib
	$(AR) freecom.lib @freecom.rsp $(LIBLIST) freecom.lst
