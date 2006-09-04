.AUTODEPEND

CFG_DEPENDENCIES = COMMAND.MAK ..\strings.h

!include "..\config.mak"

all: $(CFG) command.exe

command.exe : $(CFG) cswapc.obj \
	batch.obj \
	cmdtable.obj \
	command.obj \
	expalias.obj \
	init.obj \
	kswap.obj \
	loadhigh.obj \
	module.obj \
	redir.obj \
	ver.obj cb_catch.obj xms_crit.obj xms_brk.obj xms_2e.obj cswap.obj \
	$(SUPPL_LIB_PATH)\suppl_s.lib ..\cmd\cmds.lib ..\lib\freecom.lib ..\strings\strings.lib
	$(LD) /m/s/l /c /d @&&|
$(LIBPATH)\c0s.obj+
cb_catch.obj xms_crit.obj xms_brk.obj xms_2e.obj cswap.obj+
cswapc.obj+
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
		# no map file, # dummies.obj
$(SUPPL_LIB_PATH)\suppl_s.lib +
..\cmd\cmds.lib +
..\lib\freecom.lib +
..\strings\strings.lib +
$(LIBPATH)\cs.lib
|
