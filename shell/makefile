# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.9  2002/05/11 22:10:40  skaus
# v0.83 Beta 40:
# bugfix: ^Break detection after XMS-Swapping
#
# Revision 1.8  2002/05/11 20:10:59  skaus
# bugfix: ^Break detection after XMS-Swapping
#
# Revision 1.7  2002/04/23 16:34:51  skaus
# chg: BUILD.BAT to handle XMS-Swap better
#
# Revision 1.6  2002/04/03 01:10:31  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
#
# Revision 1.5  2002/04/02 23:36:37  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
#
# Revision 1.4  2002/04/02 20:58:13  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
#
# Revision 1.3  2002/04/02 18:09:31  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
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
SRC = batch.c cmdtable.c command.c dummies.asm expalias.c init.c kswap.c \
	loadhigh.c module.c redir.c ver.c
OBJ = batch.obj cmdtable.obj command.obj dummies.obj expalias.obj init.obj \
	kswap.obj loadhigh.obj module.obj redir.obj ver.obj
HDR = ..\config.h ..\include/batch.h ..\include/cmdline.h \
	..\include/command.h ..\include/context.h ..\include/crossjmp.h \
	..\include/cswap.h ..\include/datefunc.h ..\include/debug.h \
	..\include/keys.h ..\include/kswap.h ..\include/misc.h \
	..\include/module.h ..\include/mux_ae.h ..\include/nls.h \
	..\include/openf.h ..\include/res.h ..\include/resource.h \
	..\include/timefunc.h ..\include\large.inc ..\include\model.inc \
	..\include\small.inc

XMS_SWAP_PRE_OBJ = cb_catch.obj xms_crit.obj xms_brk.obj cswap.obj
XMS_SWAP_ADD_OBJ = cswapc.obj

.IF $(XMS_SWAP) == $(NULL)
SRC += cb_catch.asm
OBJ += cb_catch.obj
.ELSE
SRC += cswap.asm cswapc.c xms_crit.asm xms_brk.asm cb_catch.asm
HDR += ../include/cswapc.h
OBJ += $(XMS_SWAP_ADD_OBJ)
PRE_OBJ = $(XMS_SWAP_PRE_OBJ)
.ENDIF


#	Default target
all: com.exe

dist : command.mak xms-swap.mak

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

com.exe ?= _OBJS := $(PRE_OBJ) $(OBJ:s/c0.obj//)
# STD_OBJ !:= C0.OBJ
com.exe com.map .UPDATEALL : $(PRE_OBJ) $(OBJ) ../cmd/cmds.lib ../lib/freecom.lib
	$(MAK_EXE)

#MAKEDEP START
expalias.obj : expalias.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/context.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
command.obj : command.c \
	 ../config.h ../context.h_c ../err_fcts.h ../include/batch.h \
	../include/cmdline.h ../include/command.h ../include/crossjmp.h \
	../include/cswap.h ../include/datefunc.h ../include/debug.h \
	../include/kswap.h ../include/misc.h ../include/mux_ae.h \
	../include/nls.h ../include/openf.h ../include/timefunc.h ../strings.h
module.obj : module.c \
	 ../config.h ../context.h_c ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/kswap.h ../include/misc.h \
	../include/module.h ../include/res.h ../include/resource.h \
	../include/timefunc.h
kswap.obj : kswap.c \
	 ../config.h ../context.h_c ../err_fcts.h ../include/command.h \
	../include/context.h ../include/datefunc.h ../include/debug.h \
	../include/kswap.h ../include/misc.h ../include/timefunc.h
ver.obj : ver.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
init.obj : init.c \
	 ../config.h ../context.h_c ../err_fcts.h ../include/cmdline.h \
	../include/command.h ../include/context.h ../include/cswap.h \
	../include/datefunc.h ../include/debug.h ../include/keys.h \
	../include/kswap.h ../include/misc.h ../include/module.h \
	../include/timefunc.h ../strings.h
cmdtable.obj : cmdtable.c \
	 ../config.h ../include/command.h ../include/datefunc.h \
	../include/debug.h ../include/misc.h ../include/timefunc.h ../strings.h
loadhigh.obj : loadhigh.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h ../strings.h
batch.obj : batch.c \
	 ../config.h ../err_fcts.h ../include/batch.h ../include/cmdline.h \
	../include/command.h ../include/datefunc.h ../include/debug.h \
	../include/misc.h ../include/timefunc.h \
	/freedos/src/include/../include/dfn.h
dummies.obj : dummies.asm \
	 .\..\include\large.inc .\..\include\model.inc .\..\include\small.inc
redir.obj : redir.c \
	 ../config.h ../err_fcts.h ../include/cmdline.h ../include/command.h \
	../include/datefunc.h ../include/debug.h ../include/misc.h \
	../include/timefunc.h
DYNSOURCES =
#MAKEDEP STOP

cswapc.c : ../include/cswap.h ../include/misc.h

clobber : clean
	$(RM) $(RMFLAGS) *.com *.cln command.mak com.exe *.cfg

clean :
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj *.exe $(CFG) *.dmp com.com tags errlist

command.mak : makefile command.m1 command.m2
	+copy command.m1 + $(mktmp command.exe : \44(CFG) $(OBJ:t" \\\\\\n\\t") \n\t\44(LD) /m/s/l /c /d @&&|\n\44(LIBPATH)\\c0s.obj+\n$(OBJ:t"+\\n")\n) + command.m2 $@

xms-swap.mak : makefile command.m1 command.m2
	+copy command.m1 + $(mktmp command.exe : \44(CFG) $(OBJ:t" \\\\\\n\\t") $(XMS_SWAP_PRE_OBJ)\n\t\44(LD) /m/s/l /c /d @&&|\n\44(LIBPATH)\\c0s.obj+\n$(XMS_SWAP_PRE_OBJ)+\n$(OBJ:t"+\\n")\n) + command.m2 $@
