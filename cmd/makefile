# $Id$
#
# Makefile for the FreeCOM library
#
# $Log$
# Revision 1.3  2001/12/03 20:15:54  skaus
# v0.83 Beta 32:
# bugfix: if FREECOM.COM /P without AUTOEXEC.BAT --> assert() failure.
# add: if DEBUG enabled: internal command EXIT!! that ignores /P
# chg: FEATURE_BOOT_KEYS: mandory boot keys (F5/F8) turned into
# 	optional compile-time option; disabled by default now as the
# 	kernel provides F5/F8 checking
# chg: some document updates
#
# Revision 1.2  2001/06/10 17:17:23  skaus
# bugfix: Single point of configuration CONFIG.MAK/.H
#
# Revision 1.1  2001/04/12 00:09:06  skaus
# chg: New structure
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

.INCLUDE : ../_config.mk

# Sources of this make target
SRC = alias.c beep.c break.c call.c cdd.c chdir.c cls.c copy.c ctty.c \
	date.c del.c dir.c dirs.c doskey.c echo.c exit.c exit2.c fddebug.c for.c \
	goto.c history.c if.c memory.c mkdir.c path.c pause.c popd.c prompt.c \
	pushd.c rem.c ren.c rmdir.c set.c shift.c time.c truename.c type.c \
	verify.c which.c
OBJ = alias.obj beep.obj break.obj call.obj cdd.obj chdir.obj cls.obj \
	copy.obj ctty.obj date.obj del.obj dir.obj dirs.obj doskey.obj echo.obj \
	exit.obj exit2.obj fddebug.obj for.obj goto.obj history.obj if.obj \
	memory.obj mkdir.obj path.obj pause.obj popd.obj prompt.obj pushd.obj \
	rem.obj ren.obj rmdir.obj set.obj shift.obj time.obj truename.obj \
	type.obj verify.obj which.obj
HDR = ..\config.h ..\err_fcts.h ..\include/batch.h ..\include/cmdline.h \
	..\include/command.h ..\include/context.h ..\include/datefunc.h \
	..\include/debug.h ..\include/kswap.h ..\include/misc.h ..\include/nls.h \
	..\include/openf.h ..\include/timefunc.h


#	Default target
all: cmds.lib

dist: cmd.mak

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
	-ctags *.c ..\lib\*.c ..\shell\*.c $(FREEDOS)\SRC\LIB\SUPPL\*.c

cmds.lib .LIBRARY : $(OBJ)

#MAKEDEP START
mkdir.obj : mkdir.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
memory.obj : memory.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
goto.obj : goto.c \
	 ../config.h ../err_fcts.h ../include/batch.h ../include/cmdline.h \
	../include/command.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h ../strings.h
copy.obj : copy.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/openf.h ../include/timefunc.h ../strings.h
shift.obj : shift.c \
	 ../config.h ../include/batch.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
rmdir.obj : rmdir.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
pause.obj : pause.c \
	 ../config.h ../include/batch.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
date.obj : date.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/nls.h ../include/timefunc.h ../strings.h
dir.obj : dir.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/nls.h ../include/timefunc.h ../strings.h
truename.obj : truename.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
rem.obj : rem.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
ctty.obj : ctty.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/openf.h \
	../include/timefunc.h ../strings.h
ren.obj : ren.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
break.obj : break.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
for.obj : for.c \
	 ../config.h ../err_fcts.h ../include/batch.h ../include/cmdline.h \
	../include/command.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h ../strings.h
chdir.obj : chdir.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
alias.obj : alias.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/context.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h ../strings.h
cdd.obj : cdd.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
set.obj : set.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/context.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h ../strings.h
exit2.obj : exit2.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
path.obj : path.c \
	 ../config.h ../include/cmdline.h ../include/command.h \
	../include/context.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h ../strings.h
if.obj : if.c \
	 ../config.h ../err_fcts.h ../include/batch.h ../include/cmdline.h \
	../include/command.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
history.obj : history.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
dirs.obj : dirs.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
which.obj : which.c \
	 ../config.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
popd.obj : popd.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
exit.obj : exit.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
call.obj : call.c \
	 ../config.h ../context.h_c ../err_fcts.h ../include/batch.h \
	../include/cmdline.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/kswap.h ../include/misc.h \
	../include/timefunc.h
beep.obj : beep.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
echo.obj : echo.c \
	 ../config.h ../include/batch.h ../include/cmdline.h \
	../include/command.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h ../strings.h
type.obj : type.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/openf.h ../include/timefunc.h ../strings.h
prompt.obj : prompt.c \
	 ../config.h ../include/cmdline.h ../include/command.h \
	../include/context.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h
cls.obj : cls.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h
pushd.obj : pushd.c \
	 ../config.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
time.obj : time.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/nls.h ../include/timefunc.h ../strings.h
doskey.obj : doskey.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
fddebug.obj : fddebug.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
verify.obj : verify.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
del.obj : del.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
DYNSOURCES =
#MAKEDEP STOP

clobber : clean
	$(RM) $(RMFLAGS) *.com *.cln *.lib *.lst *.cfg cmd.mak

clean : 
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj *.exe $(CFG) *.dmp com.com tags errlist


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

cmd.mak : makefile cmd.m1 cmd.m2
	+copy cmd.m1 + $(mktmp cmds.lib : \44(CFG) $(OBJ:t" \\\\\\n\\t") \n\tif exist cmds.lib \44(AR) cmds.lib /c @&&|\n+-$(OBJ:t" &\\n+-")\n| , cmds.lst \n\tif not exist cmds.lib \44(AR) cmds.lib /c @&&|\n+$(OBJ:t" &\\n+")\n| , cmds.lst \n) + cmd.m2 $@

