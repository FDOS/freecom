.AUTODEPEND

CFG = TCCDOS.CFG

#		*Translator Definitions*
##>> Modify this file with your local settings
!include "..\config.mak"


all: $(CFG) mktools.exe mkctxt.exe chunk.exe mkinfres.exe

#		*Implicit Rules*
.c.exe:
  $(CC) $< 
.c.obj:
  $(CC) -c {$< }
.asm.obj:
	$(NASM) $(NASMFLAGS) -f obj $<

mktools.c : ..\config.h
