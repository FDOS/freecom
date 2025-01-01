CFG_DEPENDENCIES = makefile.mak

TOP = ..
!include "$(TOP)/config.mak"

all : src/$(CFG)

src/$(CFG) : $(CFG)
	$(CP) $(CFG) src

clean :
	clnsuppl.bat
