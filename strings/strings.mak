.AUTODEPEND

#		*Translator Definitions*
CFG = TCCDOS.CFG
CFG_DEPENDENCIES = strings.mak

all : strings.h strings.err

## Configuration parameters
!include "..\config.mak"

MMODEL = c

strings.h : default.lng fixstrs.exe
	fixstrs.exe /lib $(LNG)
	copy strings.h ..

strings.err : default.err critstrs.exe
	critstrs.exe $(LNG)

#		*Implicit Rules*
.c.obj:
  $(CC) -c -m$(MMODEL) {$< }

fixstrs.exe: $(CFG) fixstrs.c
	$(CC) -efixstrs.exe -m$(MMODEL) fixstrs.c

critstrs.exe: $(CFG) critstrs.c
	$(CC) -ecritstrs.exe -m$(MMODEL) critstrs.c

#		*Individual File Dependencies*
fixstrs.obj: $(CFG) fixstrs.c

critstrs.obj: $(CFG) critstrs.c
