CFG_DEPENDENCIES = cmd.mak

TOP=..
!include "$(TOP)/config.mak"

all: $(CFG) cmds.lib

OBJ1 =	alias.obj \
	beep.obj \
	break.obj \
	call.obj \
	cdd.obj \
	chcp.obj \
	chdir.obj \
	cls.obj \
	copy.obj
OBJ2 =	ctty.obj \
	date.obj \
	del.obj \
	dir.obj \
	dirs.obj \
	doskey.obj \
	echo.obj \
	exit.obj
OBJ3 =	exit2.obj \
	fddebug.obj \
	for.obj \
	goto.obj \
	history.obj \
	if.obj \
	lfnfor.obj \
	memory.obj
OBJ4 =	mkdir.obj \
	path.obj \
	pause.obj \
	popd.obj \
	prompt.obj \
	pushd.obj \
	rem.obj \
	ren.obj
OBJ5 =	rmdir.obj \
	set.obj \
	shift.obj \
	time.obj \
	truename.obj \
	type.obj \
	verify.obj \
	which.obj 

echolib.bat : ..\scripts\echolib.bat
	copy ..\scripts\echolib.bat

cmds.rsp : echolib.bat cmd.mak
	..\scripts\rmfiles cmds.rsp
	$(ECHOLIB) cmds.rsp $(OBJ1)
	$(ECHOLIB) cmds.rsp $(OBJ2)
	$(ECHOLIB) cmds.rsp $(OBJ3)
	$(ECHOLIB) cmds.rsp $(OBJ4)
	$(ECHOLIB) cmds.rsp $(OBJ5)

cmds.lib : $(CFG) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) cmds.rsp
	..\scripts\rmfiles cmds.lib
	$(AR) cmds.lib /c @cmds.rsp $(LIBLIST) cmds.lst 
