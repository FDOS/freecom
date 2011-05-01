CFG_DEPENDENCIES = COMMAND.MAK ..\strings.h

TOP=..
!include "$(TOP)/config.mak"

all: command.exe

OBJ1 =	$(OBJC0) cb_catch.obj
!if $(XMS_SWAP)0 == 0
OBJ2 =	dummies.obj
!else
OBJ2 =	xms_crit.obj\
	xms_brk.obj\
	xms_2e.obj\
	cswap.obj\
	cswapc.obj
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
..\lib\freecom.lib ..\strings\strings.lib $(LIBC)

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
	$(LD) @command.rsp
