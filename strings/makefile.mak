CFG_DEPENDENCIES = makefile.mak

COMPACT_MODEL = 1

TOP = ..
!include "$(TOP)/config.mak"

all : $(CFG) strings.h strings.err fixstrs.exe

strings.h : DEFAULT.lng
	..$(DIRSEP)utilsc$(DIRSEP)fixstrs.exe $(FIXSTRSOPT) $(LNG)
	$(CP) strings.h ..

strings.err : DEFAULT.err
	..$(DIRSEP)utilsc$(DIRSEP)critstrs.exe $(LNG)

fixstrs.exe: fixstrs.c
