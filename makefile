# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.4.4.3  2001/02/06 16:30:14  skaus
# chg: FreeCOM no longer uses the shell hack if kswap is active
# bugfix: aliases were read back incorrectly
#
# Revision 1.4.4.2  2001/01/28 21:23:55  skaus
# add: Kernel Swap Support
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
SRC = alias.c batch.c beep.c break.c call.c cb_catch.asm cls.c cmdinput.c \
	cmdline.c cmdtable.c command.c copy.c ctty.c \
	d:\freedos\src\cvs.res\freecom\makdep.opt date.c datefunc.c debug.c \
	del.c dir.c dstack.c echo.c environ.c error.c exec.c fddebug.c \
	filecomp.c for.c goto.c history.c if.c init.c internal.c kswap.c lh.asm \
	loadhigh.c lowexec.asm messages.c misc.c module.c nls.c openf.c \
	parsenum.c path.c pause.c prompt.c redir.c ren.c res.c res_r.c session.c \
	set.c shift.c tempfile.c time.c timefunc.c tmpnam.c truename.c type.c \
	ver.c verify.c where.c
OBJ = alias.obj batch.obj beep.obj break.obj call.obj cb_catch.obj cls.obj \
	cmdinput.obj cmdline.obj cmdtable.obj command.obj copy.obj ctty.obj \
	date.obj datefunc.obj debug.obj del.obj dir.obj dstack.obj echo.obj \
	environ.obj error.obj exec.obj fddebug.obj filecomp.obj for.obj goto.obj \
	history.obj if.obj init.obj internal.obj kswap.obj lh.obj loadhigh.obj \
	lowexec.obj messages.obj misc.obj module.obj nls.obj openf.obj \
	parsenum.obj path.obj pause.obj prompt.obj redir.obj ren.obj res.obj \
	res_r.obj session.obj set.obj shift.obj tempfile.obj time.obj \
	timefunc.obj tmpnam.obj truename.obj type.obj ver.obj verify.obj \
	where.obj
HDR = alias.h batch.h cmdline.h command.h config.h criter.mod\context.h_c \
	datefunc.h debug.h kswap.h loadhigh.h misc.h model.def module.h nls.h \
	openf.h res.h resource.h session.h strings.h strings.typ tempfile.h \
	timefunc.h




#	Default target
all: com.com tools


.INIT .PHONY .SEQUENTIAL : verscheck $(CFG) __errl

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

com.com : com.exe strings.dat criter criter1
	@+copy /b com.exe + criter.mod\\criter + criter.mod\\criter1 + strings.dat $@

#	@+copy /b com.exe com_tmp.exe >nul
#	@+tdstrip com_tmp.exe
#	@+copy /b com_tmp.exe + criter + strings.err + strings.dat $@
#	@+del com_tmp.exe >nul

strings.err strings.h strings.dat .UPDATEALL .SETDIR=strings : default.lng
	@+echo ==Entering $(PWD)
	$(MAKE) LNG=$(LNG) all
	@+echo ==Leaving $(PWD)

strings_clean .SETDIR=strings :
	@+echo ==Entering $(PWD)
	$(MAKE) clean
	@+echo ==Leaving $(PWD)

strings_clobber .SETDIR=strings :
	@+echo ==Entering $(PWD)
	$(MAKE) clobber
	@+echo ==Leaving $(PWD)

tools .SETDIR=tools :
	@+echo ==Entering $(PWD)
	$(MAKE) LNG=$(LNG) NASM=$(NASM) all
	@+echo ==Leaving $(PWD)

tools_clean .SETDIR=tools :
	@+echo ==Entering $(PWD)
	$(MAKE) clean
	@+echo ==Leaving $(PWD)

tools_clobber .SETDIR=tools :
	@+echo ==Entering $(PWD)
	$(MAKE) clobber
	@+echo ==Leaving $(PWD)

criter .SETDIR=criter.mod : criter.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $<

criter1 .SETDIR=criter.mod : criter.asm
	$(NASM) $(NASMFLAGS) -DAUTO_FAIL -f bin -o $@ $<

#MAKEDEP START
lowexec.obj : lowexec.asm \
	 .\model.def
openf.obj : openf.c \
	 openf.h
messages.obj : messages.c \
	 /freedos/src/include/suppl.h command.h config.h debug.h misc.h res.h \
	resource.h strings.h strings.typ
goto.obj : goto.c \
	 batch.h cmdline.h command.h config.h debug.h misc.h strings.h
dstack.obj : dstack.c \
	 /freedos/src/include/dfn.h command.h config.h debug.h misc.h \
	strings.h
where.obj : where.c \
	 /freedos/src/include/dfn.h command.h config.h debug.h misc.h
res_r.obj : res_r.c \
	 resource.h
copy.obj : copy.c \
	 /freedos/src/include/dfn.h /freedos/src/include/supplio.h cmdline.h \
	command.h config.h debug.h misc.h openf.h strings.h
internal.obj : internal.c \
	 cmdline.h command.h config.h debug.h misc.h strings.h
shift.obj : shift.c \
	 batch.h command.h config.h debug.h misc.h
pause.obj : pause.c \
	 batch.h command.h config.h debug.h misc.h strings.h
date.obj : date.c \
	 command.h config.h datefunc.h debug.h misc.h strings.h
dir.obj : dir.c \
	 /freedos/src/include/dfn.h /freedos/src/include/dynstr.h \
	/freedos/src/include/nls_c.h /freedos/src/include/sstr.h cmdline.h \
	command.h config.h debug.h misc.h strings.h
command.obj : command.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	cmdline.h command.h config.h criter.mod/context.h_c debug.h kswap.h \
	misc.h nls.h openf.h session.h strings.h
truename.obj : truename.c \
	 command.h config.h debug.h misc.h
session.obj : session.c \
	 command.h config.h debug.h misc.h session.h
ctty.obj : ctty.c \
	 command.h config.h debug.h misc.h openf.h strings.h
cmdinput.obj : cmdinput.c \
	 batch.h command.h config.h debug.h misc.h
ren.obj : ren.c \
	 cmdline.h command.h config.h debug.h misc.h
exec.obj : exec.c \
	 command.h config.h debug.h misc.h
break.obj : break.c \
	 command.h config.h debug.h misc.h strings.h
module.obj : module.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h criter.mod/context.h_c debug.h kswap.h misc.h module.h res.h \
	resource.h
kswap.obj : kswap.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h \
	command.h config.h criter.mod/context.h_c debug.h kswap.h misc.h
for.obj : for.c \
	 batch.h cmdline.h command.h config.h debug.h misc.h strings.h
cb_catch.obj : cb_catch.asm \
	 .\model.def
res.obj : res.c \
	 command.h debug.h misc.h resource.h
alias.obj : alias.c \
	 alias.h cmdline.h command.h config.h debug.h misc.h tempfile.h
parsenum.obj : parsenum.c \
	 cmdline.h command.h config.h debug.h misc.h
set.obj : set.c \
	 cmdline.h command.h config.h debug.h misc.h
misc.obj : misc.c \
	 batch.h command.h config.h debug.h misc.h strings.h
ver.obj : ver.c \
	 cmdline.h command.h config.h debug.h misc.h strings.h
tmpnam.obj : tmpnam.c \
	 command.h config.h debug.h misc.h
path.obj : path.c \
	 cmdline.h command.h config.h debug.h misc.h
if.obj : if.c \
	 batch.h cmdline.h command.h config.h debug.h misc.h
history.obj : history.c \
	 command.h config.h debug.h misc.h strings.h
init.obj : init.c \
	 /freedos/src/include/dfn.h /freedos/src/include/environ.h \
	/freedos/src/include/fmemory.h /freedos/src/include/mcb.h \
	/freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	cmdline.h command.h config.h criter.mod/context.h_c debug.h kswap.h \
	misc.h module.h strings.h timefunc.h
tempfile.obj : tempfile.c \
	 command.h config.h debug.h misc.h tempfile.h
timefunc.obj : timefunc.c \
	 config.h debug.h timefunc.h
cmdtable.obj : cmdtable.c \
	 command.h config.h debug.h misc.h strings.h
cmdline.obj : cmdline.c \
	 cmdline.h command.h config.h debug.h misc.h
call.obj : call.c \
	 /freedos/src/include/p-bc.h /freedos/src/include/p-pac.h \
	/freedos/src/include/p-watcom.h /freedos/src/include/portable.h batch.h \
	cmdline.h command.h config.h criter.mod/context.h_c debug.h kswap.h \
	misc.h
beep.obj : beep.c \
	 batch.h command.h config.h debug.h misc.h
echo.obj : echo.c \
	 batch.h cmdline.h command.h config.h debug.h misc.h strings.h
loadhigh.obj : loadhigh.c \
	 cmdline.h command.h config.h debug.h loadhigh.h misc.h strings.h
environ.obj : environ.c \
	 /freedos/src/include/environ.h command.h config.h debug.h misc.h
type.obj : type.c \
	 cmdline.h command.h config.h debug.h misc.h openf.h strings.h
prompt.obj : prompt.c \
	 cmdline.h command.h config.h datefunc.h debug.h misc.h
cls.obj : cls.c \
	 command.h config.h debug.h misc.h
batch.obj : batch.c \
	 /freedos/src/include/dfn.h batch.h cmdline.h command.h config.h \
	debug.h misc.h
debug.obj : debug.c \
	 command.h config.h debug.h misc.h
error.obj : error.c \
	 command.h config.h debug.h misc.h strings.h
datefunc.obj : datefunc.c \
	 config.h datefunc.h debug.h
nls.obj : nls.c \
	 config.h debug.h nls.h
time.obj : time.c \
	 command.h config.h debug.h misc.h strings.h timefunc.h
filecomp.obj : filecomp.c \
	 command.h config.h debug.h misc.h
fddebug.obj : fddebug.c \
	 command.h config.h debug.h misc.h strings.h
lh.obj : lh.asm
verify.obj : verify.c \
	 command.h config.h debug.h misc.h strings.h
del.obj : del.c \
	 /freedos/src/include/dfn.h cmdline.h command.h config.h debug.h \
	misc.h strings.h
redir.obj : redir.c \
	 command.h config.h debug.h misc.h
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
