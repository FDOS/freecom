# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.10  2001/03/07 19:48:59  skaus
# Merged in Swap Support changes
#
# Revision 1.4.4.7  2001/02/27 18:27:04  skaus
# fix: criter.asm: DEBUG does no longer compile to .COM (for consistency)
# chg: removed all NULLs to prevent NULL problematic, functions must be
# 	used fully prototyped now
#
# Revision 1.4.4.6  2001/02/21 01:07:01  skaus
# bugfix: _fstrchr() doesnot find '\0'
# bugfix: NULL problematic
#
# Revision 1.4.4.5  2001/02/18 21:08:23  skaus
# add: command WHICH
# fix: BUILD.BAT and accompanying makefiles for TC++ v1.01
#
# Revision 1.4.4.4  2001/02/18 17:59:36  skaus
# bugfix: KSSF: restore parentPSP on exit
# chg: Using STRINGS resource for all non-interactive messages
# chg: moving all assembly files to NASM
#
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
# Revision 1.9  2001/02/14 23:50:05  skaus
# add: DIR /Y to display 4digit year
# fix: DIR displays "bytes free" at the very end, but if more than one
# 	argument is specified, it only displays the free bytes of the
# 	very last argument. The line is now printed after the display
# 	of each argument.
# bugfix: When the "bytes free" are displayed the used drive letter
# 	had been deallocated already.
# add: NLS-compatible input of date
# add: NLS-compatible display of date & time (incl DIR)
# fix: parse years 80..199 as century 1900 (by Arkady)
# chg: removed some static variables
# chg: made all global functions & variables static to LOADHIGH.C
# sub: LH.ASM/LOADHIGH.C: duplicate functions (memory API, farmemcmp)
# fix: doc of DATE, TIME, FreeCOM
#
# Revision 1.8  2000/12/10 04:16:07  skaus
# add: Installable Commands FEATURE_INSTALLABLE_COMMANDS
# add: F1, F3, F5, cur-right command line editing
# relased version 0.80
#
# Revision 1.7  2000/09/11 20:34:10  skaus
# bugfix: "line too long" error if last line of batch files has no newline
# bugfix: root path not recognized, e.g. in "C:\PATH> \program"
#
# Revision 1.6  2000/08/15 21:31:37  skaus
# chg: binary packages on FTP\nbugfix: piping
#
# Revision 1.5  2000/07/24 23:00:19  skaus
# fix: DIR & COPY, envSize, Makefiles
#
# Revision 1.4.2.2  2000/07/24 22:38:10  skaus
# add: Spanish Strings
# fix: Makefiles COMMAND.MAK for TC++ v1.01
# fix: environment size between 256..32767
# fix: COPY (to be tested)
# bugfix: DIR /S; output formatting to match MS COMMAND.COM
#
# Revision 1.4.2.1  2000/07/19 19:57:25  skaus
# Experimental DIR.C & COPY.C
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


.IMPORT : FREEDOS
.IMPORT .IGNORE : LNG DEBUG NDEBUG

INCDIR +=;$(FREEDOS)\SRC\INCLUDE
LIBDIR +=;$(FREEDOS)\SRC\LIB\$(_COMPILER)
LDLIBS = $(FREEDOS)\SRC\LIB\$(_COMPILER)\Suppl_$(_MODEL).lib
LDFLAGS += /msl
NASM *= c:\TOOL\NASMW.EXE

LD_TLINK != D:\BC5\BIN\TLINK.EXE

# Project specific C compiler flags
MYCFLAGS_DBG = -DNDEBUG=1 $(null,$(DEBUG) $(NULL) -DDEBUG=1)
MYCFLAGS_NDBG =
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))

# Project specific NASM compiler flags
NASMFLAGS_DBG = $(null,$(DEBUG) $(NULL) -DDEBUG=1) -s
NASMFLAGS_NDBG = -s
NASMFLAGS = $(null,$(NDEBUG) $(NASMFLAGS_DBG) $(NASMFLAGS_NDBG))

.SOURCE.lib := $(LIBDIR:s/;/ /:u)

# Sources of this make target
SRC = alias.c batch.c beep.c break.c call.c cb_catch.asm cls.c cmdinput.c \
	cmdline.c cmdtable.c command.c copy.c ctty.c \
	d:\freedos\src\1\freecom\makdep.opt \
	d:\freedos\src\cvs.mod\freecom\makdep.opt date.c datefunc.c debug.c \
	del.c dir.c dstack.c dummies.asm echo.c environ.c error.c exec.c \
	fddebug.c filecomp.c for.c goto.c history.c if.c init.c internal.c \
	kswap.c loadhigh.c lowexec.asm messages.c misc.c module.c mux_ae.c nls.c \
	openf.c parsenum.c path.c pause.c prompt.c redir.c ren.c res.c res_r.c \
	res_w.c set.c shift.c tempfile.c time.c timefunc.c tmpnam.c truename.c \
	type.c ver.c verify.c where.c
OBJ = alias.obj batch.obj beep.obj break.obj call.obj cb_catch.obj cls.obj \
	cmdinput.obj cmdline.obj cmdtable.obj command.obj copy.obj ctty.obj \
	date.obj datefunc.obj debug.obj del.obj dir.obj dstack.obj dummies.obj \
	echo.obj environ.obj error.obj exec.obj fddebug.obj filecomp.obj for.obj \
	goto.obj history.obj if.obj init.obj internal.obj kswap.obj loadhigh.obj \
	lowexec.obj messages.obj misc.obj module.obj mux_ae.obj nls.obj \
	openf.obj parsenum.obj path.obj pause.obj prompt.obj redir.obj ren.obj \
	res.obj res_r.obj res_w.obj set.obj shift.obj tempfile.obj time.obj \
	timefunc.obj tmpnam.obj truename.obj type.obj ver.obj verify.obj \
	where.obj
HDR = alias.h batch.h cmdline.h command.h config.h criter.mod\context.h_c \
	crossjmp.h datefunc.h debug.h kswap.h misc.h model.inc module.h mux_ae.h \
	nls.h openf.h res.h resource.h small.inc strings.h strings.typ stuff.inc \
	tempfile.h timefunc.h


.INCLUDE .IGNORE : lastmake.mk

THISMAKE !:= $(_COMPILER):$(_MODEL):$(LNG):$(null,$(NDEBUG) DBG NDBG)

#	Default target
all: com.com tools

%.obj : %.asm ; $(NASM) $(NASMFLAGS) -f obj -F borland -o $@ $< >>errlist

### Utils are required by build process
.INIT .PHONY .SEQUENTIAL : verscheck $(CFG) __errl utils

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
com.exe com.map .UPDATEALL : $(OBJ)
	$(MAK_EXE)

com.com : com.exe strings.dat criter
	@+copy /b com.exe + criter.mod\\criter + criter.mod\\criter1 + strings.dat $@

#	@+copy /b com.exe com_tmp.exe >nul
#	@+tdstrip com_tmp.exe
#	@+copy /b com_tmp.exe + criter + strings.err + strings.dat $@
#	@+del com_tmp.exe >nul

strings.err strings.h strings.dat .UPDATEALL .SETDIR=strings : default.lng
	@+echo ==Entering $(PWD)
	$(MAKE) LNG=$(LNG) NASM=$(NASM) DEBUG=$(DEBUG) NDEBUG=$(NDEBUG) all
	@+echo ==Leaving $(PWD)

strings_clean .SETDIR=strings :
	@+echo ==Entering $(PWD)
	$(MAKE) clean
	@+echo ==Leaving $(PWD)

strings_clobber .SETDIR=strings :
	@+echo ==Entering $(PWD)
	$(MAKE) clobber
	@+echo ==Leaving $(PWD)

default.mk: ../config.h
	..\utils\mktools.exe >$@

tools .SETDIR=tools : default.mk
	@+echo ==Entering $(PWD)
	$(MAKE) LNG=$(LNG) NASM=$(NASM) DEBUG=$(DEBUG) NDEBUG=$(NDEBUG) all
	@+echo ==Leaving $(PWD)

tools_clean .SETDIR=tools :
	@+echo ==Entering $(PWD)
	$(MAKE) -f makefile.1 clean
	@+echo ==Leaving $(PWD)

tools_clobber .SETDIR=tools :
	@+echo ==Entering $(PWD)
	$(MAKE) -f makefile.1 clobber
	@+echo ==Leaving $(PWD)

utils .SETDIR=utils :
	@+echo ==Entering $(PWD)
	$(MAKE) LNG=$(LNG) NASM=$(NASM) DEBUG=$(DEBUG) NDEBUG=$(NDEBUG) all
	@+echo ==Leaving $(PWD)

utils_clean .SETDIR=utils :
	@+echo ==Entering $(PWD)
	$(MAKE) clean
	@+echo ==Leaving $(PWD)

utils_clobber .SETDIR=utils :
	@+echo ==Entering $(PWD)
	$(MAKE) clobber
	@+echo ==Leaving $(PWD)

criter criter.mod/context.h_c .UPDATEALL .SETDIR=criter.mod : 
	@+echo ==Entering $(PWD)
	$(MAKE) LNG=$(LNG) NASM=$(NASM) DEBUG=$(DEBUG) NDEBUG=$(NDEBUG) all
	@+echo ==Leaving $(PWD)

criter_clean .SETDIR=criter.mod :
	@+echo ==Entering $(PWD)
	$(MAKE) clean
	@+echo ==Leaving $(PWD)

criter_clobber .SETDIR=criter.mod :
	@+echo ==Entering $(PWD)
	$(MAKE) clobber
	@+echo ==Leaving $(PWD)

#MAKEDEP START
lowexec.obj : lowexec.asm \
	 .\model.inc .\small.inc .\stuff.inc
date.obj : date.c \
	 cmdline.h command.h config.h datefunc.h debug.h misc.h nls.h \
	strings.h
dir.obj : dir.c \
	 cmdline.h command.h config.h debug.h misc.h nls.h strings.h
command.obj : command.c \
	 batch.h cmdline.h command.h config.h criter.mod/context.h_c \
	crossjmp.h debug.h kswap.h misc.h mux_ae.h nls.h openf.h strings.h
ctty.obj : ctty.c \
	 command.h config.h debug.h misc.h openf.h strings.h
ren.obj : ren.c \
	 cmdline.h command.h config.h debug.h misc.h
module.obj : module.c \
	 command.h criter.mod/context.h_c debug.h kswap.h misc.h module.h \
	res.h resource.h
cb_catch.obj : cb_catch.asm \
	 .\model.inc .\small.inc .\stuff.inc
res.obj : res.c \
	 command.h debug.h misc.h resource.h
alias.obj : alias.c \
	 alias.h cmdline.h command.h config.h debug.h misc.h tempfile.h
parsenum.obj : parsenum.c \
	 cmdline.h command.h config.h debug.h misc.h
ver.obj : ver.c \
	 cmdline.h command.h config.h debug.h misc.h strings.h
path.obj : path.c \
	 cmdline.h command.h config.h debug.h misc.h strings.h
if.obj : if.c \
	 batch.h cmdline.h command.h config.h debug.h misc.h
timefunc.obj : timefunc.c \
	 config.h debug.h timefunc.h
cmdtable.obj : cmdtable.c \
	 command.h config.h debug.h misc.h strings.h
call.obj : call.c \
	 batch.h cmdline.h command.h config.h criter.mod/context.h_c debug.h \
	kswap.h misc.h
mux_ae.obj : mux_ae.c \
	 command.h config.h crossjmp.h debug.h misc.h mux_ae.h
type.obj : type.c \
	 cmdline.h command.h config.h debug.h misc.h openf.h strings.h
prompt.obj : prompt.c \
	 cmdline.h command.h config.h datefunc.h debug.h misc.h
cls.obj : cls.c \
	 command.h config.h debug.h misc.h
debug.obj : debug.c \
	 command.h config.h debug.h misc.h
time.obj : time.c \
	 cmdline.h command.h config.h debug.h misc.h nls.h strings.h \
	timefunc.h
verify.obj : verify.c \
	 command.h config.h debug.h misc.h strings.h
redir.obj : redir.c \
	 command.h config.h debug.h misc.h
openf.obj : openf.c \
	 openf.h
messages.obj : messages.c \
	 command.h config.h debug.h misc.h res.h resource.h strings.h \
	strings.typ
goto.obj : goto.c \
	 batch.h cmdline.h command.h config.h debug.h misc.h strings.h
dstack.obj : dstack.c \
	 command.h config.h debug.h misc.h strings.h
where.obj : where.c \
	 cmdline.h command.h config.h debug.h misc.h
res_r.obj : res_r.c \
	 resource.h
copy.obj : copy.c \
	 cmdline.h command.h config.h debug.h misc.h openf.h strings.h
internal.obj : internal.c \
	 cmdline.h command.h config.h debug.h misc.h strings.h
shift.obj : shift.c \
	 batch.h command.h config.h debug.h misc.h
pause.obj : pause.c \
	 batch.h command.h config.h debug.h misc.h strings.h
res_w.obj : res_w.c \
	 resource.h
truename.obj : truename.c \
	 command.h config.h debug.h misc.h
cmdinput.obj : cmdinput.c \
	 batch.h command.h config.h debug.h misc.h
exec.obj : exec.c \
	 command.h config.h debug.h misc.h
break.obj : break.c \
	 command.h config.h debug.h misc.h strings.h
kswap.obj : kswap.c \
	 alias.h command.h config.h criter.mod/context.h_c debug.h kswap.h \
	misc.h
for.obj : for.c \
	 batch.h cmdline.h command.h config.h debug.h misc.h strings.h
set.obj : set.c \
	 cmdline.h command.h config.h debug.h misc.h
misc.obj : misc.c \
	 batch.h command.h config.h datefunc.h debug.h misc.h nls.h strings.h \
	timefunc.h
tmpnam.obj : tmpnam.c \
	 command.h config.h debug.h misc.h
history.obj : history.c \
	 command.h config.h debug.h misc.h strings.h
init.obj : init.c \
	 batch.h cmdline.h command.h config.h criter.mod/context.h_c debug.h \
	kswap.h misc.h module.h strings.h timefunc.h
tempfile.obj : tempfile.c \
	 command.h config.h debug.h misc.h tempfile.h
cmdline.obj : cmdline.c \
	 cmdline.h command.h config.h debug.h misc.h
beep.obj : beep.c \
	 batch.h command.h config.h debug.h misc.h
echo.obj : echo.c \
	 batch.h cmdline.h command.h config.h debug.h misc.h strings.h
loadhigh.obj : loadhigh.c \
	 cmdline.h command.h config.h debug.h misc.h strings.h
environ.obj : environ.c \
	 command.h config.h debug.h misc.h
batch.obj : batch.c \
	 batch.h cmdline.h command.h config.h debug.h misc.h
error.obj : error.c \
	 command.h config.h debug.h misc.h strings.h
datefunc.obj : datefunc.c \
	 config.h datefunc.h debug.h
nls.obj : nls.c \
	 cmdline.h command.h config.h debug.h misc.h nls.h strings.h
filecomp.obj : filecomp.c \
	 command.h config.h debug.h misc.h strings.h
fddebug.obj : fddebug.c \
	 cmdline.h command.h config.h debug.h misc.h strings.h
dummies.obj : dummies.asm \
	 .\model.inc .\small.inc
del.obj : del.c \
	 cmdline.h command.h config.h debug.h misc.h strings.h
DYNSOURCES =
#MAKEDEP STOP

clobber : utils_clobber tools_clobber strings_clobber criter_clobber my_clean
	$(RM) $(RMFLAGS) *.com *.cln

clean : utils_clean tools_clean strings_clean criter_clean my_clean

my_clean :
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

command.mak : makefile command.m1 command.m2
	+copy command.m1 + $(mktmp command.exe : \44(CFG) $(OBJ:t" \\\\\\n\\t") \n\t\44(TLINK) /x /c /d @&&|\n\44(LIBPATH)\\c0s.obj+\n$(OBJ:t"+\\n")\n) + command.m2 $@


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
