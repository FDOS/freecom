CFG_DEPENDENCIES = makefile.mak ../strings.h

TOP = ..
!include "$(TOP)/config.mak"

all: command.exe

OBJ1 =	$(OBJC0) cb_catch.obj termhook.obj
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
LIBS = 	..$(DIRSEP)cmd$(DIRSEP)cmds.lib ..$(DIRSEP)lib$(DIRSEP)freecom.lib \
..$(DIRSEP)strings$(DIRSEP)strings.lib \
$(SUPPL_LIB_PATH)$(DIRSEP)suppl_$(SHELL_MMODEL).lib

command.rsp : $(ECHOTODEP)
	$(RMFILES) command.rsp
	$(ECHOTO) command.rsp $(OBJ1)+
	$(ECHOTO) command.rsp $(OBJ2)+
	$(ECHOTO) command.rsp $(OBJ3)+
	$(ECHOTO) command.rsp $(OBJ4)
	$(ECHOTO) command.rsp command.exe
	$(ECHOTO) command.rsp command.map
	$(ECHOTO) command.rsp $(LIBS)+
	$(ECHOTO) command.rsp $(LIBC)

command.exe : $(CFG) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(LIBS) $(LD_RSP)
	$(LD)
