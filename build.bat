@echo off
if exist lastmake.mk call clean.bat
if "%1"=="-r" call clean.bat
if "%1"=="-r" shift
if "%1"=="clean" clean.bat
if "%1"=="clean" goto ende
set SWAP=
if "%1"=="xms-swap" set SWAP=-DXMS_SWAP
if "%1"=="xms-swap" shift
if not "%1"=="-h" goto run

echo Build FreeCOM
echo Useage: %0 [-r] [clean] [xms-swap]
echo -r: Rebuilt -- Clean before proceed
echo clean: Remove *.OBJ, *.COM, *.LIB, etc. files, then exit
echo xms-swap: Build FreeCOM with XMS-Only Swap support
goto ende

:run
if not x%1==x set LNG=%1
if "%lng%"=="" set LNG=english

echo.
echo Making basic utilities for build process
echo.
cd utils
make %SWAP% -futils.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making STRINGS resource
echo.
cd strings
if exist mkSTRLIB.Bat del mkSTRLIB.Bat >NUL
make -fstrings.mak -DLNG=%LNG% all
if errorlevel 1 goto ende
if exist mkSTRLIB.Bat call mkSTRLIB.Bat
if errorlevel 1 goto ende
if exist mkSTRLIB.Bat del mkSTRLIB.Bat >NUL
cd ..

echo.
echo Making CRITER resource
echo.
cd criter
make %SWAP% -fcriter.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making misc library
echo.
cd lib
make %SWAP% -flib.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making commands library
echo.
cd cmd
make %SWAP% -fcmd.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making COMMAND.COM
echo.
cd shell
if "%SWAP%"=="" make %SWAP% -fcommand.mak all
if not "%SWAP%"=="" make %SWAP% -fxms-swap.mak all
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
if "%SWAP%"=="" echo Note: To build the XMS-Only Swap featured FreeCOM, re-run
if "%SWAP%"=="" echo %0 -r xms-swap

:ende
set SWAP=
