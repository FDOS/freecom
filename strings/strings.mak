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
	copy &&|
if not exist strings\strings.rsp goto noStringLib
cd strings
echo.
echo Making STRINGS library
echo.
echo. >$(CFG)
$(CC) -c *.c >errlist
if errorlevel 1 goto ende
if exist strings.lib del strings.lib
$(AR) strings.lib /c @strings.rsp, strings.lst
if errorlevel 1 goto ende
copy strings.lib ..
copy strings.lst ..
echo Purging temporary directory of strings library
del strings.*	>NUL
del $(CFG)	>NUL
del makefile	>NUL
del errlist	>NUL
del *.obj	>NUL
del *.c	>NUL
cd ..
rmdir strings
:noStringLib
:ende
| mkSTRLIB.Bat


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
