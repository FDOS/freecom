.AUTODEPEND

CFG = TCCDOS.CFG

#		*Translator Definitions*
##>> Modify this file with your local settings
!include "config.mak"


all: command.com

#		*Implicit Rules*
.c.obj:
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
 dstack.obj \
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
 $(SUPPL_LIB_PATH)\suppl_s.lib

#		*Explicit Rules*
command.exe: $(CFG) $(EXE_dependencies)
  $(TLINK) /x /c /d @&&|
$(LIBPATH)\c0s.obj+
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
dstack.obj+
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
$(SUPPL_LIB_PATH)\suppl_s.lib+
$(LIBPATH)\cs.lib
|

command.com : command.exe
	copy /b command.exe + strings.dat command.com

#		*Individual File Dependencies*
alias.obj: $(CFG) alias.c

batch.obj: $(CFG) batch.c

beep.obj: $(CFG) beep.c

break.obj: $(CFG) break.c

call.obj: $(CFG) call.c

cls.obj: $(CFG) cls.c

cmdinput.obj: $(CFG) cmdinput.c

cmdline.obj: $(CFG) cmdline.c

cmdtable.obj: $(CFG) cmdtable.c

command.obj: $(CFG) command.c

copy.obj: $(CFG) copy.c

ctty.obj: $(CFG) ctty.c

date.obj: $(CFG) date.c

datefunc.obj: $(CFG) datefunc.c

debug.obj: $(CFG) debug.c

del.obj: $(CFG) del.c

dir.obj: $(CFG) dir.c

dstack.obj: $(CFG) dstack.c

echo.obj: $(CFG) echo.c

environ.obj: $(CFG) environ.c

err_hand.obj: $(CFG) err_hand.c

error.obj: $(CFG) error.c

exec.obj: $(CFG) exec.c

fddebug.obj: $(CFG) fddebug.c

filecomp.obj: $(CFG) filecomp.c

for.obj: $(CFG) for.c

goto.obj: $(CFG) goto.c

history.obj: $(CFG) history.c

if.obj: $(CFG) if.c

init.obj: $(CFG) init.c

internal.obj: $(CFG) internal.c

loadhigh.obj: $(CFG) loadhigh.c

messages.obj: $(CFG) messages.c

misc.obj: $(CFG) misc.c

nls.obj: $(CFG) nls.c

openf.obj: $(CFG) openf.c

parsenum.obj: $(CFG) parsenum.c

path.obj: $(CFG) path.c

pause.obj: $(CFG) pause.c

prompt.obj: $(CFG) prompt.c

redir.obj: $(CFG) redir.c

ren.obj: $(CFG) ren.c

session.obj: $(CFG) session.c

set.obj: $(CFG) set.c

shift.obj: $(CFG) shift.c

swapexec.obj: $(CFG) swapexec.c

tempfile.obj: $(CFG) tempfile.c

time.obj: $(CFG) time.c

timefunc.obj: $(CFG) timefunc.c

tmpnam.obj: $(CFG) tmpnam.c

truename.obj: $(CFG) truename.c

type.obj: $(CFG) type.c

ver.obj: $(CFG) ver.c

verify.obj: $(CFG) verify.c

where.obj: $(CFG) where.c

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
