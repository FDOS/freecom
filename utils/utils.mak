.AUTODEPEND

CFG = TCCDOS.CFG

#		*Translator Definitions*
##>> Modify this file with your local settings
!include "..\config.mak"


all: $(CFG) mktools.exe mkctxt.exe chunk.exe mkinfres.exe

mktools.c : ..\config.h
