# $Id$
#
# Makefile for the FreeCOM library
#
# $Log$
# Revision 1.3.2.3  2001/06/25 20:06:36  skaus
# Update #3
#
# Revision 1.3.2.2  2001/06/21 21:40:35  skaus
# Update #2
#
# Revision 1.3.2.1  2001/06/19 20:42:23  skaus
# Update #1
#
# Revision 1.3  2001/06/10 17:17:24  skaus
# bugfix: Single point of configuration CONFIG.MAK/.H
#
# Revision 1.2  2001/04/29 11:33:51  skaus
# chg: default heap size (tools\ptchsize) set to 6KB
# chg: error displaying functions centralized into lib\err_fcts.src
# add: displayError()
# chg: all errors are displayed through functions void error_*()
# bugfix: somtimes error messages are not displayed (see displayError())
# bugfix: docommand(): type:file must pass ":file" to TYPE
# bugfix: error_sfile(): string _SFILE_
# bugfix: error message on empty redirection
# bugfix: comma and semicolon ';' are recognized as argument seperators
# 	of internal commands
#
# Revision 1.1  2001/04/12 00:33:53  skaus
# chg: new structure
# chg: If DEBUG enabled, no available commands are displayed on startup
# fix: PTCHSIZE also patches min extra size to force to have this amount
#    of memory available on start
# bugfix: CALL doesn't reset options
# add: PTCHSIZE to patch heap size
# add: VSPAWN, /SWAP switch, .SWP resource handling
# bugfix: COMMAND.COM A:\
# bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
# add: command MEMORY
# bugfix: runExtension(): destroys command[-2]
# add: clean.bat
# add: localized CRITER strings
# chg: use LNG files for hard-coded strings (hangForEver(), init.c)
# 	via STRINGS.LIB
# add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
# add: fixstrs.c: prompts & symbolic keys
# add: fixstrs.c: backslash escape sequences
# add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
# chg: splitted code apart into LIB\*.c and CMD\*.c
# bugfix: IF is now using error system & STRINGS to report errors
# add: CALL: /N
#
#

MAXLINELENGTH !:= 4096

.INCLUDE : ../_config.mk

# Sources of this make target
SRC = act_qec.c alloc_ec.c alloc_ed.c alloc_em.c alloc_er.c alloc_es.c \
	alprmblk.c alsysblk.c beep_l.c beep_n.c brk_get.c brk_set.c cbreak.c \
	cbs.c cd_dir.c cgetch.c cgettime.c chgctxt.c chgdrv.c chgenv.c chgenvc.c \
	chgenvr.c clrline.c cmdinput.c comfile.c compfile.c ctxt.c ctxt_adr.c \
	ctxt_as.c ctxt_chg.c ctxt_clr.c ctxt_get.c ctxt_inf.c ctxt_ini.c \
	ctxt_mk.c ctxt_mkn.c ctxt_pop.c ctxt_psh.c ctxt_rnu.c ctxt_ses.c \
	ctxt_set.c ctxt_sp.c ctxt_ss.c ctxt_vw.c curdatel.c curtime.c cwd.c \
	dateget.c dateset.c dbg_c.c dbg_mem.c dbg_prnt.c dbg_s.c dbg_sn.c \
	devopen.c dispcnt.c drvnum.c efct_001.c efct_002.c exec.c exec1.c \
	exp_ev.c farread.c fcompl1.c fcompl2.c fdattr.c fdevopen.c fillcomp.c \
	find.c find_arg.c freep.c frsysblk.c fstpcpy.c gallstr.c get1mcb.c \
	getcnam.c getenv.c goxy.c grabfcom.c gumblink.c hdlrctxt.c hist_get.c \
	hist_set.c inputdos.c is_empty.c is_fnamc.c is_fnstr.c is_icmd.c \
	is_ifct.c is_ivar.c is_num.c is_pchr.c isadev.c ivarset.c keyprsd.c \
	lastdget.c lastdset.c leadopt.c lowexec.asm ltrimcl.c ltrimsp.c lwr1wd.c \
	match.c messages.c mk_rddir.c mktmpfil.c msg_dflt.c msg_dps.c msg_fstr.c \
	msg_get.c msg_gpt.c msg_mkey.c msg_prmp.c mux_ae.c nls.c nls_date.c \
	nls_time.c num_fmt.c onoff.c openf.c optsb.c optsi.c optss.c parsenum.c \
	pr_date.c pr_prmpt.c pr_time.c prprompt.c realnum.c reg_str.c res.c \
	res_r.c res_w.c resfile.c rmtmpfil.c rtrimcl.c rtrimsp.c salloc.c \
	samefile.c scancmd.c scanopt.c set_arg.c showcmds.c skippath.c skqwd.c \
	spfnam.c split.c splitfc.c sumblink.c tempfile.c timeget.c timeset.c \
	tmpnam.c trimcl.c trimsp.c txtlend.c unquote.c vcgetch.c vcgetstr.c \
	where.c
OBJ = act_qec.obj alloc_ec.obj alloc_ed.obj alloc_em.obj alloc_er.obj \
	alloc_es.obj alprmblk.obj alsysblk.obj beep_l.obj beep_n.obj brk_get.obj \
	brk_set.obj cbreak.obj cbs.obj cd_dir.obj cgetch.obj cgettime.obj \
	chgctxt.obj chgdrv.obj chgenv.obj chgenvc.obj chgenvr.obj clrline.obj \
	cmdinput.obj comfile.obj compfile.obj ctxt.obj ctxt_adr.obj ctxt_as.obj \
	ctxt_chg.obj ctxt_clr.obj ctxt_get.obj ctxt_inf.obj ctxt_ini.obj \
	ctxt_mk.obj ctxt_mkn.obj ctxt_pop.obj ctxt_psh.obj ctxt_rnu.obj \
	ctxt_ses.obj ctxt_set.obj ctxt_sp.obj ctxt_ss.obj ctxt_vw.obj \
	curdatel.obj curtime.obj cwd.obj dateget.obj dateset.obj dbg_c.obj \
	dbg_mem.obj dbg_prnt.obj dbg_s.obj dbg_sn.obj devopen.obj dispcnt.obj \
	drvnum.obj efct_001.obj efct_002.obj exec.obj exec1.obj exp_ev.obj \
	farread.obj fcompl1.obj fcompl2.obj fdattr.obj fdevopen.obj fillcomp.obj \
	find.obj find_arg.obj freep.obj frsysblk.obj fstpcpy.obj gallstr.obj \
	get1mcb.obj getcnam.obj getenv.obj goxy.obj grabfcom.obj gumblink.obj \
	hdlrctxt.obj hist_get.obj hist_set.obj inputdos.obj is_empty.obj \
	is_fnamc.obj is_fnstr.obj is_icmd.obj is_ifct.obj is_ivar.obj is_num.obj \
	is_pchr.obj isadev.obj ivarset.obj keyprsd.obj lastdget.obj lastdset.obj \
	leadopt.obj lowexec.obj ltrimcl.obj ltrimsp.obj lwr1wd.obj match.obj \
	messages.obj mk_rddir.obj mktmpfil.obj msg_dflt.obj msg_dps.obj \
	msg_fstr.obj msg_get.obj msg_gpt.obj msg_mkey.obj msg_prmp.obj \
	mux_ae.obj nls.obj nls_date.obj nls_time.obj num_fmt.obj onoff.obj \
	openf.obj optsb.obj optsi.obj optss.obj parsenum.obj pr_date.obj \
	pr_prmpt.obj pr_time.obj prprompt.obj realnum.obj reg_str.obj res.obj \
	res_r.obj res_w.obj resfile.obj rmtmpfil.obj rtrimcl.obj rtrimsp.obj \
	salloc.obj samefile.obj scancmd.obj scanopt.obj set_arg.obj showcmds.obj \
	skippath.obj skqwd.obj spfnam.obj split.obj splitfc.obj sumblink.obj \
	tempfile.obj timeget.obj timeset.obj tmpnam.obj trimcl.obj trimsp.obj \
	txtlend.obj unquote.obj vcgetch.obj vcgetstr.obj where.obj
HDR = ..\config.h ..\context.h_c ..\err_fcts.h ..\include/cmdline.h \
	..\include/command.h ..\include/context.h ..\include/crossjmp.h \
	..\include/datefunc.h ..\include/debug.h ..\include/keys.h \
	..\include/kswap.h ..\include/message.h ..\include/misc.h \
	..\include/mux_ae.h ..\include/nls.h ..\include/openf.h ..\include/res.h \
	..\include/resource.h ..\include/strings.typ ..\include/timefunc.h \
	..\include\large.inc ..\include\model.inc ..\include\small.inc \
	..\include\stuff.inc


#	Default target
all: freecom.lib

# Error functions generated by scanerr.pl
.INCLUDE : err_fcts.mk
SRC += $(SRC_ERR)
OBJ += $(OBJ_ERR)


dist : lib.mak

%.obj : %.asm ; $(NASM) $(NASMFLAGS) -f obj -F borland -o $@ $< >>errlist

### Utils are required by build process
.INIT .PHONY .SEQUENTIAL : verscheck $(CFG) __errl

.IF $(THISMAKE) == $(LASTMAKE)
verscheck :;
.ELSE
verscheck : 
	@+-echo Changed Make parameters, already made files invalid!
	+-echo LASTMAKE = $(THISMAKE) >lastmake.mk
	$(RUNMAKE) clobber
.IF $(CFG)
	$(RUNMAKE) $(CFG)
.ENDIF
.ENDIF

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c ..\cmd\*.c ..\shell\*.c $(FREEDOS)\SRC\LIB\SUPPL\*.c

freecom.lib .LIBRARY : $(OBJ)

#MAKEDEP START
onoff.obj : onoff.c \
	 ../config.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
lastdget.obj : lastdget.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
gumblink.obj : gumblink.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
pr_date.obj : pr_date.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/nls.h ../include/timefunc.h
find.obj : find.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
ctxt_clr.obj : ctxt_clr.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
ctxt.obj : ctxt.c \
	 ../config.h ../include/context.h ../include/debug.h
lastdset.obj : lastdset.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
beep_l.obj : beep_l.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
is_fnstr.obj : is_fnstr.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
ctxt_ss.obj : ctxt_ss.c \
	 ../config.h ../include/context.h ../include/debug.h
cbreak.obj : cbreak.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h ../strings.h
getenv.obj : getenv.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
beep_n.obj : beep_n.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
res.obj : res.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/resource.h ../include/timefunc.h
parsenum.obj : parsenum.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
dbg_prnt.obj : dbg_prnt.c \
	 ../config.h ../include/debug.h
fcompl2.obj : fcompl2.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
efct_002.obj : efct_002.c \
	 ../config.h ../err_fcts.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h \
	../strings.h
hist_get.obj : hist_get.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
fdevopen.obj : fdevopen.c \
	 ../include/openf.h
ctxt_rnu.obj : ctxt_rnu.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
chgenv.obj : chgenv.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h ../strings.h
drvnum.obj : drvnum.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
rtrimcl.obj : rtrimcl.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
mux_ae.obj : mux_ae.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/crossjmp.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/mux_ae.h ../include/timefunc.h
getcnam.obj : getcnam.c \
	 ../config.h ../err_fcts.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
comfile.obj : comfile.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
hist_set.obj : hist_set.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
get1mcb.obj : get1mcb.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
msg_prmp.obj : msg_prmp.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
cd_dir.obj : cd_dir.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h ../strings.h
exp_ev.obj : exp_ev.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
trimcl.obj : trimcl.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
dbg_mem.obj : dbg_mem.c \
	 ../config.h ../include/debug.h
is_ivar.obj : is_ivar.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
hdlrctxt.obj : hdlrctxt.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
ctxt_ses.obj : ctxt_ses.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
ctxt_psh.obj : ctxt_psh.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
optsi.obj : optsi.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
rtrimsp.obj : rtrimsp.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
vcgetstr.obj : vcgetstr.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
alloc_ec.obj : alloc_ec.c \
	 ../err_fcts.h ../include/datefunc.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
farread.obj : farread.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
alprmblk.obj : alprmblk.c \
	 ../config.h ../context.h_c ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/kswap.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
scanopt.obj : scanopt.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
openf.obj : openf.c \
	 ../config.h ../include/debug.h ../include/openf.h
messages.obj : messages.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/res.h \
	../include/resource.h ../include/strings.typ ../include/timefunc.h \
	../strings.h
scancmd.obj : scancmd.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
optss.obj : optss.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
ctxt_mk.obj : ctxt_mk.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/crossjmp.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
where.obj : where.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
isadev.obj : isadev.c \
	 ../include/openf.h
alloc_em.obj : alloc_em.c \
	 ../err_fcts.h ../include/datefunc.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
trimsp.obj : trimsp.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
res_w.obj : res_w.c \
	 ../include/resource.h
alloc_es.obj : alloc_es.c \
	 ../err_fcts.h ../include/datefunc.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
ctxt_ini.obj : ctxt_ini.c \
	 ../config.h ../include/context.h ../include/debug.h
skippath.obj : skippath.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
mktmpfil.obj : mktmpfil.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
cmdinput.obj : cmdinput.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/keys.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
exec.obj : exec.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
fdattr.obj : fdattr.c
goxy.obj : goxy.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
clrline.obj : clrline.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
fstpcpy.obj : fstpcpy.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
tmpnam.obj : tmpnam.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
frsysblk.obj : frsysblk.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
unquote.obj : unquote.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
cgettime.obj : cgettime.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
freep.obj : freep.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
realnum.obj : realnum.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
tempfile.obj : tempfile.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
inputdos.obj : inputdos.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
is_icmd.obj : is_icmd.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
chgenvc.obj : chgenvc.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
dateget.obj : dateget.c \
	 ../config.h ../include/datefunc.h ../include/debug.h
lwr1wd.obj : lwr1wd.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
is_ifct.obj : is_ifct.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
keyprsd.obj : keyprsd.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
spfnam.obj : spfnam.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
showcmds.obj : showcmds.c \
	 ../config.h ../context.h_c ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/kswap.h ../include/message.h \
	../include/misc.h ../include/timefunc.h ../strings.h
is_pchr.obj : is_pchr.c
is_empty.obj : is_empty.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
dateset.obj : dateset.c \
	 ../config.h ../include/datefunc.h ../include/debug.h
split.obj : split.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
nls.obj : nls.c \
	 ../config.h ../include/debug.h ../include/nls.h
match.obj : match.c \
	 ../config.h ../include/cmdline.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
timeget.obj : timeget.c \
	 ../config.h ../include/debug.h ../include/timefunc.h
compfile.obj : compfile.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
dbg_c.obj : dbg_c.c \
	 ../config.h ../include/debug.h
devopen.obj : devopen.c \
	 ../include/openf.h
splitfc.obj : splitfc.c \
	 ../config.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/nls.h ../include/timefunc.h
num_fmt.obj : num_fmt.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
lowexec.obj : lowexec.asm \
	 .\..\include\large.inc .\..\include\model.inc .\..\include\small.inc \
	.\..\include\stuff.inc
ctxt_adr.obj : ctxt_adr.c \
	 ../config.h ../include/context.h ../include/debug.h
timeset.obj : timeset.c \
	 ../config.h ../include/debug.h ../include/timefunc.h
nls_time.obj : nls_time.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/nls.h \
	../include/timefunc.h ../strings.h
cwd.obj : cwd.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
dbg_s.obj : dbg_s.c \
	 ../config.h ../include/debug.h
is_fnamc.obj : is_fnamc.c \
	 ../config.h ../include/debug.h ../include/nls.h
grabfcom.obj : grabfcom.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
prprompt.obj : prprompt.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
sumblink.obj : sumblink.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
set_arg.obj : set_arg.c \
	 ../config.h ../include/cmdline.h ../include/command.h \
	../include/context.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
ctxt_sp.obj : ctxt_sp.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
msg_fstr.obj : msg_fstr.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/strings.typ \
	../include/timefunc.h ../strings.h
skqwd.obj : skqwd.c \
	 ../config.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
is_num.obj : is_num.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
mk_rddir.obj : mk_rddir.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h ../strings.h
act_qec.obj : act_qec.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
msg_gpt.obj : msg_gpt.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
efct_001.obj : efct_001.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h ../strings.h
fcompl1.obj : fcompl1.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
curdatel.obj : curdatel.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/nls.h ../include/timefunc.h ../strings.h
reg_str.obj : reg_str.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
exec1.obj : exec1.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
ltrimcl.obj : ltrimcl.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
ctxt_get.obj : ctxt_get.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
txtlend.obj : txtlend.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
vcgetch.obj : vcgetch.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/keys.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
msg_get.obj : msg_get.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
optsb.obj : optsb.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
resfile.obj : resfile.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
rmtmpfil.obj : rmtmpfil.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
ltrimsp.obj : ltrimsp.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
cgetch.obj : cgetch.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/keys.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
ctxt_set.obj : ctxt_set.c \
	 ../config.h ../include/context.h ../include/debug.h
pr_time.obj : pr_time.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/nls.h ../include/timefunc.h
ctxt_pop.obj : ctxt_pop.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
alloc_ed.obj : alloc_ed.c \
	 ../err_fcts.h ../include/datefunc.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
dispcnt.obj : dispcnt.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
ctxt_as.obj : ctxt_as.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
gallstr.obj : gallstr.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
find_arg.obj : find_arg.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
res_r.obj : res_r.c \
	 ../config.h ../include/debug.h ../include/resource.h
msg_mkey.obj : msg_mkey.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
brk_get.obj : brk_get.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
ivarset.obj : ivarset.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
ctxt_inf.obj : ctxt_inf.c \
	 ../config.h ../include/context.h ../include/debug.h
alloc_er.obj : alloc_er.c \
	 ../err_fcts.h ../include/datefunc.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
brk_set.obj : brk_set.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
nls_date.obj : nls_date.c \
	 ../config.h ../include/debug.h ../include/nls.h
dbg_sn.obj : dbg_sn.c \
	 ../config.h ../include/debug.h
msg_dps.obj : msg_dps.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
cbs.obj : cbs.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
curtime.obj : curtime.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/nls.h ../include/timefunc.h
salloc.obj : salloc.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
chgdrv.obj : chgdrv.c \
	 ../config.h ../err_fcts.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h \
	../strings.h
alsysblk.obj : alsysblk.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
samefile.obj : samefile.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
ctxt_mkn.obj : ctxt_mkn.c \
	 ../config.h ../include/context.h ../include/debug.h
leadopt.obj : leadopt.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
chgenvr.obj : chgenvr.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h ../strings.h
ctxt_vw.obj : ctxt_vw.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h ../strings.h
msg_dflt.obj : msg_dflt.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h \
	../strings.h
ctxt_chg.obj : ctxt_chg.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
pr_prmpt.obj : pr_prmpt.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
chgctxt.obj : chgctxt.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
fillcomp.obj : fillcomp.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
DYNSOURCES =
#MAKEDEP STOP

clobber : clean
	$(RM) $(RMFLAGS) *.com *.cln *.lib *.lst *.cfg lib.mak

clean : 
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj *.exe $(CFG) *.dmp com.com tags errlist lib.m3


.IF $(CFG) != $(NULL)

#CONFIGURATION = $(CONF_BASE) $(null,$(NDEBUG) $(CONF_DBG) $(CONF_NDBG))
CONFIGURATION = $(CONF_BASE)

.IF $(_COMPTYPE) == BC
CONF_BASE =	\
-f- \
-I$(INCDIR:s/;/ /:t";")	\
-L$(LIBDIR:s/;/ /:t";")	\
-w

.IF $(_COMPILER)==BC5
CONFIGURATION += -RT- -x-
.ENDIF

CONF_DBG =	$(MYCFLAGS_DBG)

CONF_NDBG =	$(MYCFLAGS_NDBG)

.ENDIF

.IF $(_COMPTYPE) == TC
CONF_BASE =	\
-I$(INCDIR:s/;/ /:t";")	\
-L$(LIBDIR:s/;/ /:t";")	\
-w

# The "-f-" causes that TCC passes option "/r" to the assembler, though
# ASM chokes about it

CONF_DBG =	$(MYCFLAGS_DBG)

CONF_NDBG =	$(MYCFLAGS_NDBG)

.ENDIF


.ENDIF
#-weas	\
#-wpre	\
#-w-pro	\

lib.mak : makefile lib.m1 lib.m2
	..\utils\chunk freecom_deps <$(mktmp $(OBJ:t"\n")\n) >lib.m3
	+copy lib.m1 + lib.m3 + $(mktmp freecom.lib : \44(CFG) freecom_deps \n\tif exist freecom.lib \44(AR) freecom.lib /c @&&|\n+-$(OBJ:t" &\\n+-")\n| , freecom.lst \n\tif not exist freecom.lib \44(AR) freecom.lib /c @&&|\n+$(OBJ:t" &\\n+")\n| , freecom.lst \n) + lib.m2 $@
#	+copy lib.m1 + $(mktmp freecom.lib : \44(CFG) $(OBJ:t" \\\\\\n\\t") \n\tif exist freecom.lib \44(AR) freecom.lib /c @&&|\n+-$(OBJ:t" &\\n+-")\n| , freecom.lst \n\tif not exist freecom.lib \44(AR) freecom.lib /c @&&|\n+$(OBJ:t" &\\n+")\n| , freecom.lst \n) + lib.m2 $@

