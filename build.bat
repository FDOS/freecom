@echo off

set MKOPT=YES-DXMS-SWAP____________________
if NOT "%MKOPT%"=="YES-DXMS-SWAP____________________" goto err1
: BEGIN Internal stuff for ska -- If one of these three commands
:       fail for you, your distribution is broken! Please report.
for %%a in (lib\lib.mak cmd\cmd.mak shell\command.mak) do if not exist %%a set MKOPT=NO
if "%MKOPT%"=="NO" set XMS_SWAP=
if "%MKOPT%"=="NO" call dmake dist
: END
set MKOPT=

if exist lastmake.mk call clean.bat
if "%1"=="-r" call clean.bat
if "%1"=="-r" shift
if "%1"=="clean" clean.bat
if "%1"=="clean" goto ende
if "%1"=="xms-swap" set MKOPT=-DXMS_SWAP
if "%1"=="xms-swap" shift
if "%1"=="debug" set MKOPT=%MKOPT% -DDEBUG
if "%1"=="debug" shift
if not "%1"=="-h" goto run

echo Build FreeCOM
echo Useage: %0 [-r] [clean] [xms-swap] [debug] [language]
echo -r: Rebuilt -- Clean before proceed
echo clean: Remove *.OBJ, *.COM, *.LIB, etc. files, then exit
echo xms-swap: Build FreeCOM with XMS-Only Swap support
echo debug: Build FreeCOM with debug settings.
echo You can select for which language to built FreeCOM by setting
echo the environment variable LNG before running this script, e.g.:
echo SET LNG=german
echo selects the German language. For available language see STRINGS\*.LNG
goto ende

:run
if not x%1==x set LNG=%1
if "%lng%"=="" set LNG=english
echo Building FreeCOM for lanaguage %LNG%

if not exist config.mak goto err2

echo.
echo Checking SUPPL library
cd suppl
if exist skip goto endSuppl
echo Building SUPPL library
if exist compile.me del compile.me >NUL
make -fsuppl.mak %MKOPT% all
if errorlevel 1 goto ende
cd src
make -fsuppl.mak %MKOPT% all
if errorlevel 1 goto ende
if exist compile.me call do_suppl.bat
if errorlevel 1 goto ende
if not exist all_done goto ende
if exist compile.bat del compile.bat >NUL
if exist linkme.bat del linkme.bat >NUL
if exist suppl.bat del suppl.bat >NUL
:endSuppl
cd ..

echo.
echo Making basic utilities for build process
echo.
cd utils
make %MKOPT% -futils.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making STRINGS resource
echo.
cd strings
if exist mkSTRLIB.Bat del mkSTRLIB.Bat >NUL
make -fstrings.mak %MKOPT% -DLNG=%LNG% all
if errorlevel 1 goto ende
if exist mkSTRLIB.Bat call mkSTRLIB.Bat
if errorlevel 1 goto ende
if exist mkSTRLIB.Bat del mkSTRLIB.Bat >NUL
cd ..

echo.
echo Making CRITER resource
echo.
cd criter
make %MKOPT% -fcriter.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making misc library
echo.
cd lib
make %MKOPT% -flib.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making commands library
echo.
cd cmd
make %MKOPT% -fcmd.mak all
if errorlevel 1 goto ende
cd ..

echo.
echo Making COMMAND.COM
echo.
cd shell
make %MKOPT% -fcommand.mak all
if errorlevel 1 goto ende
cd ..

utils\mkinfres.exe /tinfo.txt infores shell\command.map shell\command.exe
copy /b shell\command.exe + infores + criter\criter1 + criter\criter + strings\strings.dat command.com
if not exist command.com goto ende

echo.
echo Making supplemental tools
echo.
cd tools
type tools.m1 >tools.mak
..\utils\mktools.exe >>tools.mak
type tools.m2 >>tools.mak
make -ftools.mak %MKOPT% all
if errorlevel 1 goto ende
cd ..

echo.
echo Patching heap size to 6KB
echo.
tools\ptchsize.exe command.com +6KB

echo.
echo All done. COMMAND.COM is ready for useage!
echo.
if NOT "%MKOPT%"=="" goto ende

echo Note: To build the XMS-Only Swap featured FreeCOM, re-run
echo BUILD.BAT -r xms-swap %LNG%
goto ende

:err2
echo Please copy CONFIG.STD into CONFIG.MAK and update the
echo settings therein, especially CC_BASE_PATH and NASM
goto ende

:err1
echo Environment full (cannot add environment variables)
echo Cannot proceed

:ende
set MKOPT=
