# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.15.4.6  2001/07/30 00:45:16  skaus
# Update #13 / Beta 27: plain dynamic context
#
# Revision 1.15.4.5  2001/07/25 20:17:28  skaus
# Update #12
#
# Revision 1.15.4.2  2001/07/16 20:28:36  skaus
# Update #9
#
# Revision 1.15.4.1  2001/07/05 22:18:33  skaus
# Update #5
#
# Revision 1.15  2001/04/29 11:33:50  skaus
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
# Revision 1.14  2001/04/12 00:09:06  skaus
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

.IMPORT : FREEDOS
.IMPORT .IGNORE : LNG DEBUG NDEBUG

INCDIR +=;$(FREEDOS)\SRC\INCLUDE;$(PWD)\\INCLUDE
LIBDIR +=;$(FREEDOS)\SRC\LIB\$(_COMPILER)
LDLIBS = CMD\\CMDS.LIB LIB\\FREECOM.LIB STRINGS\\STRINGS.LIB $(FREEDOS)\SRC\LIB\$(_COMPILER)\Suppl_$(_MODEL).lib
LDFLAGS += /msl
NASM *= c:\TOOL\NASMW.EXE

# Use BC5 linker in order to use TD of BC5
LD_TLINK != D:\BC5\BIN\TLINK.EXE

# Project specific C compiler flags
MYCFLAGS_DBG = -UNDEBUG $(null,$(DEBUG) $(NULL) -DDEBUG=1)
MYCFLAGS_NDBG = -DNDEBUG=1 -UDEBUG
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))

# Project specific NASM compiler flags
NASMFLAGS_DBG = $(null,$(DEBUG) $(NULL) -DDEBUG=1) -s
NASMFLAGS_NDBG = -s
NASMFLAGS = $(null,$(NDEBUG) $(NASMFLAGS_DBG) $(NASMFLAGS_NDBG))

RUNMAKE = $(MAKE) LNG=$(LNG) NASM=$(NASM) DEBUG=$(DEBUG) NDEBUG=$(NDEBUG) MODEL=$(MODEL)

GLOBAL_DEPS = err_fcts.h

.SOURCE.lib := $(LIBDIR:s/;/ /:u)

# Sources of this make target
SRC = env_clear.c
OBJ = env_clear.obj
HDR =


.INCLUDE .IGNORE : lastmake.mk

THISMAKE !:= $(_COMPILER):$(_MODEL):$(LNG):$(null,$(NDEBUG) DBG NDBG):$(LNG)

#	Default target
all: com.com tools

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
	-ctags cmd\*.c lib\*.c shell\*.c \freedos\src\lib\suppl\*.c include\*.h \freedos\src\lib\suppl\*.h

com.com .SEQUENTIAL : utils strings criter context lib cmd shell\\com.exe infores
	@+copy /b shell\\com.exe + infores + criter\\criter + criter\\criter1 + strings\\strings.dat $@

infores : config.h include/command.h shell\\com.exe shell/com.map
	utils\mkinfres.exe /T$@.txt $@ shell\\com.map shell\\com.exe

context : gflags.h_c

gflags.h_c: context.h_c
	utils\\mkgflags.exe <$< >$@

shell\\com.exe : shell

subdirs.mk : makefile
	perl mksubmk.pl >$@

.INCLUDE : subdirs.mk

err_fcts.h : lib\\err_fcts.src lib\\efct_*.c ; perl lib\\scanerr.pl lib\\err_fcts.src lib\\efct_*.c >$@

#MAKEDEP START
env_clear.obj : env_clear.c
DYNSOURCES =
#MAKEDEP STOP

clobber : $(CLOBBER_DEPENDENCIES) my_clean
	$(RM) $(RMFLAGS) *.com *.cln subdirs.mk err_fcts.h *.cfg

clean : $(CLEAN_DEPENDENCIES) 

dist : err_fcts.h utils $(DIST_DEPENDENCIES)

command.lsm : command.com
	perl get_vers.pl $<


my_clean :
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj *.exe $(CFG) *.dmp
	$(RM) $(RMFLAGS) strings.dat strings.h context.h_c criter1 criter2
	$(RM) $(RMFLAGS) context.inc gflags.h_c infores infores.txt

