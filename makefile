# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
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

# Project specific C compiler flags
MYCFLAGS_DBG = -DDEBUG=1
MYCFLAGS_NDBG =
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))

.SOURCE.lib := $(LIBDIR:s/;/ /:u)
.IMPORT : FREEDOS
.IMPORT .IGNORE : LNG

# Sources of this make target
SRC = alias.c batch.c beep.c break.c call.c cb_catch.asm cls.c cmdinput.c \
	cmdline.c cmdtable.c command.c copy.c ctty.c date.c datefunc.c debug.c \
	del.c dir.c dstack.c echo.c environ.c err_hand.c error.c exec.c \
	fddebug.c filecomp.c for.c goto.c history.c if.c init.c internal.c \
	lh.asm loadhigh.c lowexec.asm messages.c misc.c nls.c openf.c parsenum.c \
	path.c pause.c prompt.c redir.c ren.c session.c set.c shift.c spawn.asm \
	swapexec.c tempfile.c time.c timefunc.c tmpnam.c truename.c type.c ver.c \
	verify.c where.c
OBJ = alias.obj batch.obj beep.obj break.obj call.obj cb_catch.obj cls.obj \
	cmdinput.obj cmdline.obj cmdtable.obj command.obj copy.obj ctty.obj \
	date.obj datefunc.obj debug.obj del.obj dir.obj dstack.obj echo.obj \
	environ.obj err_hand.obj error.obj exec.obj fddebug.obj filecomp.obj \
	for.obj goto.obj history.obj if.obj init.obj internal.obj lh.obj \
	loadhigh.obj lowexec.obj messages.obj misc.obj nls.obj openf.obj \
	parsenum.obj path.obj pause.obj prompt.obj redir.obj ren.obj session.obj \
	set.obj shift.obj spawn.obj swapexec.obj tempfile.obj time.obj \
	timefunc.obj tmpnam.obj truename.obj type.obj ver.obj verify.obj \
	where.obj
HDR = alias.h batch.h cmdline.h command.h compat.h config.h datefunc.h \
	debug.h err_hand.h loadhigh.h model.def nls.h openf.h session.h \
	strings.h strings.typ swapexec.h tempfile.h timefunc.h




#	Default target
all: com.com tools


.INIT : $(CFG) __errl

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c

com.exe ?= _OBJS := $(OBJ:s/c0.obj//)
# STD_OBJ !:= C0.OBJ
com.map com.exe .UPDATEALL : $(OBJ)
	$(MAK_EXE)

com.com : com.exe strings.dat
	@+copy /b com.exe com_tmp.exe >nul
	@+tdstrip com_tmp.exe
	@+copy /b com_tmp.exe + strings.dat $@
	@+del com_tmp.exe >nul

strings.h strings.dat .UPDATEALL .SETDIR=strings : default.lng
	@+echo Entering $(PWD)
	$(MAKE) LNG=$(LNG) all
	@+echo Leaving $(PWD)

strings_clean .SETDIR=strings :
	@+echo Entering $(PWD)
	$(MAKE) clean
	@+echo Leaving $(PWD)

strings_clobber .SETDIR=strings :
	@+echo Entering $(PWD)
	$(MAKE) clobber
	@+echo Leaving $(PWD)

tools .SETDIR=tools :
	@+echo Entering $(PWD)
	$(MAKE) all
	@+echo Leaving $(PWD)

tools_clean .SETDIR=tools :
	@+echo Entering $(PWD)
	$(MAKE) clean
	@+echo Leaving $(PWD)

tools_clobber .SETDIR=tools :
	@+echo Entering $(PWD)
	$(MAKE) clobber
	@+echo Leaving $(PWD)

#MAKEDEP START
lowexec.obj : lowexec.asm \
	 .\model.def
openf.obj : openf.c \
	 openf.h
messages.obj : messages.c \
	 command.h config.h debug.h strings.h strings.typ
goto.obj : goto.c \
	 batch.h cmdline.h command.h config.h debug.h strings.h
dstack.obj : dstack.c \
	 command.h config.h debug.h strings.h
where.obj : where.c \
	 command.h config.h debug.h
copy.obj : copy.c \
	 cmdline.h command.h config.h debug.h openf.h strings.h
internal.obj : internal.c \
	 cmdline.h command.h config.h debug.h strings.h
shift.obj : shift.c \
	 batch.h command.h config.h debug.h
pause.obj : pause.c \
	 batch.h command.h config.h debug.h strings.h
date.obj : date.c \
	 command.h config.h datefunc.h debug.h strings.h
dir.obj : dir.c \
	 cmdline.h command.h config.h debug.h strings.h
command.obj : command.c \
	 batch.h cmdline.h command.h config.h debug.h nls.h openf.h session.h \
	strings.h swapexec.h
swapexec.obj : swapexec.c \
	 command.h compat.h config.h debug.h swapexec.h
truename.obj : truename.c \
	 command.h config.h debug.h
err_hand.obj : err_hand.c \
	 command.h config.h debug.h err_hand.h
session.obj : session.c \
	 command.h config.h debug.h session.h
ctty.obj : ctty.c \
	 command.h config.h debug.h openf.h strings.h
cmdinput.obj : cmdinput.c \
	 batch.h command.h config.h debug.h
ren.obj : ren.c \
	 cmdline.h command.h config.h debug.h
exec.obj : exec.c \
	 command.h config.h debug.h
break.obj : break.c \
	 command.h config.h debug.h strings.h
for.obj : for.c \
	 batch.h cmdline.h command.h config.h debug.h strings.h
cb_catch.obj : cb_catch.asm \
	 .\model.def
alias.obj : alias.c \
	 alias.h cmdline.h command.h config.h debug.h tempfile.h
parsenum.obj : parsenum.c \
	 cmdline.h command.h config.h debug.h
set.obj : set.c \
	 cmdline.h command.h config.h debug.h
misc.obj : misc.c \
	 batch.h command.h config.h debug.h strings.h
ver.obj : ver.c \
	 cmdline.h command.h config.h debug.h strings.h
tmpnam.obj : tmpnam.c \
	 command.h config.h debug.h
path.obj : path.c \
	 cmdline.h command.h config.h debug.h
if.obj : if.c \
	 batch.h cmdline.h command.h config.h debug.h
history.obj : history.c \
	 command.h config.h debug.h strings.h
init.obj : init.c \
	 batch.h cmdline.h command.h config.h debug.h err_hand.h strings.h \
	timefunc.h
tempfile.obj : tempfile.c \
	 command.h config.h debug.h tempfile.h
timefunc.obj : timefunc.c \
	 config.h debug.h timefunc.h
cmdtable.obj : cmdtable.c \
	 command.h config.h debug.h strings.h
cmdline.obj : cmdline.c \
	 cmdline.h command.h config.h debug.h
call.obj : call.c \
	 batch.h command.h config.h debug.h
beep.obj : beep.c \
	 batch.h command.h config.h debug.h
echo.obj : echo.c \
	 batch.h cmdline.h command.h config.h debug.h strings.h
loadhigh.obj : loadhigh.c \
	 cmdline.h command.h config.h debug.h loadhigh.h strings.h
environ.obj : environ.c \
	 command.h config.h debug.h
type.obj : type.c \
	 cmdline.h command.h config.h debug.h openf.h strings.h
prompt.obj : prompt.c \
	 cmdline.h command.h config.h datefunc.h debug.h
cls.obj : cls.c \
	 command.h config.h debug.h
batch.obj : batch.c \
	 batch.h cmdline.h command.h config.h debug.h
debug.obj : debug.c \
	 command.h config.h debug.h
spawn.obj : spawn.asm
error.obj : error.c \
	 command.h config.h debug.h strings.h
datefunc.obj : datefunc.c \
	 config.h datefunc.h debug.h
nls.obj : nls.c \
	 config.h debug.h nls.h
time.obj : time.c \
	 command.h config.h debug.h strings.h timefunc.h
filecomp.obj : filecomp.c \
	 command.h config.h debug.h
fddebug.obj : fddebug.c \
	 command.h config.h debug.h strings.h
lh.obj : lh.asm
verify.obj : verify.c \
	 command.h config.h debug.h strings.h
del.obj : del.c \
	 cmdline.h command.h config.h debug.h strings.h
redir.obj : redir.c \
	 command.h config.h debug.h
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
