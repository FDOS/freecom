# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.2  2000/07/09 21:57:25  skaus
# + Support for international strings without recompiling
# + Useage of TC++1
#

INCDIR +=;$(FREEDOS)\SRC\INCLUDE;..
LIBDIR +=;$(FREEDOS)\SRC\LIB\$(_COMPILER)
# LDLIBS = suppl_$(_MODEL).lib
LDFLAGS += /msl

# Project specific C compiler flags
MYCFLAGS_DBG = -DDEBUG=1
MYCFLAGS_NDBG =
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))

.SOURCE.h := . ..
.SOURCE.lib := $(LIBDIR:s/;/ /:u)
.IMPORT : FREEDOS

# Sources of this make target

#	Default target
all: ptchldrv.exe


.INIT : $(CFG) __errl

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c


ptchldrv.obj : ptchldrv.c


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


