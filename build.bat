@echo off
if exist lastmake.mk del lastmake.mk
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
call fixstrs.bat tc101
echo.
echo Making CRITER resource
echo.
cd criter.mod
make -fcriter.mak all
if errorlevel 1 goto ende
cd ..
echo.
echo Making COMMAND.COM
echo.
make -fcommand.mak all
if errorlevel 1 goto ende
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
:ende
