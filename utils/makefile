# $Id$
#
# Makefile for tools of the FreeDOS kernel's command interpreter
#
#
# $Log$
# Revision 1.5  2002/04/02 23:36:37  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
#
# Revision 1.4  2001/04/12 00:09:06  skaus
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
# Revision 1.1.2.1  2001/02/18 17:59:37  skaus
# bugfix: KSSF: restore parentPSP on exit
# chg: Using STRINGS resource for all non-interactive messages
# chg: moving all assembly files to NASM
#

INCDIR +=;$(FREEDOS)\SRC\INCLUDE;..
LIBDIR +=;$(FREEDOS)\SRC\LIB\$(_COMPILER)
# LDLIBS = suppl_$(_MODEL).lib
LDFLAGS += /msl

# Project specific C compiler flags
MYCFLAGS_DBG = -DNDEBUG=1 $(null,$(DEBUG) $(NULL) -DDEBUG=1)
MYCFLAGS_NDBG =
MYASMFLAGS_DBG = $(null,$(DEBUG) $(NULL) -DDEBUG=1) -s
MYASMFLAGS_NDBG = -s
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))
MYASMFLAGS = $(null,$(NDEBUG) $(MYASMFLAGS_DBG) $(MYASMFLAGS_NDBG))

.SOURCE.h := . ..
.SOURCE.lib := $(LIBDIR:s/;/ /:u)
.IMPORT : FREEDOS

# Sources of this make target

#	Default target
all: mkctxt.exe mktools.exe chunk.exe mkinfres.exe

mkctxt.exe : mkctxt.c

chunk.exe : chunk.c

mktools.exe : mktools.c ..\config.h

dist : ;

.INIT : $(CFG) __errl

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c



clobber : clean
	$(RM) $(RMFLAGS) *.exe *.com *.cfg

clean :
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj $(CFG) *.dmp

