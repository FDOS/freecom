CFG_DEPENDENCIES = strings.mak
MMODEL = -mc

TOP=..
!include "$(TOP)/config.mak"

all : strings.h strings.err

strings.h : DEFAULT.LNG fixstrs.exe
	fixstrs.exe /lib $(LNG)
	$(CP) strings.h ..

strings.err : DEFAULT.err CRITSTRS.exe
	CRITSTRS.exe $(LNG)

fixstrs.exe: $(CFG) fixstrs.c

CRITSTRS.exe: $(CFG) CRITSTRS.C

#		*Individual File Dependencies*
fixstrs.obj: $(CFG) fixstrs.c

critstrs.obj: $(CFG) critstrs.c
