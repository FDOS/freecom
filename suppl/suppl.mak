CFG_DEPENDENCIES = suppl.mak

!include "..\config.mak"

all : $(CFG)
	copy $(CFG) src

clean :
	clnsuppl.bat
