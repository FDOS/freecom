# $Id$
#
# Makefile for tools of the FreeDOS kernel's command interpreter
#
#	There is no default target!!
#
# Available main targets:
#	ptchldrv.exe	- patch log drive
#	icmd			- Installable Commands samples
#
#
# $Log$
# Revision 1.9  2004/06/29 14:14:57  skaus
# fix: help screen of internal commands causes "Unknown command error" {Bernd Blaauw}
#
# Revision 1.8  2002/04/02 23:36:37  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
#
# Revision 1.7  2001/04/12 00:09:06  skaus
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

.IF $(NASM) == $(NULL)

.INCLUDE : ../_config.mk

.ENDIF

INCDIR +=;$(FREEDOS)\SRC\INCLUDE;..
# LIBDIR +=;$(FREEDOS)\SRC\LIB\$(_COMPILER)
LDLIBS = $(FREEDOS)\SRC\LIB\$(_COMPILER)\Suppl_$(_MODEL).lib
LDFLAGS += /msl

# Project specific C compiler flags
MYCFLAGS_DBG = -DNDEBUG=1 $(null,$(DEBUG) $(NULL) -DDEBUG=1)
MYCFLAGS_NDBG =
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))
# Project specific NASM compiler flags
NASMFLAGS_DBG = $(null,$(DEBUG) $(NULL) -DDEBUG=1) -s
NASMFLAGS_NDBG = -s
NASMFLAGS = $(null,$(NDEBUG) $(NASMFLAGS_DBG) $(NASMFLAGS_NDBG))


.SOURCE.h := . ..
.SOURCE.lib := $(LIBDIR:s/;/ /:u)
.IF $(FRREDOS) == $(NULL)
.IMPORT : FREEDOS
.ENDIF

# Sources of this make target

.IF $(MAKETARGETS:s/all//) == $(NULL)
#	Default target
#	Required only, if no targets were specified on command line
.INCLUDE : default.mk
## all: ptchldrv.exe kssf.com icmd
error:
	errlvl 127
.ENDIF

dist : ;

icmd : load_icd.exe icmd_{1 2 3}.icd

%.icd : %.nas ; $(NASM) -f bin -o $@ $<

icmd_1.icd : icmd_inc.inc icmd_1.nas

icmd_2.icd : icmd_inc.inc icmd_2.nas

icmd_3.icd : icmd_inc.inc icmd_3.nas

icmd_4.icd : icmd_inc.inc icmd_4.nas

.INIT : $(CFG) __errl

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c


ptchldrv.obj : ptchldrv.c

vspawn.com : vspawn.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $(MYASMFLAGS) $<

kssf.com : kssf.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $(MYASMFLAGS) $<

load_icd.obj : load_icd.c

.INCLUDE : makefile.1

