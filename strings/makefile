# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.5  2000/08/15 21:31:37  skaus
# chg: binary packages on FTP\nbugfix: piping
#
# Revision 1.4  2000/07/24 23:00:20  skaus
# fix: DIR & COPY, envSize, Makefiles
#
# Revision 1.3.2.1  2000/07/24 22:38:10  skaus
# add: Spanish Strings
# fix: Makefiles COMMAND.MAK for TC++ v1.01
# fix: environment size between 256..32767
# fix: COPY (to be tested)
# bugfix: DIR /S; output formatting to match MS COMMAND.COM
#
# Revision 1.3  2000/07/10 22:26:56  skaus
# Change internal strings without re-compiling FreeCOM
#
# Revision 1.2  2000/07/09 21:57:25  skaus
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

INCDIR +=;$(FREEDOS)\SRC\INCLUDE
LIBDIR +=;$(FREEDOS)\SRC\LIB\$(_COMPILER)
#LDLIBS = suppl_$(_MODEL).lib
LDFLAGS += /m/s/l
_MODEL !:= c
## Disable debugging
NDEBUG !:= Yes

# Project specific C compiler flags
MYCFLAGS_DBG = -DDEBUG=1
MYCFLAGS_NDBG =
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))

OBJ :=

.SOURCE.lib := $(LIBDIR:s/;/ /:u)
.IMPORT : FREEDOS

# Sources of this make target


#	Default target
all: strings.h strings.dat 
	+copy strings.h .. >nul
	+copy strings.dat .. >nul


.INIT : $(CFG) __errl

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c

strings.h strings.dat .UPDATEALL : fixstrs.exe default.lng
	+fixstrs.exe $(LNG)

fixstrs.exe : fixstrs.c ../strings.typ

#MAKEDEP START
#MAKEDEP STOP

clobber : clean
	$(RM) $(RMFLAGS) *.exe *.com ..\strings.h ..\strings.dat

clean :
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj $(CFG) strings.h strings.dat
	$(RM) $(RMFLAGS) *.log


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

