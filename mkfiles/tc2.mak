CC_BASE_PATH = $(TC2_BASE)
BINPATH = $(CC_BASE_PATH)	# Turbo C
LIBPATH = $(CC_BASE_PATH)\LIB
INCLUDEPATH = -I$(CC_BASE_PATH)\INCLUDE
CC = $(BINPATH)\TCC		# Turbo C/C++
CL = $(CC)
AR = $(BINPATH)\Tlib /C
LD_RSP = command.rsp
LD = $(BINPATH)\Tlink /m/s/l /c/d @$(LD_RSP)

OBJC0 = $(LIBPATH)\c0$(SHELL_MMODEL).obj
LIBC = $(LIBPATH)\c$(SHELL_MMODEL).lib

CFG = TURBOC.CFG
CFLAGS1 = -1- -a- -f- -ff- -K -w+ -O -Z
CFLAGS2 = -L$(LIBPATH)

#		*Implicit Rules*
.c.exe:
  $(CC) $< $(SUPPL_LIB_PATH)\SUPPL_$(SHELL_MMODEL).LIB
.c.obj:
  $(CC) -c $<
