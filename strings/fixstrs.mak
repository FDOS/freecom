.AUTODEPEND

#		*Translator Definitions*
CFG = TCCDOS.CFG

## Configuration parameters
!include "..\config.mak"

MMODEL = c

#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 fixstrs.obj

#		*Explicit Rules*
fixstrs.exe: $(CFG) $(EXE_dependencies)
  $(TLINK) /s/c/d/L$(LIBPATH) @&&|
c0$(MMODEL).obj+
fixstrs.obj
fixstrs,fixstrs
c$(MMODEL).lib
|


#		*Individual File Dependencies*
fixstrs.obj: $(CFG) fixstrs.c

#		*Compiler Configuration File*
$(CFG): fixstrs.mak
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
-P-.C
| $(CFG)


#-H=FIXSTRS.SYM
