.AUTODEPEND

CFG = TCCDOS.CFG
CFG_DEPENDENCIES = utils.mak

all: $(CFG) mktools.exe mkctxt.exe chunk.exe mkinfres.exe

##>> Modify this file with your local settings
!include "..\config.mak"


mktools.c : ..\config.h
