CFG_DEPENDENCIES = makefile.mak

TOP = ..
!include "$(TOP)/config.mak"

all:  context.def criter criter1

context.def : context.x
	..$(DIRSEP)utils$(DIRSEP)mkctxt.exe context
	$(CP) context.h_c ..
	$(CP) context.inc ..

criter : criter.asm dmy_cbrk.asm ../include/stuff.inc context.def
	$(NASM) $(NASMFLAGS) -f bin -o criter criter.asm

criter1 : criter.asm dmy_cbrk.asm ../include/stuff.inc context.def
	$(NASM) $(NASMFLAGS) -DAUTO_FAIL -f bin -o criter1 criter.asm

