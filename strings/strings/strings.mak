CFG_DEPENDENCIES = strings.mak

TOP=../..
!include "$(TOP)/config.mak"

all : strings.lib

strings.lib: ../strings.h ../strings.err
	echo Making STRINGS library
	$(CL) -c *.c
	$(RMFILES2) strings.lib
	$(AR) strings.lib @strings.rsp $(LIBLIST) strings.lst
	$(CP) strings.lib ..
	$(CP) strings.lst ..
