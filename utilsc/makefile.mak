CFG_DEPENDENCIES = makefile.mak

UTILS_BUILD = 1

COMPACT_MODEL = 1

TOP=..
!include "$(TOP)/config.mak"

all : $(CFG) fixstrs.exe critstrs.exe

fixstrs.exe: $(CFG) fixstrs.c ../strings/fixstrs.c

critstrs.exe: $(CFG) critstrs.c
