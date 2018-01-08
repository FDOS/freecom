CFG_DEPENDENCIES = suppl.mak

TOP=..
!include "$(TOP)/config.mak"

all : src/$(CFG)

src/$(CFG) : $(CFG)
	copy $(CFG) src

clean :
	clnsuppl.bat
