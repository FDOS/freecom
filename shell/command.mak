CFG_DEPENDENCIES = COMMAND.MAK ..\strings.h

!include "..\config.mak"

all: command.exe

OBJ1 =	$(LIBPATH)\c0s.obj\
	cb_catch.obj
!if $(XMS_SWAP)
OBJ2 =	xms_crit.obj\
	xms_brk.obj\
	xms_2e.obj\
	cswap.obj\
	cswapc.obj
!else
OBJ2 =	dummies.obj
!endif
OBJ3 =	batch.obj\
	cmdtable.obj\
	command.obj\
	expalias.obj\
	init.obj\
	kswap.obj\
	loadhigh.obj\
	module.obj
OBJ4 =	redir.obj\
	ver.obj
LIBS = 	$(SUPPL_LIB_PATH)\suppl_$(SHELL_MMODEL).lib ..\cmd\cmds.lib \
..\lib\freecom.lib ..\strings\strings.lib $(LIBPATH)\c$(SHELL_MMODEL).lib

echoto.bat: ..\scripts\echoto.bat
	copy ..\scripts\echoto.bat

command.rsp : echoto.bat
        ..\scripts\rmfiles command.rsp
	echoto command.rsp $(OBJ1)+
	echoto command.rsp $(OBJ2)+
	echoto command.rsp $(OBJ3)+
	echoto command.rsp $(OBJ4)
	echoto command.rsp command.exe
	echoto command.rsp command.map
	echoto command.rsp $(LIBS)

command.exe : $(CFG) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(LIBS) command.rsp
	$(LD) /m/s/l /c/d @command.rsp
