!ifdef __LINUX__
DIRSEP = /
RMFILES = rm -f
ECHOTO = echo >>
ECHOTODEP =
CP = cp
!endif
LIBLIST = >
ECHOLIB = echo >>
ECHOLIBDEP =

!ifdef __LINUX__
LD = $(CL) -l=dos -fe=command.exe $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(LIBS) -\"op map,statics,verbose,stack=4k\"
!else
LD_RSP = command.rsp
LD = wlinker /ma/nologo @$(LD_RSP)
!endif
INCLUDEPATH = -I$(WATCOM)$(DIRSEP)h
CC = wcc -zq -fo=.obj
CL = wcl -zq -fo=.obj -bcl=dos
CL386 = wcl386 -zq -fo=.obj
AR = wlib -n -c

CFG = watcomc.cfg
CFLAGS1 = -os-s-wx

#		*Implicit Rules*
!ifeq UTILS_BUILD 1
.c.exe:
! ifdef __LINUX__
  $(CL386) -I$(WATCOM)$(DIRSEP)lh $< -fm -fe=$@ -I..$(DIRSEP)suppl
! else ifdef __MSDOS__
!  ifeq COMPACT_MODEL 1
  $(CL) -mc -I$(WATCOM)$(DIRSEP)h $< -fm -fe=$@ -I..$(DIRSEP)suppl
!  else
  $(CL) -ms -I$(WATCOM)$(DIRSEP)h $< -fm -fe=$@ -I..$(DIRSEP)suppl
!  endif
! else
  $(CL386) -I$(WATCOM)$(DIRSEP)h $< -fm -fe=$@ -I..$(DIRSEP)suppl
! endif
!else
.c.obj:
  $(CC) $< -bt=dos @$(CFG)

.obj.exe:
  wlink sys DOS f $< lib $(SUPPL_LIB_PATH)$(DIRSEP)suppl_$(SHELL_MMODEL).lib op q,map

.c.exe:
  $(CL) $< @$(CFG) -\"op map,statics,verbose,stack=4k\"

!endif
