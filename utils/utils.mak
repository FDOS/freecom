CFG_DEPENDENCIES = utils.mak

!include "..\config.mak"

all: $(CFG) mktools.exe mkctxt.exe chunk.exe mkinfres.exe

mktools.c : ..\config.h
