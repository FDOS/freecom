# $Id$
#
# Makefile for tools of the FreeDOS kernel's command interpreter
#
#
# $Log$
# Revision 1.2  2001/03/07 19:49:01  skaus
# Merged in Swap Support changes
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
all: mkctxt.exe mktools.exe

mkctxt.exe : mkctxt.c

mktools.exe : mktools.c ..\config.h

.INIT : $(CFG) __errl

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c



clobber : clean
	$(RM) $(RMFLAGS) *.exe *.com 

clean :
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj $(CFG) *.dmp


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


