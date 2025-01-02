CFG_DEPENDENCIES = makefile.mak

TOP2LVL = 1
TOP = ../..
!include "$(TOP)/config.mak"

all : $(CFG) strings.lib

!include "strings.mak"

strings.lib: ../strings.h ../strings.err $(STRINGS_DEPS)
	echo Making STRINGS library
	$(RMFILES) strings.lib
	$(AR) strings.lib @strings.rsp $(LIBLIST) strings.lst
	$(CP) strings.lib ..
	$(CP) strings.lst ..
