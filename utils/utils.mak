.AUTODEPEND

CFG = TCCDOS.CFG

#		*Translator Definitions*
##>> Modify this file with your local settings
!include "..\config.mak"


all: $(CFG) mktools.exe mkctxt.exe mkinfres.exe

#		*Implicit Rules*
.c.exe:
  $(CC) $< 
.c.obj:
  $(CC) -c {$< }
.asm.obj:
	$(NASM) $(NASMFLAGS) -f obj $<

mktools.c : ..\config.h

#		*Compiler Configuration File*
#-h
#-Vf
#-Ff
#-C
#-L$(LIBPATH)

# added strings.h here because $(CFG) is included everywhere already
$(CFG): utils.mak
  copy &&|
-a
-f-
-ff-
-K
-w+
-O
-Z
-k-
-d
-b-
-I$(INCLUDEPATH)
-L$(LIBPATH)
-D_NO__DOS_DATE
-D_NO__DOS_TIME
| $(CFG)
