CFG_DEPENDENCIES = suppl.mak

!include "..\config.mak"

all : src\$(CFG)

src\$(CFG) : $(CFG)
	copy $(CFG) src

clean :
	clnsuppl.bat
