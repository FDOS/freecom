CFG_DEPENDENCIES = makefile.mak

FIXSTRS_BUILD = 1

TOP=..
!include "$(TOP)/config.mak"

all : strings.h strings.err

strings.h : DEFAULT.lng fixstrs.exe
	fixstrs.exe --lib $(LNG)
	$(CP) strings.h ..

strings.err : DEFAULT.err critstrs.exe
	critstrs.exe $(LNG)

fixstrs.exe: $(CFG) fixstrs.c

critstrs.exe: $(CFG) critstrs.c

#		*Individual File Dependencies*
fixstrs.obj: $(CFG) fixstrs.c

critstrs.obj: $(CFG) critstrs.c
