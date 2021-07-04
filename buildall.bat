@ECHO OFF
if "%COMPILER%"=="" set COMPILER=wc
if NOT "%1"=="" goto %1

rd /s /q build
call clean
mkdir build
mkdir build\localize
copy VERSION.TXT build\localize\
copy command.lsm build\localize\
copy docs\localize.txt build\localize\

REM make FreeDOS package structure
md build\package
SET pkgdir=build\package
md %pkgdir%\APPINFO
md %pkgdir%\BIN
md %pkgdir%\DOC
md %pkgdir%\DOC\COMMAND
md %pkgdir%\HELP
md %pkgdir%\NLS
md %pkgdir%\SOURCE
md %pkgdir%\SOURCE\FREECOM

pushd .
set languages=NL Dutch ES Spanish DE German FI Finnish FR French IT Italian PO Polish PTR PT_BR RU Russian SE Swedish Ser Serbian SI Slovene TR Turkish UKR Ukr YUG YU437 EN English
set buildfile=%0
call %0 buildall padding %languages%
::for %%a in (Dutch Spanish German Finnish French Italian Polish PT_BR Russian Swedish Serbian Slovene Turkish Ukr YU437 English) DO %0 %%a
popd

pushd .
cd build\localize
7z a -mx9 -tzip ..\localize.zip .
popd

REM source with filetimes matching commit
pushd .
cd build
md source
cd source
git clone https://github.com/FDOS/freecom.git
cd freecom
REM this is from git-tools, it is not included with git
git restore-mtime
REM remove git references
rd /s /q .git
rd /s /q .github
del .gitattributes
del .gitignore
cd ..
7z a -mx9 -tzip ..\freecom-source.zip .
popd

REM make FreeDOS package
pushd .
copy command.lsm %pkgdir%\APPINFO\COMMAND.LSM
copy bin\fdcom.ico %pkgdir%\BIN\FDCOM.ICO
copy build\English\xmsswap\command.com %pkgdir%\BIN\COMMAND.COM
copy FILE_ID.DIZ %pkgdir%\DOC\COMMAND\FILE_ID.DIZ
copy README.md %pkgdir%\DOC\COMMAND\README.MD
copy VERSION.TXT %pkgdir%\DOC\COMMAND\VERSION.TXT
copy docs\HELP.EN %pkgdir%\HELP\COMMAND.EN
copy docs\html\commands\cmd.html %pkgdir%\DOC\COMMAND\CMD.HTM
copy docs\html\commands\FreeCOM.html %pkgdir%\DOC\COMMAND\FREECOM.HTM
copy utils\MAKECMD.BAT %pkgdir%\NLS\MAKECMD.BAT
:: language specific versions copied into NLS during build
copy build\freecom-source.zip %pkgdir%\SOURCE\FREECOM\SOURCES.ZIP
cd %pkgdir%
7z a -mx9 -tzip ..\command.zip .
popd


call clean
SET languages=
SET pkgdir=
SET buildfile=
ECHO Done bulding all!
goto end

:buildall
rem skip past buildall dummy on first pass, skip last language on each successive pass
shift
shift
rem %1 and %2 are abbr and language name
if "%1"=="" goto end
pushd .
ECHO building %2 (%1)
call %buildfile% dobuild %2
popd
copy build\%2\xmsswap\command.com %pkgdir%\NLS\CMD-%1.COM
goto buildall

:dobuild
rem skip past dobuild
shift
rem %1 is the language name (should match one of the *.LNG files)
mkdir build\%1
copy VERSION.TXT build\%1\
copy command.lsm build\%1\

mkdir build\%1\xmsswap
call build -r %COMPILER% xms-swap %1
copy command.com build\%1\xmsswap\
copy tools\28.com build\%1\
copy tools\50.com build\%1\
copy tools\load_icd.exe build\%1\

copy strings\xmsswap.cln build\localize\
copy strings\fixstrs.exe build\localize\
copy strings\default.lng build\localize\
copy strings\%1.lng build\localize\
copy strings\strings.dat build\localize\%1.dat
copy tools\28.com build\localize\
copy tools\50.com build\localize\
copy tools\load_icd.exe build\localize\


mkdir build\%1\kswap
call build -r %COMPILER% no-xms-swap %1
copy command.com build\%1\kswap\
copy docs\k-swap.txt build\%1\kswap\
copy tools\kssf.com build\%1\kswap\
copy tools\vspawn.com build\%1\kswap\

copy strings\command.cln build\localize\
copy tools\kssf.com build\localize\
copy tools\vspawn.com build\localize\


mkdir build\%1\debug
call build -r %COMPILER% xms-swap debug %1
copy command.com build\%1\debug\


mkdir build\%1\plainedt
call build -r %COMPILER% xms-swap plainedt %1
copy command.com build\%1\plainedt\


pushd .
cd build\%1
7z a -mx9 -tzip ..\%1.zip .
popd

:end
