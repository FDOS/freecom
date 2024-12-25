# $Id$
#
# Makefile for the FreeCOM library
#
# $Log$
# Revision 1.17  2004/12/01 21:15:31  skaus
# add: /Z: Display exit code after external command {Bernd Blaauw}
#
# Revision 1.16  2004/09/13 18:59:39  skaus
# add: CRITER: Repeat check autofail magic {Tom Ehlert/Eric Auer}
#
# Revision 1.15  2004/06/21 17:49:25  skaus
# fix: DIR: disable APPEND.EXE during DIR processing {Eduardo Almao}
#
# Revision 1.14  2004/02/01 13:24:22  skaus
# bugfix: misidentifying unspecific failures from within SUPPL
#
# Revision 1.13  2003/12/09 21:29:24  skaus
# bugfix: Ask for FreeCOM location when STRINGS are missing [#687]
#
# Revision 1.12  2003/12/09 20:25:34  skaus
# bugfix: INIT: FreeCOM-path and TTY can be in any order
#
# Revision 1.11  2003/09/01 14:14:30  skaus
# bugfix: typo: bad varable specification [#1678]
#
# Revision 1.10  2002/11/12 19:48:47  skaus
# bugfix: COPY: additional output to honor redirection {Eric Auer} [bugID #1313]
#
# Revision 1.9  2002/04/25 18:50:40  skaus
# v0.83 Beta 38:
#
# Revision 1.8  2002/04/23 16:40:41  skaus
# bugfix: CTTY: added to try to set the device flags
#
# Revision 1.7  2002/04/02 23:36:37  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
#
# Revision 1.6  2002/04/02 20:58:13  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
#
# Revision 1.5  2002/04/02 18:09:31  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
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
SRC = absfile.c almemblk.c alprmblk.c alsysblk.c app_get.c app_set.c \
	beep_l.c beep_n.c brk_get.c brk_set.c cbreak.c cbs.c cd_dir.c cgetch.c \
	cgettime.c chgctxt.c chgdrv.c chgenv.c chgenvc.c chgenvr.c \
	cmdinput.c comfile.c compfile.c critend.c critrchk.c ctxt.c ctxt_adr.c \
	ctxt_as.c ctxt_chg.c ctxt_clr.c ctxt_get.c ctxt_inf.c ctxt_mk.c \
	ctxt_mkb.c ctxt_mkn.c ctxt_pop.c ctxt_psh.c ctxt_rnu.c ctxt_set.c \
	ctxt_ss.c ctxt_vw.c curdatel.c curtime.c cwd.c dateget.c dateset.c \
	dbg_c.c dbg_mem.c dbg_prnt.c dbg_s.c dbg_sn.c devopen.c dfn_err.c \
	dispcnt.c dispexit.c drvnum.c efct_001.c exec.c exec1.c farread.c \
	fcompl1.c fcompl2.c fdattr.c fdevopen.c fdsattr.c fillcomp.c find.c \
	freep.c frsysblk.c fstpcpy.c gallstr.c get1mcb.c getenv.c goxy.c \
	grabfcom.c gumblink.c hdlrctxt.c hist_get.c hist_set.c inputdos.c \
	is_empty.c is_fnamc.c is_fnstr.c is_pchr.c isadev.c keyprsd.c kswap_c.c \
	lastdget.c lastdset.c leadopt.c lowexec.asm ltrimcl.c ltrimsp.c lwr1wd.c \
	match.c messages.c mk_rddir.c mktmpfil.c msg_dflt.c msg_dps.c msg_fstr.c \
	msg_get.c msg_gpt.c msg_mkey.c msg_prmp.c mux_ae.c nls.c nls_date.c \
	nls_time.c num_fmt.c onoff.c openf.c optsb.c optsi.c optss.c parsenum.c \
	pr_date.c pr_prmpt.c pr_time.c prprompt.c readcmd.c realnum.c res.c \
	res_r.c res_vald.c res_w.c resfile.c rmtmpfil.c rtrimcl.c rtrimsp.c \
	salloc.c samefile.c scancmd.c scanopt.c session.c showcmds.c skqwd.c \
	spfnam.c split.c sumblink.c timeget.c timeset.c tmpnam.c \
	trimcl.c trimsp.c truepath.c truncate.c txtlend.c unquote.c vcgetch.c \
	vcgetstr.c where.c
OBJ = absfile.obj almemblk.obj alprmblk.obj alsysblk.obj app_get.obj \
	app_set.obj beep_l.obj beep_n.obj brk_get.obj brk_set.obj cbreak.obj \
	cbs.obj cd_dir.obj cgetch.obj cgettime.obj chgctxt.obj chgdrv.obj \
	chgenv.obj chgenvc.obj chgenvr.obj cmdinput.obj comfile.obj \
	compfile.obj critend.obj critrchk.obj ctxt.obj ctxt_adr.obj ctxt_as.obj \
	ctxt_chg.obj ctxt_clr.obj ctxt_get.obj ctxt_inf.obj ctxt_mk.obj \
	ctxt_mkb.obj ctxt_mkn.obj ctxt_pop.obj ctxt_psh.obj ctxt_rnu.obj \
	ctxt_set.obj ctxt_ss.obj ctxt_vw.obj curdatel.obj curtime.obj cwd.obj \
	dateget.obj dateset.obj dbg_c.obj dbg_mem.obj dbg_prnt.obj dbg_s.obj \
	dbg_sn.obj devopen.obj dfn_err.obj dispcnt.obj dispexit.obj drvnum.obj \
	efct_001.obj exec.obj exec1.obj farread.obj fcompl1.obj fcompl2.obj \
	fdattr.obj fdevopen.obj fdsattr.obj fillcomp.obj find.obj freep.obj \
	frsysblk.obj fstpcpy.obj gallstr.obj get1mcb.obj getenv.obj goxy.obj \
	grabfcom.obj gumblink.obj hdlrctxt.obj hist_get.obj hist_set.obj \
	inputdos.obj is_empty.obj is_fnamc.obj is_fnstr.obj is_pchr.obj \
	isadev.obj keyprsd.obj kswap_c.obj lastdget.obj lastdset.obj leadopt.obj \
	lowexec.obj ltrimcl.obj ltrimsp.obj lwr1wd.obj match.obj messages.obj \
	mk_rddir.obj mktmpfil.obj msg_dflt.obj msg_dps.obj msg_fstr.obj \
	msg_get.obj msg_gpt.obj msg_mkey.obj msg_prmp.obj mux_ae.obj nls.obj \
	nls_date.obj nls_time.obj num_fmt.obj onoff.obj openf.obj optsb.obj \
	optsi.obj optss.obj parsenum.obj pr_date.obj pr_prmpt.obj pr_time.obj \
	prprompt.obj readcmd.obj realnum.obj res.obj res_r.obj res_vald.obj \
	res_w.obj resfile.obj rmtmpfil.obj rtrimcl.obj rtrimsp.obj salloc.obj \
	samefile.obj scancmd.obj scanopt.obj session.obj showcmds.obj skqwd.obj \
	spfnam.obj split.obj sumblink.obj timeget.obj timeset.obj \
	tmpnam.obj trimcl.obj trimsp.obj truepath.obj truncate.obj txtlend.obj \
	unquote.obj vcgetch.obj vcgetstr.obj where.obj
HDR = ..\config.h ..\context.h_c ..\err_fcts.h ..\include/batch.h \
	..\include/cmdline.h ..\include/command.h ..\include/context.h \
	..\include/crossjmp.h ..\include/cswap.h ..\include/datefunc.h \
	..\include/debug.h ..\include/keys.h ..\include/kswap.h \
	..\include/misc.h ..\include/mux_ae.h ..\include/nls.h \
	..\include/openf.h ..\include/res.h ..\include/resource.h \
	..\include/strings.typ ..\include/timefunc.h ..\include\command.h \
	..\include\context.h ..\include\crossjmp.h ..\include\large.inc \
	..\include\misc.h ..\include\model.inc ..\include\small.inc \
	..\include\stuff.inc


#	Default target
all: freecom.lib

# Error functions generated by scanerr.pl
.INCLUDE : err_fcts.mk
SRC += $(SRC_ERR)
OBJ += $(OBJ_ERR)


dist : makefile.mak

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
absfile.obj : absfile.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
almemblk.obj : almemblk.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
alprmblk.obj : alprmblk.c \
	 ../config.h ../context.h_c ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/kswap.h ../include/misc.h \
	../include/timefunc.h
alsysblk.obj : alsysblk.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
app_get.obj : app_get.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
app_set.obj : app_set.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
beep_l.obj : beep_l.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
beep_n.obj : beep_n.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
brk_get.obj : brk_get.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
brk_set.obj : brk_set.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
cbreak.obj : cbreak.c \
	 ../config.h ../include/batch.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
cbs.obj : cbs.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
cd_dir.obj : cd_dir.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
cgetch.obj : cgetch.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/keys.h ../include/misc.h \
	../include/timefunc.h
cgettime.obj : cgettime.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
chgctxt.obj : chgctxt.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
chgdrv.obj : chgdrv.c \
	 ../config.h ../err_fcts.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h ../strings.h
chgenv.obj : chgenv.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
chgenvc.obj : chgenvc.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
chgenvr.obj : chgenvr.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
cmdinput.obj : cmdinput.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/keys.h ../include/misc.h \
	../include/timefunc.h
comfile.obj : comfile.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
compfile.obj : compfile.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
critend.obj : critend.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
critrchk.obj : critrchk.c \
	 ../config.h ../err_fcts.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
ctxt.obj : ctxt.c \
	 ../config.h ../include/context.h ../include/debug.h
ctxt_adr.obj : ctxt_adr.c \
	 ../config.h ../include/context.h ../include/debug.h
ctxt_as.obj : ctxt_as.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
ctxt_chg.obj : ctxt_chg.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
ctxt_clr.obj : ctxt_clr.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
ctxt_get.obj : ctxt_get.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
ctxt_inf.obj : ctxt_inf.c \
	 ../config.h ../include/context.h ../include/debug.h
ctxt_mk.obj : ctxt_mk.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/crossjmp.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
ctxt_mkb.obj : ctxt_mkb.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/crossjmp.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
ctxt_mkn.obj : ctxt_mkn.c \
	 ../config.h ../include/context.h ../include/debug.h
ctxt_pop.obj : ctxt_pop.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
ctxt_psh.obj : ctxt_psh.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
ctxt_rnu.obj : ctxt_rnu.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
ctxt_set.obj : ctxt_set.c \
	 ../config.h ../include/context.h ../include/debug.h
ctxt_ss.obj : ctxt_ss.c \
	 ../config.h ../include/context.h ../include/debug.h
ctxt_vw.obj : ctxt_vw.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
curdatel.obj : curdatel.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/nls.h \
	../include/timefunc.h ../strings.h
curtime.obj : curtime.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/nls.h \
	../include/timefunc.h
cwd.obj : cwd.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
dateget.obj : dateget.c \
	 ../config.h ../include/datefunc.h ../include/debug.h
dateset.obj : dateset.c \
	 ../config.h ../include/datefunc.h ../include/debug.h
dbg_c.obj : dbg_c.c \
	 ../config.h ../include/debug.h
dbg_mem.obj : dbg_mem.c \
	 ../config.h ../include/debug.h
dbg_prnt.obj : dbg_prnt.c \
	 ../config.h ../include/debug.h
dbg_s.obj : dbg_s.c \
	 ../config.h ../include/debug.h
dbg_sn.obj : dbg_sn.c \
	 ../config.h ../include/debug.h
devopen.obj : devopen.c \
	 ../include/openf.h
dfn_err.obj : dfn_err.c \
	 ../config.h ../err_fcts.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
dispcnt.obj : dispcnt.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
dispexit.obj : dispexit.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
drvnum.obj : drvnum.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
efct_001.obj : efct_001.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
exec.obj : exec.c \
	 ../config.h ../include/command.h ../include/cswap.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/nls.h ../include/timefunc.h
exec1.obj : exec1.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
farread.obj : farread.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
fcompl1.obj : fcompl1.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
fcompl2.obj : fcompl2.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
fdattr.obj : fdattr.c
fdevopen.obj : fdevopen.c \
	 ../include/openf.h
fdsattr.obj : fdsattr.c
fillcomp.obj : fillcomp.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
find.obj : find.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
freep.obj : freep.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
frsysblk.obj : frsysblk.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
fstpcpy.obj : fstpcpy.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
gallstr.obj : gallstr.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
get1mcb.obj : get1mcb.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
getenv.obj : getenv.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
goxy.obj : goxy.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
grabfcom.obj : grabfcom.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
gumblink.obj : gumblink.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
hdlrctxt.obj : hdlrctxt.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
hist_get.obj : hist_get.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
hist_set.obj : hist_set.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
inputdos.obj : inputdos.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
is_empty.obj : is_empty.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
is_fnamc.obj : is_fnamc.c \
	 ../config.h ../include/debug.h ../include/nls.h
is_fnstr.obj : is_fnstr.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
is_pchr.obj : is_pchr.c
isadev.obj : isadev.c \
	 ../include/openf.h
keyprsd.obj : keyprsd.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
kswap_c.obj : kswap_c.c \
	 ../config.h ../context.h_c ../include/debug.h ../include/kswap.h
lastdget.obj : lastdget.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
lastdset.obj : lastdset.c \
	 ../config.h ../err_fcts.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
leadopt.obj : leadopt.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
lowexec.obj : lowexec.asm \
	 .\..\include\large.inc .\..\include\model.inc .\..\include\small.inc \
	.\..\include\stuff.inc
ltrimcl.obj : ltrimcl.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
ltrimsp.obj : ltrimsp.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
lwr1wd.obj : lwr1wd.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
match.obj : match.c \
	 ../config.h ../include/cmdline.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
messages.obj : messages.c \
	 ../config.h ../include/command.h ../include/cswap.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/res.h ../include/resource.h ../include/strings.typ \
	../include/timefunc.h ../strings.h
mk_rddir.obj : mk_rddir.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
mktmpfil.obj : mktmpfil.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
msg_dflt.obj : msg_dflt.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h ../strings.h
msg_dps.obj : msg_dps.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
msg_fstr.obj : msg_fstr.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/strings.typ ../include/timefunc.h \
	../strings.h
msg_get.obj : msg_get.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
msg_gpt.obj : msg_gpt.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
msg_mkey.obj : msg_mkey.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
msg_prmp.obj : msg_prmp.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
mux_ae.obj : mux_ae.c \
	 ../config.h ../include/command.h ../include/crossjmp.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/mux_ae.h ../include/nls.h ../include/timefunc.h
nls.obj : nls.c \
	 ../config.h ../include/debug.h ../include/nls.h
nls_date.obj : nls_date.c \
	 ../config.h ../include/debug.h ../include/nls.h
nls_time.obj : nls_time.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/nls.h ../include/timefunc.h ../strings.h
num_fmt.obj : num_fmt.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/nls.h ../include/timefunc.h
onoff.obj : onoff.c \
	 ../config.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
openf.obj : openf.c \
	 ../config.h ../include/debug.h ../include/openf.h
optsb.obj : optsb.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
optsi.obj : optsi.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
optss.obj : optss.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
parsenum.obj : parsenum.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
pr_date.obj : pr_date.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/nls.h \
	../include/timefunc.h
pr_prmpt.obj : pr_prmpt.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
pr_time.obj : pr_time.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/nls.h \
	../include/timefunc.h
prprompt.obj : prprompt.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
readcmd.obj : readcmd.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
realnum.obj : realnum.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
res.obj : res.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/resource.h \
	../include/timefunc.h
res_r.obj : res_r.c \
	 ../config.h ../include/debug.h ../include/resource.h
res_vald.obj : res_vald.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/openf.h \
	../include/resource.h ../include/strings.typ ../include/timefunc.h
res_w.obj : res_w.c \
	 ../include/resource.h
resfile.obj : resfile.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
rmtmpfil.obj : rmtmpfil.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
rtrimcl.obj : rtrimcl.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
rtrimsp.obj : rtrimsp.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
salloc.obj : salloc.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
samefile.obj : samefile.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
scancmd.obj : scancmd.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
scanopt.obj : scanopt.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
session.obj : session.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/crossjmp.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
showcmds.obj : showcmds.c \
	 ../config.h ../context.h_c ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/kswap.h ../include/misc.h \
	../include/timefunc.h ../strings.h
skqwd.obj : skqwd.c \
	 ../config.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
spfnam.obj : spfnam.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
split.obj : split.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
sumblink.obj : sumblink.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
timeget.obj : timeget.c \
	 ../config.h ../include/debug.h ../include/timefunc.h
timeset.obj : timeset.c \
	 ../config.h ../include/debug.h ../include/timefunc.h
tmpnam.obj : tmpnam.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
trimcl.obj : trimcl.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
trimsp.obj : trimsp.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
truepath.obj : truepath.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
truncate.obj : truncate.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
txtlend.obj : txtlend.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
unquote.obj : unquote.c \
	 ../config.h ../include/cmdline.h ../include/debug.h
vcgetch.obj : vcgetch.c \
	 ../config.h ../include/datefunc.h ../include/debug.h \
	../include/keys.h ../include/misc.h ../include/timefunc.h
vcgetstr.obj : vcgetstr.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
where.obj : where.c \
	 ../config.h ../err_fcts.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
DYNSOURCES =
#MAKEDEP STOP

clobber : clean
	$(RM) $(RMFLAGS) *.com *.cln *.lib *.lst *.cfg makefile.mak

clean : 
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj *.exe $(CFG) *.dmp com.com tags errlist lib.m3


makefile.mak : makefile lib.m1 lib.m2
	..\utils\chunk freecom_deps <$(mktmp $(OBJ:t"\n")\n) >lib.m3
	+copy lib.m1 + lib.m3 + $(mktmp freecom.lib : \44(CFG) freecom_deps \n\tif exist freecom.lib \44(AR) freecom.lib /c @&&|\n+-$(OBJ:t" &\\n+-")\n| , freecom.lst \n\tif not exist freecom.lib \44(AR) freecom.lib /c @&&|\n+$(OBJ:t" &\\n+")\n| , freecom.lst \n) + lib.m2 $@
#	+copy lib.m1 + $(mktmp freecom.lib : \44(CFG) $(OBJ:t" \\\\\\n\\t") \n\tif exist freecom.lib \44(AR) freecom.lib /c @&&|\n+-$(OBJ:t" &\\n+-")\n| , freecom.lst \n\tif not exist freecom.lib \44(AR) freecom.lib /c @&&|\n+$(OBJ:t" &\\n+")\n| , freecom.lst \n) + lib.m2 $@

