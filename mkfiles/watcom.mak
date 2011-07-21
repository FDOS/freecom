CC_BASE_PATH = $(WATCOM)
BINPATH = $(CC_BASE_PATH)\BINW
LIBPATH = $(CC_BASE_PATH)\LIB
INCLUDEPATH = -I$(CC_BASE_PATH)\H
CC = $(BINPATH)\wcc -zq		# Turbo C/C++
CL = $(BINPATH)\wcl -zq
AR = $(BINPATH)\wlib /n
LD = wlinker /ma/nologo
LIBLIST = /l=
ECHOLIB = echo >>

CFG = watcomc.cfg
CFLAGS1 = -os-s-wx-bt=DOS

#		*Implicit Rules*
.obj.exe:
  $(BINPATH)\wlink sys DOS f $< lib $(SUPPL_LIB_PATH)\SUPPL_$(SHELL_MMODEL).LIB op q
.c.obj:
  $(CC) $< @$(CFG)
