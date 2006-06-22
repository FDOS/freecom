.AUTODEPEND

CFG = TCCDOS.CFG
CFG_DEPENDENCIES = cmd.mak

all: $(CFG) cmds.lib

##>> Modify this file with your local settings
!include "..\config.mak"


cmds.lib : $(CFG) alias.obj \
	beep.obj \
	break.obj \
	call.obj \
	cdd.obj \
	chcp.obj \
	chdir.obj \
	cls.obj \
	copy.obj \
	ctty.obj \
	date.obj \
	del.obj \
	dir.obj \
	dirs.obj \
	doskey.obj \
	echo.obj \
	exit.obj \
	exit2.obj \
	fddebug.obj \
	for.obj \
	goto.obj \
	history.obj \
	if.obj \
	lfnfor.obj \
	memory.obj \
	mkdir.obj \
	path.obj \
	pause.obj \
	popd.obj \
	prompt.obj \
	pushd.obj \
	rem.obj \
	ren.obj \
	rmdir.obj \
	set.obj \
	shift.obj \
	time.obj \
	truename.obj \
	type.obj \
	verify.obj \
	which.obj 
	if exist cmds.lib $(AR) cmds.lib /c @&&|
+-alias.obj &
+-beep.obj &
+-break.obj &
+-call.obj &
+-cdd.obj &
+-chcp.obj &
+-chdir.obj &
+-cls.obj &
+-copy.obj &
+-ctty.obj &
+-date.obj &
+-del.obj &
+-dir.obj &
+-dirs.obj &
+-doskey.obj &
+-echo.obj &
+-exit.obj &
+-exit2.obj &
+-fddebug.obj &
+-for.obj &
+-goto.obj &
+-history.obj &
+-if.obj &
+-lfnfor.obj &
+-memory.obj &
+-mkdir.obj &
+-path.obj &
+-pause.obj &
+-popd.obj &
+-prompt.obj &
+-pushd.obj &
+-rem.obj &
+-ren.obj &
+-rmdir.obj &
+-set.obj &
+-shift.obj &
+-time.obj &
+-truename.obj &
+-type.obj &
+-verify.obj &
+-which.obj
| , cmds.lst 
	if not exist cmds.lib $(AR) cmds.lib /c @&&|
+alias.obj &
+beep.obj &
+break.obj &
+call.obj &
+cdd.obj &
+chcp.obj &
+chdir.obj &
+cls.obj &
+copy.obj &
+ctty.obj &
+date.obj &
+del.obj &
+dir.obj &
+dirs.obj &
+doskey.obj &
+echo.obj &
+exit.obj &
+exit2.obj &
+fddebug.obj &
+for.obj &
+goto.obj &
+history.obj &
+if.obj &
+lfnfor.obj &
+memory.obj &
+mkdir.obj &
+path.obj &
+pause.obj &
+popd.obj &
+prompt.obj &
+pushd.obj &
+rem.obj &
+ren.obj &
+rmdir.obj &
+set.obj &
+shift.obj &
+time.obj &
+truename.obj &
+type.obj &
+verify.obj &
+which.obj
| , cmds.lst 

