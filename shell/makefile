# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.2.2.3  2001/06/25 20:06:36  skaus
# Update #3
#
# Revision 1.2.2.2  2001/06/21 21:40:35  skaus
# Update #2
#
# Revision 1.2.2.1  2001/06/19 20:42:23  skaus
# Update #1
#
# Revision 1.2  2001/06/10 17:17:24  skaus
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
SRC = batch.c cb_catch.asm cmdtable.c command.c do_batch.c do_cmd.c \
	do_for.c dummies.asm ec_fcts.c expalias.c fcttable.c init.c kswap.c \
	loadhigh.c module.c redir.c ver.c
OBJ = batch.obj cb_catch.obj cmdtable.obj command.obj do_batch.obj \
	do_cmd.obj do_for.obj dummies.obj ec_fcts.obj expalias.obj fcttable.obj \
	init.obj kswap.obj loadhigh.obj module.obj redir.obj ver.obj
HDR = ..\config.h ..\include/cmdline.h ..\include/command.h \
	..\include/context.h ..\include/crossjmp.h ..\include/datefunc.h \
	..\include/debug.h ..\include/keys.h ..\include/kswap.h \
	..\include/message.h ..\include/misc.h ..\include/module.h \
	..\include/mux_ae.h ..\include/nls.h ..\include/openf.h ..\include/res.h \
	..\include/resource.h ..\include/timefunc.h ..\include\large.inc \
	..\include\model.inc ..\include\small.inc ..\include\stuff.inc



#	Default target
all: com.exe

dist : command.mak

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
	-ctags *.c ..\lib\*.c ..\cmd\*.c \freedos\src\lib\suppl\*.c

com.exe ?= _OBJS := $(OBJ:s/c0.obj//)
# STD_OBJ !:= C0.OBJ
com.exe com.map .UPDATEALL : $(OBJ) ../cmd/cmds.lib ../lib/freecom.lib
	$(MAK_EXE)

#MAKEDEP START
expalias.obj : expalias.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
command.obj : command.c \
	 ../config.h ../context.h_c ../err_fcts.h ../include/cmdline.h \
	../include/command.h ../include/context.h ../include/crossjmp.h \
	../include/datefunc.h ../include/debug.h ../include/kswap.h \
	../include/message.h ../include/misc.h ../include/mux_ae.h \
	../include/nls.h ../include/openf.h ../include/timefunc.h ../strings.h
fcttable.obj : fcttable.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
module.obj : module.c \
	 ../context.h_c ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/kswap.h ../include/message.h \
	../include/misc.h ../include/module.h ../include/res.h \
	../include/resource.h ../include/timefunc.h
kswap.obj : kswap.c \
	 ../config.h ../context.h_c ../err_fcts.h ../include/command.h \
	../include/context.h ../include/datefunc.h ../include/debug.h \
	../include/kswap.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
do_cmd.obj : do_cmd.c \
	 ../config.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h
cb_catch.obj : cb_catch.asm \
	 .\..\include\large.inc .\..\include\model.inc .\..\include\small.inc \
	.\..\include\stuff.inc
ver.obj : ver.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h ../strings.h
init.obj : init.c \
	 ../config.h ../context.h_c ../err_fcts.h ../include/cmdline.h \
	../include/command.h ../include/context.h ../include/datefunc.h \
	../include/debug.h ../include/keys.h ../include/kswap.h \
	../include/message.h ../include/misc.h ../include/module.h \
	../include/timefunc.h ../strings.h
ec_fcts.obj : ec_fcts.c \
	 ../config.h ../include/context.h ../include/debug.h
do_for.obj : do_for.c \
	 ../config.h ../err_fcts.h ../include/command.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
cmdtable.obj : cmdtable.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/message.h ../include/misc.h \
	../include/timefunc.h ../strings.h
loadhigh.obj : loadhigh.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h ../strings.h
batch.obj : batch.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/context.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
dummies.obj : dummies.asm \
	 .\..\include\large.inc .\..\include\model.inc .\..\include\small.inc
do_batch.obj : do_batch.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/context.h ../include/datefunc.h ../include/debug.h \
	../include/message.h ../include/misc.h ../include/timefunc.h
redir.obj : redir.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/message.h \
	../include/misc.h ../include/timefunc.h
DYNSOURCES =
#MAKEDEP STOP

clobber : clean
	$(RM) $(RMFLAGS) *.com *.cln command.mak com.exe *.cfg

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

command.mak : makefile command.m1 command.m2
	+copy command.m1 + $(mktmp command.exe : \44(CFG) $(OBJ:t" \\\\\\n\\t") \n\t\44(LD) /m/s/l /c /d @&&|\n\44(LIBPATH)\\c0s.obj+\n$(OBJ:t"+\\n")\n) + command.m2 $@
