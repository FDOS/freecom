CFG = TCCDos.Cfg
CFG_DEPENDENCIES = suppl.mak

!include "..\config.mak"

all : $(CFG) suppl_$(SHELL_MMODEL).lib

suppl_$(SHELL_MMODEL).lib: # suppl.tgz untar.exe
	echo SET CFG=$(CFG)>vars.bat
	echo SET SUPPL=suppl_$(SHELL_MMODEL)>>vars.bat
	echo SET CC=$(CC)>>vars.bat
	echo SET CFLAGS=$(CFLAGS)>>vars.bat
	echo SET AR=$(AR)>>vars.bat
	echo >compile.me

clean :
	clnsuppl.bat
