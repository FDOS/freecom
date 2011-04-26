.AUTODEPEND

CFG_DEPENDENCIES = COMMAND.MAK ..\strings.h

!include "..\config.mak"

all: $(CFG) command.exe

!if $(XMS_SWAP)
XMS_SWAP = xms_crit.obj xms_brk.obj xms_2e.obj cswap.obj cswapc.obj
!else
XMS_SWAP = dummies.obj
!endif

command.exe : $(CFG) batch.obj \
	cmdtable.obj \
	command.obj \
	expalias.obj \
	init.obj \
	kswap.obj \
	loadhigh.obj \
	module.obj \
	redir.obj \
	ver.obj \
	cb_catch.obj \
	$(XMS_SWAP) \
	$(SUPPL_LIB_PATH)\suppl_$(SHELL_MMODEL).lib ..\cmd\cmds.lib ..\lib\freecom.lib ..\strings\strings.lib
	$(LD) /m/s/l /c /d @&&|
$(LIBPATH)\c0$(SHELL_MMODEL).obj+
cb_catch.obj $(XMS_SWAP)+
batch.obj+
cmdtable.obj+
command.obj+
expalias.obj+
init.obj+
kswap.obj+
loadhigh.obj+
module.obj+
redir.obj+
ver.obj
command
		# no map file
$(SUPPL_LIB_PATH)\suppl_$(SHELL_MMODEL).lib +
..\cmd\cmds.lib +
..\lib\freecom.lib +
..\strings\strings.lib +
$(LIBPATH)\c$(SHELL_MMODEL).lib
|
