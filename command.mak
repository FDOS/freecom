.AUTODEPEND

#		*Translator Definitions*
CC = \bc45\bin\bcc +COMMAND.CFG
TASM = \bc45\bin\TASM
TLIB = \bc45\bin\tlib
TLINK = \bc45\bin\tlink
LIBPATH = C:\BC45\LIB
INCLUDEPATH = C:\BC45\INCLUDE;SUPPL


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 alias.obj \
 batch.obj \
 beep.obj \
 break.obj \
 call.obj \
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
 echo.obj \
 environ.obj \
 error.obj \
 err_hand.obj \
 exec.obj \
 fddebug.obj \
 filecomp.obj \
 for.obj \
 goto.obj \
 history.obj \
 if.obj \
 init.obj \
 internal.obj \
 loadhigh.obj \
 messages.obj \
 misc.obj \
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
 swapexec.obj \
 tempfile.obj \
 time.obj \
 timefunc.obj \
 tmpnam.obj \
 truename.obj \
 type.obj \
 ver.obj \
 verify.obj \
 where.obj \
 cb_catch.obj \
 lh.obj \
 lowexec.obj \
 spawn.obj \
 {$(LIBPATH)}suppl_s.lib

#		*Explicit Rules*
command.exe: command.cfg $(EXE_dependencies)
  $(TLINK) /x/c/d/L$(LIBPATH) @&&|
c0s.obj+
alias.obj+
batch.obj+
beep.obj+
break.obj+
call.obj+
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
echo.obj+
environ.obj+
error.obj+
err_hand.obj+
exec.obj+
fddebug.obj+
filecomp.obj+
for.obj+
goto.obj+
history.obj+
if.obj+
init.obj+
internal.obj+
loadhigh.obj+
messages.obj+
misc.obj+
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
swapexec.obj+
tempfile.obj+
time.obj+
timefunc.obj+
tmpnam.obj+
truename.obj+
type.obj+
ver.obj+
verify.obj+
where.obj+
cb_catch.obj+
lh.obj+
lowexec.obj+
spawn.obj
command
		# no map file
suppl_s.lib+
cs.lib
|


#		*Individual File Dependencies*
alias.obj: command.cfg alias.c

batch.obj: command.cfg batch.c

beep.obj: command.cfg beep.c

break.obj: command.cfg break.c

call.obj: command.cfg call.c

cls.obj: command.cfg cls.c

cmdinput.obj: command.cfg cmdinput.c

cmdline.obj: command.cfg cmdline.c

cmdtable.obj: command.cfg cmdtable.c

command.obj: command.cfg command.c

copy.obj: command.cfg copy.c

ctty.obj: command.cfg ctty.c

date.obj: command.cfg date.c

datefunc.obj: command.cfg datefunc.c

debug.obj: command.cfg debug.c

del.obj: command.cfg del.c

dir.obj: command.cfg dir.c

echo.obj: command.cfg echo.c

environ.obj: command.cfg environ.c

error.obj: command.cfg error.c

err_hand.obj: command.cfg err_hand.c

exec.obj: command.cfg exec.c

fddebug.obj: command.cfg fddebug.c

filecomp.obj: command.cfg filecomp.c

for.obj: command.cfg for.c

goto.obj: command.cfg goto.c

history.obj: command.cfg history.c

if.obj: command.cfg if.c

init.obj: command.cfg init.c

internal.obj: command.cfg internal.c

loadhigh.obj: command.cfg loadhigh.c

messages.obj: command.cfg messages.c

misc.obj: command.cfg misc.c

nls.obj: command.cfg nls.c

openf.obj: command.cfg openf.c

parsenum.obj: command.cfg parsenum.c

path.obj: command.cfg path.c

pause.obj: command.cfg pause.c

prompt.obj: command.cfg prompt.c

redir.obj: command.cfg redir.c

ren.obj: command.cfg ren.c

session.obj: command.cfg session.c

set.obj: command.cfg set.c

shift.obj: command.cfg shift.c

swapexec.obj: command.cfg swapexec.c

tempfile.obj: command.cfg tempfile.c

time.obj: command.cfg time.c

timefunc.obj: command.cfg timefunc.c

tmpnam.obj: command.cfg tmpnam.c

truename.obj: command.cfg truename.c

type.obj: command.cfg type.c

ver.obj: command.cfg ver.c

verify.obj: command.cfg verify.c

where.obj: command.cfg where.c

cb_catch.obj: command.cfg cb_catch.asm
	$(TASM) /MX /ZI /O CB_CATCH.ASM,CB_CATCH.OBJ

lh.obj: command.cfg lh.asm
	$(TASM) /MX /ZI /O LH.ASM,LH.OBJ

lowexec.obj: command.cfg lowexec.asm
	$(TASM) /MX /ZI /O LOWEXEC.ASM,LOWEXEC.OBJ

spawn.obj: command.cfg spawn.asm
	$(TASM) /MX /ZI /O SPAWN.ASM,SPAWN.OBJ

#		*Compiler Configuration File*
command.cfg: command.mak
  copy &&|
-f-
-ff-
-K
-j100
-O
-Z
-d
-b-
-vi-
-H=COMMAND.SYM
-w-ret
-w-nci
-w-inl
-wpin
-wamb
-wamp
-w-par
-wasm
-wcln
-w-cpt
-wdef
-w-dup
-w-pia
-wsig
-wnod
-w-ill
-w-sus
-wstv
-wucp
-wuse
-w-ext
-w-ias
-w-ibc
-w-pre
-w-nst
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P-.C
| command.cfg
