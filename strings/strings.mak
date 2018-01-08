CFG_DEPENDENCIES = strings.mak
MMODEL = -mc

TOP=..
!include "$(TOP)/config.mak"

all : strings.h strings.err strings.lib

strings.h : DEFAULT.LNG fixstrs.exe
	fixstrs.exe /lib $(LNG)
	copy strings.h ..

strings.err : DEFAULT.err critstrs.exe
	critstrs.exe $(LNG)

strings.lib: strings.h strings.err
	cd strings
	echo Making STRINGS library
	..\..\scripts\rmfiles $(CFG)
	$(CL) -c *.c
	..\..\scripts\rmfiles strings.lib
	$(AR) strings.lib @strings.rsp $(LIBLIST) strings.lst
	copy strings.lib ..
	copy strings.lst ..
	echo Purging temporary directory of strings library
	..\..\scripts\rmfiles strings.*	makefile errlist *.obj *.c
	cd ..
	rmdir strings

fixstrs.exe: $(CFG) fixstrs.c

critstrs.exe: $(CFG) CRITSTRS.C

#		*Individual File Dependencies*
fixstrs.obj: $(CFG) fixstrs.c

critstrs.obj: $(CFG) critstrs.c
