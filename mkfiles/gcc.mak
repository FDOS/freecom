# Linux only for now
DIRSEP = /
RMFILES = rm -f
RMFILES2 = rm -f
ECHOTO = echo >>
ECHOTO0 = echo >>
CP = cp
NASMFLAGS := $(NASMFLAGS) -felf
SHELL_MMODEL_COMP=cmodel=small
INCLUDEPATH=-I. -I../compat -I../suppl/compat

CC = ia16-elf-gcc -c
CL = ia16-elf-gcc -mcmodel=small -o $@
AR = ar crsv
LD = $(CL) $(CFLAGS1) -o command.exe $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(LIBS) -Wl,-Map,command.map \#
LIBLIST = >
ECHOLIB = echo >>

CFG = gcc.cfg
CFLAGS1 = -Os -Wall -Werror -Wno-pointer-to-int-cast -Wno-incompatible-pointer-types -mrtd -fno-builtin-printf -fno-strict-aliasing -Wno-format

#		*Implicit Rules*
.SUFFIXES:
.SUFFIXES: .C .c .asm .com .exe .obj
.c.exe:
	gcc -x c -DGCC -D__GETOPT_H -I../suppl $< -o $@
.C.exe:
	gcc -x c -DGCC -D__GETOPT_H -I../suppl $< -o $@
.c.obj:
	$(CC) $< @$(CFG) -o $@
