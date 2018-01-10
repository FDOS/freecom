CFG_DEPENDENCIES = strings.mak
MMODEL = -mc

TOP=..
!include "$(TOP)/config.mak"

all : strings.h strings.err strings.lib

strings.h : DEFAULT.LNG fixstrs.exe
	fixstrs.exe /lib $(LNG)
	$(CP) strings.h ..

strings.err : DEFAULT.err critstrs.exe
	critstrs.exe $(LNG)

strings.lib: strings.h strings.err
	cd strings
	echo Making STRINGS library
	$(RMFILES2) $(CFG)
	$(CL) -c *.c
	$(RMFILES2) strings.lib
	$(AR) strings.lib @strings.rsp $(LIBLIST) strings.lst
	$(CP) strings.lib ..
	$(CP) strings.lst ..
	echo Purging temporary directory of strings library
	$(RMFILES2) strings.*	makefile errlist *.obj *.c *.o
	cd ..
	rmdir strings

fixstrs.exe: $(CFG) fixstrs.c

critstrs.exe: $(CFG) CRITSTRS.C

#		*Individual File Dependencies*
fixstrs.obj: $(CFG) fixstrs.c

critstrs.obj: $(CFG) critstrs.c
