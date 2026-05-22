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
SHELL_MMODEL_COMP = cmodel=small
INCLUDEPATH = -I. -I../compat -I../suppl/compat
COMPACT_MMODEL = -mcmodel=compact

CC = ia16-elf-gcc
CL = ia16-elf-gcc -mcmodel=small
CLO = -o $@
AR = ia16-elf-ar crsv
LD = $(CL) $(CFLAGS1) -o command.exe $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) command.ld $(LIBS) $(LIBC) -Wl,-Map,command.map

CFG = gcc.cfg
CFLAGS1 = -Os -Wall -Werror -Wno-pointer-to-int-cast -Wno-incompatible-pointer-types -mregparmcall -fno-builtin -fno-strict-aliasing -Wno-format -mfar-function-if-far-return-type -fno-caller-saves -fno-optimize-sibling-calls -fno-move-loop-invariants -fno-tree-loop-optimize -fno-rerun-cse-after-loop

#		*Implicit Rules*
.SUFFIXES:
.SUFFIXES: .c .asm .com .exe .obj
ifeq ($(UTILS_BUILD),1)
.c.exe:
	gcc -x c -Og -g -Wall -DTARGET_USE_GCC_IA16 -D__GETOPT_H -I../suppl $< -o $@
else ifeq ($(COMPACT_MODEL),1)
.c.obj .c.exe:
	@echo ------------------------------------------------------
	@echo "$@ (DOS version) is not build because"
	@echo ia16-elf-gcc does not support the compact memory model
	@echo ------------------------------------------------------

.obj.exe:

else
.c.obj:
	$(CC) -c $< @$(CFG) -o $@

.c.exe .obj.exe:
	$(CL) $< @$(CFG) $(LIBS) $(LIBC) -o $@

endif
