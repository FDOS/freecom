CFG_DEPENDENCIES = makefile.mak

TOP=..
!include "$(TOP)/config.mak"

all: $(CFG) cmds.lib

OBJS1 = alias.obj beep.obj break.obj call.obj cdd.obj chcp.obj chdir.obj
LOBJS1 = $(LIBPLUS)alias.obj $(LIBPLUS)beep.obj $(LIBPLUS)break.obj $(LIBPLUS)call.obj $(LIBPLUS)cdd.obj $(LIBPLUS)chcp.obj $(LIBPLUS)chdir.obj
OBJS2 = cls.obj copy.obj ctty.obj date.obj del.obj dir.obj dirs.obj
LOBJS2 = $(LIBPLUS)cls.obj $(LIBPLUS)copy.obj $(LIBPLUS)ctty.obj $(LIBPLUS)date.obj $(LIBPLUS)del.obj $(LIBPLUS)dir.obj $(LIBPLUS)dirs.obj
OBJS3 = doskey.obj echo.obj exit.obj exit2.obj fddebug.obj for.obj goto.obj
LOBJS3 = $(LIBPLUS)doskey.obj $(LIBPLUS)echo.obj $(LIBPLUS)exit.obj $(LIBPLUS)exit2.obj $(LIBPLUS)fddebug.obj $(LIBPLUS)for.obj $(LIBPLUS)goto.obj
OBJS4 = history.obj if.obj lfnfor.obj memory.obj mkdir.obj path.obj pause.obj
LOBJS4 = $(LIBPLUS)history.obj $(LIBPLUS)if.obj $(LIBPLUS)lfnfor.obj $(LIBPLUS)memory.obj $(LIBPLUS)mkdir.obj $(LIBPLUS)path.obj $(LIBPLUS)pause.obj
OBJS5 = popd.obj prompt.obj pushd.obj rem.obj ren.obj rmdir.obj set.obj
LOBJS5 = $(LIBPLUS)popd.obj $(LIBPLUS)prompt.obj $(LIBPLUS)pushd.obj $(LIBPLUS)rem.obj $(LIBPLUS)ren.obj $(LIBPLUS)rmdir.obj $(LIBPLUS)set.obj
OBJS6 = shift.obj time.obj truename.obj type.obj verify.obj which.obj
LOBJS6 = $(LIBPLUS)shift.obj $(LIBPLUS)time.obj $(LIBPLUS)truename.obj $(LIBPLUS)type.obj $(LIBPLUS)verify.obj $(LIBPLUS)which.obj

echolib.bat : ../scripts/echolib.bat
	$(CP) ..$(DIRSEP)scripts$(DIRSEP)echolib.bat .

cmds.rsp : echolib.bat makefile.mak
	$(RMFILES) cmds.rsp
	$(ECHOLIB) cmds.rsp $(LOBJS1) $(LIBCONT)
	$(ECHOLIB) cmds.rsp $(LOBJS2) $(LIBCONT)
	$(ECHOLIB) cmds.rsp $(LOBJS3) $(LIBCONT)
	$(ECHOLIB) cmds.rsp $(LOBJS4) $(LIBCONT)
	$(ECHOLIB) cmds.rsp $(LOBJS5) $(LIBCONT)
	$(ECHOLIB) cmds.rsp $(LOBJS6)

cmds.lib : $(CFG) $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) cmds.rsp
	$(RMFILES) cmds.lib
	$(AR) cmds.lib @cmds.rsp $(LIBLIST) cmds.lst 
