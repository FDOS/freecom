.AUTODEPEND

#		*Translator Definitions*
CC = tcc +FIXSTRS.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = C:\TC\LIB
INCLUDEPATH = C:\TC\INCLUDE


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
c0t.obj+
fixstrs.obj
fixstrs,fixstrs
cs.lib
|


#		*Individual File Dependencies*
fixstrs.obj: fixstrs.cfg fixstrs.c

#		*Compiler Configuration File*
fixstrs.cfg: fixstrs.mak
  copy &&|
-mt
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
-H=FIXSTRS.SYM
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P-.C
| fixstrs.cfg


