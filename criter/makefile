# $Id$
#
# Makefile for the FreeCOM CRITER module
#
# $Log$
# Revision 1.2  2002/04/02 23:36:37  skaus
# add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
#
# Revision 1.1  2001/04/23 21:35:41  skaus
# Beta 7 changes (code split)
#
# Revision 1.1.2.2  2001/02/27 18:27:06  skaus
# fix: criter.asm: DEBUG does no longer compile to .COM (for consistency)
# chg: removed all NULLs to prevent NULL problematic, functions must be
# 	used fully prototyped now
#
# Revision 1.1.2.1  2001/02/18 18:09:55  skaus
# intermediate update #3
#

.IMPORT : FREEDOS

INCDIR +=;$(FREEDOS)\SRC\INCLUDE
LIBDIR +=;$(FREEDOS)\SRC\LIB\$(_COMPILER)
LDLIBS = suppl_$(_MODEL).lib
LDFLAGS += /msl
NASM *= c:\TOOL\NASMW.EXE

##LD_TLINK != D:\BC5\BIN\TLINK.EXE

# Project specific C compiler flags
MYCFLAGS_DBG = -DNDEBUG=1 $(null,$(DEBUG) $(NULL) -DDEBUG=1)
MYCFLAGS_NDBG =
MYCFLAGS = $(null,$(NDEBUG) $(MYCFLAGS_DBG) $(MYCFLAGS_NDBG))

# Project specific NASM compiler flags
NASMFLAGS_DBG = $(null,$(DEBUG) $(NULL) -DDEBUG=1) -s
NASMFLAGS_NDBG = -s
NASMFLAGS = $(null,$(NDEBUG) $(NASMFLAGS_DBG) $(NASMFLAGS_NDBG))

# Sources of this make target
SRC = 

#	Default target
all: criter criter1 context.def


dist : ;

.INIT .PHONY .SEQUENTIAL : $(CFG) __errl


__errl:
	@+-if exist errlist del errlist >nul
#	-ctags *.c

criter.asm : context.def dmy_cbrk.asm ../include/stuff.inc resource.inc ../strings/strings.err

criter : criter.asm
	$(NASM) $(NASMFLAGS) -f bin -o $@ $< >>errlist

criter1 : criter.asm
	$(NASM) $(NASMFLAGS) -DAUTO_FAIL -f bin -o $@ $< >>errlist

context.def context.h_c .UPDATEALL : context.x
	..\utils\mkctxt.exe $<
	+copy context.h_c ..
	+copy context.inc ..

#MAKEDEP START
#MAKEDEP STOP

clobber : clean
	$(RM) $(RMFLAGS) *.com *.cln criter criter1 *.cfg

clean : 
	$(RM) $(RMFLAGS) *.obj *.bak context.h_c context.def context.inc ..\context.h_c ..\context.inc tags errlist strings.inc

