.AUTODEPEND

CFG = TCCDOS.CFG

#		*Translator Definitions*
##>> Modify this file with your local settings
!include "config.mak"


all: command.com

#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

command.exe : $(CFG) alias.obj \
	batch.obj \
	beep.obj \
	break.obj \
	call.obj \
	cb_catch.obj \
	cls.obj \
	cmdinput.obj \
	cmdline.obj \
	cmdtable.obj \
	command.obj \
	copy.obj \
	ctty.obj \
	date.obj \
	datefunc.obj \
	debug.obj \
	del.obj \
	dir.obj \
	dstack.obj \
	echo.obj \
	environ.obj \
	err_hand.obj \
	error.obj \
	exec.obj \
	fddebug.obj \
	filecomp.obj \
	for.obj \
	goto.obj \
	history.obj \
	if.obj \
	init.obj \
	internal.obj \
	lh.obj \
	loadhigh.obj \
	lowexec.obj \
	messages.obj \
	misc.obj \
	mux_ae.obj \
	nls.obj \
	openf.obj \
	parsenum.obj \
	path.obj \
	pause.obj \
	prompt.obj \
	redir.obj \
	ren.obj \
	session.obj \
	set.obj \
	shift.obj \
	spawn.obj \
	swapexec.obj \
	tempfile.obj \
	time.obj \
	timefunc.obj \
	tmpnam.obj \
	truename.obj \
	type.obj \
	ver.obj \
	verify.obj \
	where.obj 
	$(TLINK) /x /c /d @&&|
$(LIBPATH)\c0s.obj+
alias.obj+
batch.obj+
beep.obj+
break.obj+
call.obj+
cb_catch.obj+
cls.obj+
cmdinput.obj+
cmdline.obj+
cmdtable.obj+
command.obj+
copy.obj+
ctty.obj+
date.obj+
datefunc.obj+
debug.obj+
del.obj+
dir.obj+
dstack.obj+
echo.obj+
environ.obj+
err_hand.obj+
error.obj+
exec.obj+
fddebug.obj+
filecomp.obj+
for.obj+
goto.obj+
history.obj+
if.obj+
init.obj+
internal.obj+
lh.obj+
loadhigh.obj+
lowexec.obj+
messages.obj+
misc.obj+
mux_ae.obj+
nls.obj+
openf.obj+
parsenum.obj+
path.obj+
pause.obj+
prompt.obj+
redir.obj+
ren.obj+
session.obj+
set.obj+
shift.obj+
spawn.obj+
swapexec.obj+
tempfile.obj+
time.obj+
timefunc.obj+
tmpnam.obj+
truename.obj+
type.obj+
ver.obj+
verify.obj+
where.obj
command
		# no map file
$(SUPPL_LIB_PATH)\suppl_s.lib+
$(LIBPATH)\cs.lib
|

command.com : command.exe
	copy /b command.exe + strings.dat command.com

cb_catch.obj: $(CFG) cb_catch.asm
	$(TASM) $(ASMFLAGS) CB_CATCH.ASM,CB_CATCH.OBJ

lh.obj: $(CFG) lh.asm
	$(TASM) $(ASMFLAGS) LH.ASM,LH.OBJ

lowexec.obj: $(CFG) lowexec.asm
	$(TASM) $(ASMFLAGS) LOWEXEC.ASM,LOWEXEC.OBJ

spawn.obj: $(CFG) spawn.asm
	$(TASM) $(ASMFLAGS) SPAWN.ASM,SPAWN.OBJ

strings.h:
	$(COMSPEC) /c fixstrs.bat tc101

#		*Compiler Configuration File*
#-h
#-Vf
#-Ff
#-C
#-L$(LIBPATH)

# added strings.h here because $(CFG) is included everywhere already
$(CFG): command.mak strings.h
  copy &&|
-a
-f-
-ff-
-K
-w+
-O
-Z
-k-
-d
-b-
-I$(INCLUDEPATH)
-D_NO__DOS_DATE
-D_NO__DOS_TIME
| $(CFG)
