# $Id$
#
# Makefile for the FreeDOS kernel's command interpreter
#
# $Log$
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

.INCLUDE : ../_config.mk

LDLIBS !:= 

.IF $(LNG) != $(NULL)

_LNG = $(LNG:u:s/.LNG//)
_LLNG = $(_LNG).LNG
_ELNG = $(LNG).ERR

.ELSE

_LLNG = 
_ELNG = 

.ENDIF

OBJ :=

#	Default target
all: strings.h strings.dat strings.lib strings.err


dist : ;

.INIT : $(CFG) __errl

__errl:
	@+-if exist errlist del errlist >nul
	-ctags *.c ..\lib\*.c

strings.h strings.dat .UPDATEALL : fixstrs.exe default.lng $(_LLNG)
	fixstrs.exe /lib $(LNG)
	+copy strings.h .. >nul

strings.lib : fixstrs.exe default.lng
	$(RUNMAKE) strings_lib strings_lib_clean

strings_lib .SETDIR=strings :
	$(RUNMAKE) NDEBUG!:=Yes DEBUG!:=
	+copy strings.lib ..
	+copy strings.lst ..

strings_lib_clean :
	$(RM) -fr strings

strings.err : critstrs.exe default.err $(_ELNG)
	critstrs.exe $(LNG)

fixstrs.exe ?= _MODEL !:= c
fixstrs.exe : fixstrs.c ../include/strings.typ

critstrs.exe ?= _MODEL !:= c
critstrs.exe : critstrs.c

#MAKEDEP START
#MAKEDEP STOP

clobber : clean
	$(RM) $(RMFLAGS) *.exe *.com ..\strings.h ..\strings.dat ..\string.err

clean : strings_lib_clean
	$(RM) $(RMFLAGS) *.lst *.map *.bin *.bak *.las *.obj $(CFG) strings.h strings.dat
	$(RM) $(RMFLAGS) *.log strings.err strings.lib strings.lst


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

