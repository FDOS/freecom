# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.4.4.1.2.1  2000/12/17 21:57:36  skaus
# intermediate update 1
#
# Revision 1.4.4.1  2000/07/19 20:28:29  skaus
# Experimental Resources, Modules, Context
#
# Revision 1.4  2000/07/10 22:15:58  skaus
# Change internal strings without re-compiling FreeCOM
#
# Revision 1.3  2000/07/09 21:57:25  skaus
# + Support for international strings without recompiling
# + Useage of TC++1
#
# Revision 1.1.1.1  2000/05/20 03:29:38  jimtabor
# The GNU GPL FreeCom Command Shell
#
# Revision 1.2  2000/03/30 14:07:03  jprice
# Merge in changes from Steffen's 0.78ska2 code
#
# Revision 1.1  1999/09/20 23:56:36  jprice
# Steffen Kaiser patches
#

FDBIN *= D:\FREEDOS\SRC\BIN
INCDIR +=;$(FREEDOS)\SRC\INCLUDE
LIBDIR +=;$(FREEDOS)\SRC\LIB\$(_COMPILER)
LDLIBS = suppl_$(_MODEL).lib
LDFLAGS += /msl
NASM *= c:\TOOL\NASMW.EXE

# Project specific C compiler flags
MYCFLAGS_DBG = -DDEBUG=1
MYCFLAGS_NDBG =
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))

.SOURCE.lib := $(LIBDIR:s/;/ /:u)
.IMPORT : FREEDOS
.IMPORT .IGNORE : LNG

.INCLUDE .IGNORE : lastmake.mk

THISMAKE !:= $(_COMPILER):$(_MODEL):$(LNG):$(null,$(NDEBUG) DBG NDBG)

# Sources of this make target
SRC = alias.c batch.c beep.c break.c call.c cls.c cmdinput.c cmdline.c \
	cmdtable.c command.c copy.c ctty.c \
	d:\freedos\src\cvs~1.spa\freecom\makdep.opt date.c datefunc.c debug.c \
	del.c dir.c dstack.c echo.c environ.c error.c exec.c fddebug.c \
	filecomp.c for.c goto.c history.c if.c init.c internal.c lh.asm \
	loadhigh.c lowexec.asm messages.c misc.c mod_cbrk.c mod_crit.c \
	mod_rspn.c module.c nls.c openf.c parsenum.c path.c pause.c prompt.c \
	redir.c ren.c res.c res_r.c session.c set.c shift.c tempfile.c time.c \
	timefunc.c tmpnam.c truename.c type.c ver.c verify.c where.c
OBJ = alias.obj batch.obj beep.obj break.obj call.obj cls.obj cmdinput.obj \
	cmdline.obj cmdtable.obj command.obj copy.obj ctty.obj date.obj \
	datefunc.obj debug.obj del.obj dir.obj dstack.obj echo.obj environ.obj \
	error.obj exec.obj fddebug.obj filecomp.obj for.obj goto.obj history.obj \
	if.obj init.obj internal.obj lh.obj loadhigh.obj lowexec.obj \
	messages.obj misc.obj mod_cbrk.obj mod_crit.obj mod_rspn.obj module.obj \
	nls.obj openf.obj parsenum.obj path.obj pause.obj prompt.obj redir.obj \
	ren.obj res.obj res_r.obj session.obj set.obj shift.obj tempfile.obj \
	time.obj timefunc.obj tmpnam.obj truename.obj type.obj ver.obj \
	verify.obj where.obj
HDR = alias.h batch.h cb_catch.ver cmdline.h command.h config.h context.h \
	context.h_c criter.ver datefunc.h debug.h dmy_cbrk.inc dmy_crit.inc \
	error.h loadhigh.h misc.h model.def module.h nls.h openf.h res.h \
	resource.h rsp_fail.ver session.h strings.h strings.typ tempfile.h \
	timefunc.h


#	Default target
all: com.com


.INIT .PHONY .SEQUENTIAL : verscheck $(CFG) __errl tools

.IF $(THISMAKE) == $(LASTMAKE)
verscheck :;
.ELSE
verscheck : 
	@+-echo Changed Make parameters, already made files invalid!
	+-echo LASTMAKE = $(THISMAKE) >lastmake.mk
	$(MAKE) clobber
.IF $(CFG)
	$(MAKE) $(CFG)
.ENDIF
.ENDIF

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c

com.exe ?= _OBJS := $(OBJ:s/c0.obj//)
# STD_OBJ !:= C0.OBJ
com.map com.exe .UPDATEALL : $(OBJ)
	$(MAK_EXE)

com.com : com.exe strings.dat strings.err criter cb_catch
	@+copy /b com.exe + cb_catch + criter + strings.err + strings.dat $@

strings.err strings.h strings.dat .UPDATEALL .SETDIR=strings : default.lng
	@+echo ==Entering $(PWD)
	$(MAKE) LNG=$(LNG) COMPILER=$(_COMPILER) MODEL=$(_MODEL) all
	@+echo ==Leaving $(PWD)

strings_clean .SETDIR=strings :
	@+echo ==Entering $(PWD)
	$(MAKE) clean
	@+echo ==Leaving $(PWD)

strings_clobber .SETDIR=strings :
	@+echo ==Entering $(PWD)
	$(MAKE) clobber
	@+echo ==Leaving $(PWD)

tools .SETDIR=tools .UPDATEALL :
	@+echo ==Entering $(PWD)
	$(MAKE) LNG=$(LNG) COMPILER=$(_COMPILER) MODEL=$(_MODEL) all
	@+echo ==Leaving $(PWD)

tools_clean .SETDIR=tools :
	@+echo ==Entering $(PWD)
	$(MAKE) clean
	@+echo ==Leaving $(PWD)

tools_clobber .SETDIR=tools :
	@+echo ==Entering $(PWD)
	$(MAKE) clobber
	@+echo ==Leaving $(PWD)

global.h_c global.inc global.def .UPDATEALL : global.x
	tools\mkctxt global.x

context.h_c context.inc context.def .UPDATEALL : context.x
	tools\mkctxt context.x

context.h : context.h_c

mkres.c : resource.h
	tools\mkresid.exe <resource.h >mkres.c

resource.id : mkres.exe resource.h
	mkres.exe >$@

rsp_fail stubs criter cb_catch : context.inc resource.inc resource.id

dmy_crit : dmy_crit.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $<

dmy_crit.inc : dmy_crit
	$(FDBIN)\Bin2c /a criter_dummy $< >$@

criter : criter.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $<

cb_catch : cb_catch.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $<

dmy_cbrk : dmy_cbrk.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $<

dmy_cbrk.inc : dmy_cbrk
	$(FDBIN)\Bin2c /a cbreak_dummy $< >$@

stubs : stubs.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $<

rsp_fail : rsp_fail.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $<

error.h : error.c
	perl mkerror.pl $& >$@

#MAKEDEP START
lowexec.obj : lowexec.asm \
	 .\model.def
openf.obj : openf.c \
	 openf.h
messages.obj : messages.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	/freedos/src/include/suppl.h command.h config.h context.h context.h_c \
	debug.h error.h misc.h res.h resource.h strings.h strings.typ
goto.obj : goto.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h strings.h
dstack.obj : dstack.c \
	 /freedos/src/include/dfn.h /freedos/src/include/p-bc.h \
	/freedos/src/include/p-pac.h /freedos/src/include/p-watcom.h \
	/freedos/src/include/portable.h command.h config.h context.h context.h_c \
	debug.h error.h misc.h strings.h
where.obj : where.c \
	 /freedos/src/include/dfn.h /freedos/src/include/p-bc.h \
	/freedos/src/include/p-pac.h /freedos/src/include/p-watcom.h \
	/freedos/src/include/portable.h command.h config.h context.h context.h_c \
	debug.h error.h misc.h
res_r.obj : res_r.c \
	 resource.h
copy.obj : copy.c \
	 /freedos/src/include/dfn.h /freedos/src/include/p-bc.h \
	/freedos/src/include/p-pac.h /freedos/src/include/p-watcom.h \
	/freedos/src/include/portable.h /freedos/src/include/supplio.h cmdline.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h openf.h \
	strings.h
internal.obj : internal.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h strings.h
shift.obj : shift.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
pause.obj : pause.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h
date.obj : date.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c datefunc.h debug.h error.h \
	misc.h strings.h
dir.obj : dir.c \
	 /freedos/src/include/dfn.h /freedos/src/include/dynstr.h \
	/freedos/src/include/nls_c.h /freedos/src/include/p-bc.h \
	/freedos/src/include/p-pac.h /freedos/src/include/p-watcom.h \
	/freedos/src/include/portable.h /freedos/src/include/sstr.h cmdline.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h
command.obj : command.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h nls.h openf.h session.h strings.h
truename.obj : truename.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
session.obj : session.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	session.h
ctty.obj : ctty.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h openf.h \
	strings.h
cmdinput.obj : cmdinput.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
ren.obj : ren.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h
exec.obj : exec.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
break.obj : break.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h
module.obj : module.c \
	 /freedos/src/include/algnbyte.h /freedos/src/include/algndflt.h \
	config.h debug.h misc.h module.h res.h resource.h
for.obj : for.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h strings.h
res.obj : res.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h context.h context.h_c debug.h error.h misc.h resource.h
alias.obj : alias.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h alias.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h tempfile.h
parsenum.obj : parsenum.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h
set.obj : set.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h
misc.obj : misc.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h
ver.obj : ver.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h strings.h
tmpnam.obj : tmpnam.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
path.obj : path.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h
if.obj : if.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h
mod_crit.obj : mod_crit.c \
	 /freedos/src/include/algnbyte.h /freedos/src/include/algndflt.h \
	config.h criter.ver debug.h dmy_crit.inc misc.h module.h resource.h
history.obj : history.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h
init.obj : init.c \
	 /freedos/src/include/algnbyte.h /freedos/src/include/algndflt.h \
	/freedos/src/include/dfn.h /freedos/src/include/environ.h \
	/freedos/src/include/fmemory.h /freedos/src/include/mcb.h \
	/freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h module.h resource.h strings.h timefunc.h
tempfile.obj : tempfile.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	tempfile.h
timefunc.obj : timefunc.c \
	 config.h debug.h timefunc.h
cmdtable.obj : cmdtable.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h
cmdline.obj : cmdline.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h
call.obj : call.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
beep.obj : beep.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
echo.obj : echo.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h strings.h
loadhigh.obj : loadhigh.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	loadhigh.h misc.h strings.h
mod_cbrk.obj : mod_cbrk.c \
	 /freedos/src/include/algnbyte.h /freedos/src/include/algndflt.h \
	cb_catch.ver config.h debug.h dmy_cbrk.inc module.h resource.h
environ.obj : environ.c \
	 /freedos/src/include/environ.h /freedos/src/include/p-bc.h \
	/freedos/src/include/p-pac.h /freedos/src/include/p-watcom.h \
	/freedos/src/include/portable.h command.h config.h context.h context.h_c \
	debug.h error.h misc.h
type.obj : type.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c debug.h error.h \
	misc.h openf.h strings.h
prompt.obj : prompt.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	cmdline.h command.h config.h context.h context.h_c datefunc.h debug.h \
	error.h misc.h
mod_rspn.obj : mod_rspn.c \
	 /freedos/src/include/algnbyte.h /freedos/src/include/algndflt.h \
	config.h debug.h module.h resource.h rsp_fail.ver
cls.obj : cls.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
batch.obj : batch.c \
	 /freedos/src/include/dfn.h /freedos/src/include/p-bc.h \
	/freedos/src/include/p-pac.h /freedos/src/include/p-watcom.h \
	/freedos/src/include/portable.h batch.h cmdline.h command.h config.h \
	context.h context.h_c debug.h error.h misc.h
debug.obj : debug.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
error.obj : error.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h
datefunc.obj : datefunc.c \
	 config.h datefunc.h debug.h
nls.obj : nls.c \
	 config.h debug.h nls.h
time.obj : time.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h timefunc.h
filecomp.obj : filecomp.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
fddebug.obj : fddebug.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h
lh.obj : lh.asm
verify.obj : verify.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h \
	strings.h
del.obj : del.c \
	 /freedos/src/include/dfn.h /freedos/src/include/p-bc.h \
	/freedos/src/include/p-pac.h /freedos/src/include/p-watcom.h \
	/freedos/src/include/portable.h cmdline.h command.h config.h context.h \
	context.h_c debug.h error.h misc.h strings.h
redir.obj : redir.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h context.h context.h_c debug.h error.h misc.h
DYNSOURCES =
#MAKEDEP STOP

clobber : tools_clobber strings_clobber clean
	$(RM) $(RMFLAGS) *.exe *.com

clean : tools_clean strings_clean
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj *.exe $(CFG) *.dmp com.com


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




# Generate TDUMP files from OBJ files
%.dmp :| %.obj
	tdump $< > $@

# DMake v4.1 ‘ccepts only up to one entry within SOURCE.lib
# Note: TDUMP does not accept libraries of any page size, always
# verify if the page size is a power of two, if the last line
# of the output of TDUMP says "Invalid data"
suppl_$(_MODEL).dmp : ..\LIB\BC5\suppl_$(_MODEL)$A
	tdump $< >$@

clib_$(_MODEL).dmp : d:\BC5\LIB\C$(_MODEL)$A
	tdump $< >$@

# Generate Xref
xref : $(OBJ:s/obj/dmp/) suppl_$(_MODEL).dmp clib_$(_MODEL).dmp com.map com.exe
	perl mkxref.pl >$@
