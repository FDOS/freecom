CFG_DEPENDENCIES = makefile.mak

TOP = ..
!include "$(TOP)/config.mak"

all: $(CFG) cmds.lib

OBJS1 = alias.obj beep.obj break.obj call.obj cdd.obj chcp.obj chdir.obj
OBJS2 = cls.obj copy.obj ctty.obj date.obj del.obj dir.obj dirs.obj
OBJS3 = doskey.obj echo.obj exit.obj exit2.obj fddebug.obj for.obj goto.obj
OBJS4 = history.obj if.obj lfnfor.obj memory.obj mkdir.obj path.obj pause.obj
OBJS5 = popd.obj prompt.obj pushd.obj rem.obj ren.obj rmdir.obj set.obj
OBJS6 = shift.obj time.obj truename.obj type.obj verify.obj which.obj

cmds.rsp : $(ECHOLIBDEP) makefile.mak
	$(RMFILES) cmds.rsp
	$(ECHOLIB) cmds.rsp $(OBJS1)
	$(ECHOLIB) cmds.rsp $(OBJS2)
	$(ECHOLIB) cmds.rsp $(OBJS3)
	$(ECHOLIB) cmds.rsp $(OBJS4)
	$(ECHOLIB) cmds.rsp $(OBJS5)
	$(ECHOLIB) cmds.rsp $(OBJS6)

cmds.lib : $(CFG) $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) cmds.rsp
	$(RMFILES) cmds.lib
	$(AR) cmds.lib @cmds.rsp $(LIBLIST) cmds.lst 
