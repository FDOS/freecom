# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.1  2000/05/20 03:29:38  jimtabor
# Initial revision
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

# Sources of this make target
SRC = alias.c batch.c beep.c break.c call.c cb_catch.asm cls.c cmdinput.c \
	cmdline.c cmdtable.c command.c copy.c ctty.c date.c datefunc.c debug.c \
	del.c dir.c echo.c environ.c err_hand.c error.c exec.c \
	filecomp.c for.c goto.c history.c if.c init.c internal.c lh.asm \
	loadhigh.c lowexec.asm messages.c misc.c nls.c openf.c parsenum.c path.c \
	pause.c prompt.c session.c set.c shift.c \
	spawn.asm swapexec.c tempfile.c time.c timefunc.c tmpnam.c truename.c \
	type.c ver.c verify.c where.c
OBJ = alias.obj batch.obj beep.obj break.obj call.obj cb_catch.obj cls.obj \
	cmdinput.obj cmdline.obj cmdtable.obj command.obj copy.obj ctty.obj \
	date.obj datefunc.obj debug.obj del.obj dir.obj echo.obj environ.obj \
	err_hand.obj error.obj exec.obj filecomp.obj for.obj goto.obj \
	history.obj if.obj init.obj internal.obj lh.obj loadhigh.obj lowexec.obj \
	messages.obj misc.obj nls.obj openf.obj parsenum.obj path.obj pause.obj \
	prompt.obj redir.obj ren.obj session.obj set.obj shift.obj \
	spawn.obj swapexec.obj tempfile.obj time.obj timefunc.obj tmpnam.obj \
	truename.obj type.obj ver.obj verify.obj where.obj
HDR = alias.h batch.h cmdline.h command.h compat.h config.h datefunc.h \
	debug.h err_hand.h loadhigh.h model.def nls.h openf.h session.h \
	strings.h swapexec.h tempfile.h timefunc.h




#	Default target
all: com.com ptchldrv.exe


.INIT : $(CFG) __errl

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c

com.exe ?= _OBJS := $(OBJ:s/c0.obj//)
# STD_OBJ !:= C0.OBJ
com.map com.exe .UPDATEALL : $(OBJ)
	$(MAK_EXE)

com.com : com.exe strings.dat
	@+copy /b com.exe + strings.dat $@

ptchldrv.obj : ptchldrv.c config.h

strings.h : strings/strings.txt
	+fixstrs

#MAKEDEP START
if.obj : if.c \
	 batch.h cmdline.h command.h config.h debug.h
openf.obj : openf.c \
	 openf.h
loadhigh.obj : loadhigh.c \
	 cmdline.h command.h config.h debug.h loadhigh.h strings.h
cmdline.obj : cmdline.c \
	 cmdline.h command.h config.h debug.h
ver.obj : ver.c \
	 cmdline.h command.h config.h debug.h strings.h
internal.obj : internal.c \
	 cmdline.h command.h config.h debug.h strings.h
time.obj : time.c \
	 command.h config.h debug.h strings.h timefunc.h
exec.obj : exec.c \
	 command.h config.h debug.h
copy.obj : copy.c \
	 cmdline.h command.h config.h debug.h openf.h strings.h
cmdtable.obj : cmdtable.c \
	 command.h config.h debug.h strings.h
cls.obj : cls.c \
	 command.h config.h debug.h
date.obj : date.c \
	 command.h config.h datefunc.h debug.h strings.h
set.obj : set.c \
	 cmdline.h command.h config.h debug.h
cb_catch.obj : cb_catch.asm \
	 .\model.def
type.obj : type.c \
	 cmdline.h command.h config.h debug.h openf.h strings.h
redir.obj : redir.c \
	 command.h config.h debug.h
dir.obj : dir.c \
	 cmdline.h command.h config.h debug.h strings.h
messages.obj : messages.c \
	 command.h config.h debug.h strings.h
ctty.obj : ctty.c \
	 command.h config.h debug.h openf.h strings.h
path.obj : path.c \
	 cmdline.h command.h config.h debug.h
environ.obj : environ.c \
	 command.h config.h debug.h
echo.obj : echo.c \
	 batch.h cmdline.h command.h config.h debug.h strings.h
spawn.obj : spawn.asm
parsenum.obj : parsenum.c \
	 cmdline.h command.h config.h debug.h
shift.obj : shift.c \
	 batch.h command.h config.h debug.h
lh.obj : lh.asm
tempfile.obj : tempfile.c \
	 command.h config.h debug.h tempfile.h
goto.obj : goto.c \
	 batch.h cmdline.h command.h config.h debug.h strings.h
call.obj : call.c \
	 batch.h command.h config.h debug.h
beep.obj : beep.c \
	 batch.h command.h config.h debug.h
timefunc.obj : timefunc.c \
	 timefunc.h
filecomp.obj : filecomp.c \
	 command.h config.h debug.h
err_hand.obj : err_hand.c \
	 command.h config.h debug.h err_hand.h
swapexec.obj : swapexec.c \
	 command.h compat.h config.h debug.h swapexec.h
init.obj : init.c \
	 batch.h cmdline.h command.h config.h debug.h err_hand.h strings.h \
	timefunc.h
debug.obj : debug.c \
	 config.h debug.h
break.obj : break.c \
	 command.h config.h debug.h strings.h
verify.obj : verify.c \
	 command.h config.h debug.h strings.h
error.obj : error.c \
	 command.h config.h debug.h strings.h
where.obj : where.c \
	 command.h config.h debug.h
command.obj : command.c \
	 batch.h cmdline.h command.h config.h debug.h nls.h openf.h session.h \
	strings.h swapexec.h
pause.obj : pause.c \
	 batch.h command.h config.h debug.h strings.h
del.obj : del.c \
	 cmdline.h command.h config.h debug.h strings.h
session.obj : session.c \
	 command.h config.h debug.h session.h
alias.obj : alias.c \
	 alias.h cmdline.h command.h config.h debug.h tempfile.h
truename.obj : truename.c \
	 command.h config.h debug.h
tmpnam.obj : tmpnam.c \
	 command.h config.h debug.h
batch.obj : batch.c \
	 batch.h cmdline.h command.h config.h debug.h
lowexec.obj : lowexec.asm \
	 .\model.def
for.obj : for.c \
	 batch.h cmdline.h command.h config.h debug.h strings.h
misc.obj : misc.c \
	 batch.h command.h config.h debug.h
history.obj : history.c \
	 command.h config.h debug.h strings.h
datefunc.obj : datefunc.c \
	 datefunc.h
nls.obj : nls.c \
	 config.h debug.h nls.h
ren.obj : ren.c \
	 cmdline.h command.h config.h debug.h
prompt.obj : prompt.c \
	 cmdline.h command.h config.h debug.h
cmdinput.obj : cmdinput.c \
	 batch.h command.h config.h debug.h
DYNSOURCES =
#MAKEDEP STOP

clean :
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

$(CFG) : makefile
	@+-echo Creating compiler configuration file: $(CFG)
	@+type $(mktmp 	$(CONFIGURATION)) >$@

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
