@echo off
cd strings

if not exist fixstrs.com if not exist fixstrs.exe goto err1

fixstrs
copy strings.dat ..
del strings.dat
copy strings.h ..
del strings.h

:err1
echo You must make FIXSTRS executable first, e.g. by typing
echo MAKE -f FIXSTRS.MAK   -or-  DMAKE in directory STRINGS

:ende
cd ..
