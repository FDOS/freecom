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
# Revision 1.2.4.3  2001/02/18 21:08:24  skaus
# add: command WHICH
# fix: BUILD.BAT and accompanying makefiles for TC++ v1.01
#
# Revision 1.2.4.2  2001/02/18 17:59:37  skaus
# bugfix: KSSF: restore parentPSP on exit
# chg: Using STRINGS resource for all non-interactive messages
# chg: moving all assembly files to NASM
#
# Revision 1.2.4.1  2001/02/06 16:30:14  skaus
# chg: FreeCOM no longer uses the shell hack if kswap is active
# bugfix: aliases were read back incorrectly
#
# Revision 1.4  2000/12/10 04:16:12  skaus
# add: Installable Commands FEATURE_INSTALLABLE_COMMANDS
# add: F1, F3, F5, cur-right command line editing
# relased version 0.80
#
# Revision 1.3  2000/08/15 21:31:38  skaus
# chg: binary packages on FTP\nbugfix: piping
#
# Revision 1.2  2000/07/09 21:57:25  skaus
# + Support for international strings without recompiling
# + Useage of TC++1
#

INCDIR +=;$(FREEDOS)\SRC\INCLUDE;..
LIBDIR +=;$(FREEDOS)\SRC\LIB\$(_COMPILER)
# LDLIBS = suppl_$(_MODEL).lib
LDFLAGS += /msl

# Project specific C compiler flags
MYCFLAGS_DBG = -DNDEBUG=1 $(null,$(DEBUG) $(NULL) -DDEBUG=1)
MYCFLAGS_NDBG =
MYASMFLAGS_DBG = $(null,$(DEBUG) $(NULL) -DDEBUG=1)
MYASMFLAGS_NDBG =
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))
MYASMFLAGS = $(null,$(NDEBUG) $(MYASMFLAGS_DBG) $(MYASMFLAGS_NDBG))

.SOURCE.h := . ..
.SOURCE.lib := $(LIBDIR:s/;/ /:u)
.IMPORT : FREEDOS

# Sources of this make target

#	Default target
.INCLUDE : default.mk
## all: ptchldrv.exe kssf.com icmd
error:
	errlvl 127

icmd : load_icd.exe icmd_{1 2 3}.icd

%.icd : %.nas ; nasm -f bin -o $@ $<

icmd_1.icd : icmd_inc.inc icmd_1.nas

icmd_2.icd : icmd_inc.inc icmd_2.nas

icmd_3.icd : icmd_inc.inc icmd_3.nas

.INIT : $(CFG) __errl

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c


ptchldrv.obj : ptchldrv.c

kssf.com : kssf.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $(MYASMFLAGS) $<

load_icd.obj : load_icd.c

.INCLUDE : makefile.1

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


