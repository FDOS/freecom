.AUTODEPEND

#		*Translator Definitions*
CC_BASE_PATH = D:\TC101
CC = tcc +FIXSTRS.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = $(CC_BASE_PATH)\LIB
INCLUDEPATH = $(CC_BASE_PATH)\INCLUDE
MMODEL = c


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 fixstrs.obj

#		*Explicit Rules*
fixstrs.exe: fixstrs.cfg $(EXE_dependencies)
  $(TLINK) /s/c/d/L$(LIBPATH) @&&|
c0$(MMODEL).obj+
fixstrs.obj
fixstrs,fixstrs
c$(MMODEL).lib
|


#		*Individual File Dependencies*
fixstrs.obj: fixstrs.cfg fixstrs.c

#		*Compiler Configuration File*
fixstrs.cfg: fixstrs.mak
  copy &&|
-m$(MMODEL)
-a
-f-
-ff-
-K
-j100
-G
-O
-Z
-k-
-d
-vi-
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P-.C
| fixstrs.cfg


#-H=FIXSTRS.SYM
