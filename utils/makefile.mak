CFG_DEPENDENCIES = makefile.mak

TOP=..
!include "$(TOP)/config.mak"

all: $(CFG) mktools.exe mkctxt.exe chunk.exe mkinfres.exe

mktools.exe : ../config.h
