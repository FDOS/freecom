.AUTODEPEND

CFG = TCCDOS.CFG

#		*Translator Definitions*
##>> Modify this file with your local settings
!include "config.mak"


all: $(CFG) command.com

#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }
.asm.obj:
	$(NASM) $(NASMFLAGS) -f obj $<

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
	dummies.obj \
	echo.obj \
	environ.obj \
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
	kswap.obj \
	loadhigh.obj \
	lowexec.obj \
	messages.obj \
	misc.obj \
	module.obj \
	mux_ae.obj \
	nls.obj \
	openf.obj \
	parsenum.obj \
	path.obj \
	pause.obj \
	prompt.obj \
	redir.obj \
	ren.obj \
	res.obj \
	res_r.obj \
	res_w.obj \
	set.obj \
	shift.obj \
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
dummies.obj+
echo.obj+
environ.obj+
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
kswap.obj+
loadhigh.obj+
lowexec.obj+
messages.obj+
misc.obj+
module.obj+
mux_ae.obj+
nls.obj+
openf.obj+
parsenum.obj+
path.obj+
pause.obj+
prompt.obj+
redir.obj+
ren.obj+
res.obj+
res_r.obj+
res_w.obj+
set.obj+
shift.obj+
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
	copy /b command.exe + criter.mod\criter + criter.mod\criter1 + strings.dat command.com

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
