.AUTODEPEND

#		*Translator Definitions*
CC = tcc +COMMAND.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = C:\TC\LIB
INCLUDEPATH = C:\TC\INCLUDE;.\SUPPL


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 alias.obj \
 batch.obj \
 cmdinput.obj \
 command.obj \
 dir.obj \
 environ.obj \
 exec.obj \
 history.obj \
 internal.obj \
 loadhigh.obj \
 prompt.obj \
 redir.obj \
 where.obj \
 lowexec.obj \
 lh.obj \
 tempfile.obj \
 date.obj \
 err_hand.obj \
 type.obj \
 time.obj \
 misc.obj \
 error.obj \
 del.obj \
 ren.obj \
 set.obj \
 ver.obj \
 cmdtable.obj \
 truename.obj \
 call.obj \
 echo.obj \
 for.obj \
 goto.obj \
 if.obj \
 beep.obj \
 pause.obj \
 shift.obj \
 path.obj \
 cls.obj \
 break.obj \
 verify.obj \
 datefunc.obj \
 timefunc.obj \
 filecomp.obj \
 init.obj \
 ctty.obj \
 copy.obj \
 messages.obj \
 cb_catch.obj \
 tmpnam.obj \
 openf.obj \
 parsenum.obj \
 dstack.obj \
 {$(LIBPATH)}suppl_s.lib \
 swapexec.obj \
 spawn.obj

#		*Explicit Rules*
command.exe: command.cfg $(EXE_dependencies)
  $(TLINK) /s/n/c/L$(LIBPATH) @&&|
c0s.obj+
alias.obj+
batch.obj+
cmdinput.obj+
command.obj+
dir.obj+
environ.obj+
exec.obj+
history.obj+
internal.obj+
loadhigh.obj+
prompt.obj+
redir.obj+
where.obj+
lowexec.obj+
lh.obj+
tempfile.obj+
date.obj+
err_hand.obj+
type.obj+
time.obj+
misc.obj+
error.obj+
del.obj+
ren.obj+
set.obj+
ver.obj+
cmdtable.obj+
truename.obj+
call.obj+
echo.obj+
for.obj+
goto.obj+
if.obj+
beep.obj+
pause.obj+
shift.obj+
path.obj+
cls.obj+
break.obj+
verify.obj+
datefunc.obj+
timefunc.obj+
filecomp.obj+
init.obj+
ctty.obj+
copy.obj+
messages.obj+
cb_catch.obj+
tmpnam.obj+
openf.obj+
parsenum.obj+
swapexec.obj+
dstack.obj+
spawn.obj
command,command
suppl_s.lib+
cs.lib
|


#		*Individual File Dependencies*
alias.obj: command.cfg alias.c

batch.obj: command.cfg batch.c

dstack.obj: command.cfg dstack.c

cmdinput.obj: command.cfg cmdinput.c

command.obj: command.cfg command.c

dir.obj: command.cfg dir.c

environ.obj: command.cfg environ.c

exec.obj: command.cfg exec.c

history.obj: command.cfg history.c

internal.obj: command.cfg internal.c

loadhigh.obj: command.cfg loadhigh.c

prompt.obj: command.cfg prompt.c

redir.obj: command.cfg redir.c

where.obj: command.cfg where.c

lowexec.obj: command.cfg lowexec.asm
	$(TASM) /MX /ZI /O LOWEXEC.ASM,LOWEXEC.OBJ

lh.obj: command.cfg lh.asm
	$(TASM) /MX /ZI /O LH.ASM,LH.OBJ

tempfile.obj: command.cfg tempfile.c

date.obj: command.cfg date.c

err_hand.obj: command.cfg err_hand.c

type.obj: command.cfg type.c

time.obj: command.cfg time.c

misc.obj: command.cfg misc.c

error.obj: command.cfg error.c

del.obj: command.cfg del.c

ren.obj: command.cfg ren.c

set.obj: command.cfg set.c

ver.obj: command.cfg ver.c

cmdtable.obj: command.cfg cmdtable.c

truename.obj: command.cfg truename.c

call.obj: command.cfg call.c

echo.obj: command.cfg echo.c

for.obj: command.cfg for.c

goto.obj: command.cfg goto.c

if.obj: command.cfg if.c

beep.obj: command.cfg beep.c

pause.obj: command.cfg pause.c

shift.obj: command.cfg shift.c

path.obj: command.cfg path.c

cls.obj: command.cfg cls.c

break.obj: command.cfg break.c

verify.obj: command.cfg verify.c

datefunc.obj: command.cfg datefunc.c

timefunc.obj: command.cfg timefunc.c

filecomp.obj: command.cfg filecomp.c

init.obj: command.cfg init.c

ctty.obj: command.cfg ctty.c

copy.obj: command.cfg copy.c

messages.obj: command.cfg messages.c

cb_catch.obj: command.cfg cb_catch.asm
	$(TASM) /MX /ZI /O CB_CATCH.ASM,CB_CATCH.OBJ

tmpnam.obj: command.cfg tmpnam.c

openf.obj: command.cfg openf.c

parsenum.obj: command.cfg parsenum.c

swapexec.obj: command.cfg swapexec.c

spawn.obj: command.cfg spawn.asm
	$(TASM) /MX /ZI /O SPAWN.ASM,SPAWN.OBJ

#		*Compiler Configuration File*
command.cfg: command.mak
  copy &&|
-a
-f-
-ff-
-K
-C
-w+
-O
-Z
-k-
-d
-h
-b-
-H=COMMAND.SYM
-Vf
-I$(INCLUDEPATH)
-L$(LIBPATH)
-Ff
| command.cfg


