.AUTODEPEND

CFG = TCCDOS.CFG
CFG_DEPENDENCIES = COMMAND.MAK ..\strings.h

#		*Translator Definitions*
##>> Modify this file with your local settings
!include "..\config.mak"


all: $(CFG) command.exe

command.exe : $(CFG) batch.obj \
	cb_catch.obj \
	cmdtable.obj \
	command.obj \
	dummies.obj \
	expalias.obj \
	init.obj \
	kswap.obj \
	loadhigh.obj \
	module.obj \
	redir.obj \
	ver.obj 
	$(LD) /m/s/l /c /d @&&|
$(LIBPATH)\c0s.obj+
batch.obj+
cb_catch.obj+
cmdtable.obj+
command.obj+
dummies.obj+
expalias.obj+
init.obj+
kswap.obj+
loadhigh.obj+
module.obj+
redir.obj+
ver.obj
command
		# no map file
$(SUPPL_LIB_PATH)\suppl_s.lib +
..\cmd\cmds.lib +
..\lib\freecom.lib +
..\strings\strings.lib +
$(LIBPATH)\cs.lib
|
