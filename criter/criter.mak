.AUTODEPEND

CFG = TCCDOS.CFG
CFG_DEPENDENCIES = criter.mak

all:  context.def criter criter1

#		*Translator Definitions*
!include "..\config.mak"


context.def : context.x
	..\utils\mkctxt.exe context
	copy context.h_c ..
	copy context.inc ..

criter.asm :  dmy_cbrk.asm ..\include\stuff.inc context.def

criter : criter.asm 
	$(NASM) $(NASMFLAGS) -f bin -o criter criter.asm

criter1 : criter.asm
	$(NASM) $(NASMFLAGS) -DAUTO_FAIL -f bin -o criter1 criter.asm

