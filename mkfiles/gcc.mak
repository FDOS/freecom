# Linux only for now
DIRSEP = /
ECHOTO = echo >>
ECHOTODEP =
LIBLIST = >
ECHOLIB = echo >>
ECHOLIBDEP =
RMFILES = rm -f
CP = cp
LIBC = -li86
NASMFLAGS := $(NASMFLAGS) -felf
SHELL_MMODEL_COMP=cmodel=small
COMPACT_MMODEL=
INCLUDEPATH=-I. -I../compat -I../suppl/compat

CC = ia16-elf-gcc -c
CL = ia16-elf-gcc -mcmodel=small
CLO = -o $@
AR = ia16-elf-ar crsv
LD = $(CL) $(CFLAGS1) -o command.exe $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) command.ld $(LIBS) $(LIBC) -Wl,-Map,command.map

CFG = gcc.cfg
CFLAGS1 = -Os -Wall -Werror -Wno-pointer-to-int-cast -Wno-incompatible-pointer-types -mregparmcall -fno-builtin -fno-strict-aliasing -Wno-format -mfar-function-if-far-return-type -fno-caller-saves -fno-optimize-sibling-calls -fno-move-loop-invariants -fno-tree-loop-optimize -fno-rerun-cse-after-loop

#		*Implicit Rules*
.SUFFIXES:
.SUFFIXES: .c .asm .com .exe .obj
.c.exe:
	gcc -x c -Og -g -Wall -DGCC -D__GETOPT_H -I../suppl $< -o $@
.c.obj:
	$(CC) $< @$(CFG) -o $@
