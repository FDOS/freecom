.AUTODEPEND

CC_BASE_PATH = $(BC5_BASE)
BINPATH = $(CC_BASE_PATH)\BIN	# Turbo/Borland C++
LIBPATH = $(CC_BASE_PATH)\LIB
INCLUDEPATH = -I$(CC_BASE_PATH)\INCLUDE
CC = $(BINPATH)\BCC		# Borland C++
CL = $(CC)
AR = $(BINPATH)\Tlib /C
LD = $(BINPATH)\Tlink /m/s/l /c/d /i
LIBLIST = ,
ECHOLIB = echolib

OBJC0 = $(LIBPATH)\c0$(SHELL_MMODEL).obj
LIBC = $(LIBPATH)\c$(SHELL_MMODEL).lib

CFG = TURBOC.CFG
CFLAGS1 = -1- -a -f- -ff- -K -w+ -O -Z -b- -v -L$(LIBPATH)

#		*Implicit Rules*
.c.exe:
  $(CC) $< $(SUPPL_LIB_PATH)\SUPPL_$(SHELL_MMODEL).LIB
.c.obj:
  $(CC) -c {$< }
