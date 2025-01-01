CFG_DEPENDENCIES = makefile.mak

UTILS_BUILD = 1

TOP = ..
!include "$(TOP)/config.mak"

all: $(CFG) mktools.exe mkctxt.exe chunk.exe mkinfres.exe ptchsize.exe

mktools.exe : mktools.c ../config.h
ptchsize.exe : ptchsize.c ../tools/ptchsize.c