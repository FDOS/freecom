# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
# Revision 1.1  2000/06/23 06:02:56  skaus
# chg: ECHO, FDDEBUG, BREAK, VERIFY: share ON/OFF recognition code
# chg: shellname & shellver: type change: * --> const[]
# add: DIRS, PUSHD, POPD, CDD, FEATURE_LAST_DIR
# add: MISC.C: changeDrive(), cwd(), drvNum()
# chg: Display text: CMDHELP_CD, ERROR_INVALID_DRIVE
# bugfix: option are peristent for VAR command
# bugfix: "CD C:" must display current working directory of drive C:
# bugfix: unquote(): calculation of portion before left quote
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
LDFLAGS += /msl
_MODEL !:= t
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

strings.h strings.dat .UPDATEALL : fixstrs.com strings.txt
	+fixstrs.com

fixstrs.com : fixstrs.c 

#MAKEDEP START
#MAKEDEP STOP

clobber : clean
	$(RM) $(RMFLAGS) *.exe *.com ..\strings.h ..\strings.dat

clean :
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj $(CFG) strings.h strings.dat


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


.ENDIF
#-weas	\
#-wpre	\
#-w-pro	\

