@echo off
cd strings

if not exist fixstrs.com if not exist fixstrs.exe goto err1

:doit
fixstrs
copy strings.dat ..
del strings.dat
copy strings.h ..
del strings.h
goto ende

:err1
if "%1"=="tc101" make -f fixstrs.mak
if "%1"=="dmake" dmake
if exist fixstrs.com goto doit
if exist fixstrs.exe goto doit

echo You must make FIXSTRS executable first, e.g. by typing
echo MAKE -f FIXSTRS.MAK   -or-  DMAKE in directory STRINGS

:ende
cd ..
