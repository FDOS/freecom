@echo off
if exist lastmake.mk del lastmake.mk
if exist command.com del command.com
if exist *.cfg del *.cfg
if exist strings\*.cfg del strings\*.cfg
if exist tools\*.cfg del tools\*.cfg
if exist utils\*.cfg del utils\*.cfg

if not x%1==x set LNG=%1
if "%lng%"=="" set LNG=english

echo.
echo Making basic utilities for build process
echo.
cd utils
make -futils.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making STRINGS resource
echo.
cd strings
make -fstrings.mak -DLNG=%LNG% all
if errorlevel 1 goto ende
if not exist strings\strings.rsp goto noStringLib
cd strings
echo.
echo Making STRINGS library
echo.
tcc -c *.c >errlist
if errorlevel 1 goto ende
if exist strings.lib del strings.lib
tlib strings.lib /c @strings.rsp, strings.lst
if errorlevel 1 goto ende
copy strings.lib ..
copy strings.lst ..
echo Purging temporary directory of strings library
del strings.*	>NUL
del makefile	>NUL
del errlist	>NUL
del *.obj	>NUL
del *.c	>NUL
cd ..
rmdir strings
:noStringLib
cd ..

echo.
echo Making CRITER resource
echo.
cd criter
make -fcriter.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making misc library
echo.
cd lib
make -flib.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making commands library
echo.
cd cmd
make -fcmd.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making COMMAND.COM
echo.
cd shell
make -fcommand.mak all
if errorlevel 1 goto ende
cd ..

utils\mkinfres.exe infores shell\command.map shell\command.exe
copy /b shell\command.exe + infores + criter\criter1 + criter\criter + strings\strings.dat command.com
if not exist command.com goto ende

echo.
echo Making supplemental tools
echo.
cd tools
type tools.m1 >tools.mak
..\utils\mktools.exe >>tools.mak
type tools.m2 >>tools.mak
make -ftools.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Patching heap size to 6KB
echo.
tools\ptchsize.exe command.com +6KB

echo.
echo All done. COMMAND.COM is ready for useage!
echo.

:ende
