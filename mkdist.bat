@echo off
: Prepare binary distribution
set DBG=Yes
set DBG=

: don't forget to pass on the default rather than mine
copy config.std config.mak
copy /u docs\download.txt .

for %pkg in (binary debug plainedt xmsswap) for %fnam in (ptchldrv.exe ptchsize.exe kssf.com vspawn.com command.com) (set fn=packages\%pkg.std\%fnam %+ if exist %fn del /q %fn)


set _DBG=REM
if NOT "%DBG%"=="" set _DBG=

%_DBG setdos /y1 %+ %_DBG echo on

set compiler=tc101
set compiler=bc5
set lng=english
set model=
set xms_swap=

iff exist config.h.backup then
	echo config.h.backup exists
	echo something went wrong last time
	cancel 30
endiff

iff not isdir old then
	mkdir old
	if not isdir old (echo Cannot create OLD %+ quit 10)
else
	del /y old\*.*
endiff
for %stem in (cmddebug command kssf kssf_dbg) do for %ext in (new exe com cln) do (set file=%stem.%ext %+ if exist %file move %file old\%file)

%_DBG setdos /y0 %+ %_DBG echo off
dmake clobber || quit
%_DBG setdos /y1 %+ %_DBG echo on

: pause

: Make the "with debug" binary
set ndebug=
set debug=1
set compiler=bc5

%_DBG setdos /y0 %+ %_DBG echo off
dmake MODEL!:=l || quit
%_DBG setdos /y1 %+ %_DBG echo on
: pause

if not exist com.com goto ende
move com.com packages\debug.std\command.com
if exist com.com goto ende
for %file in (tools\kssf.com tools\vspawn.com tools\ptchldrv.exe tools\ptchsize.exe) (if exist %file move %file packages\debug.std\ %+ if exist %file goto ende)
:noKSSFDBG
: pause

: Disable all debug stuff
set ndebug=Yes
set debug=
set compiler=tc101
set model=s

set XMS_SWAP=Yes
%_DBG setdos /y0 %+ %_DBG echo off
dmake || quit
%_DBG setdos /y1 %+ %_DBG echo on
if not exist com.com goto ende
iff not exist tools\ptchsize.exe then
	echo no PTCHSIZE
	goto ende
endiff
tools\ptchsize.exe com.com +7KB
if errorlevel 1 goto ende
move com.com packages\xmsswap.std\command.com
copy /b shell\com.exe +infores + criter\criter + criter\criter1 packages\localize.std\xmsswap.cln
if exist com.com goto ende
set XMS_SWAP=
: pause


ren /q config.h config.h.backup || cancel 20
echo #define IGNORE_ENHANCED_INPUT >config.h
type config.h.backup >>config.h


set err=
%_DBG setdos /y0 %+ %_DBG echo off
dmake || set err=%?
%_DBG setdos /y1 %+ %_DBG echo on

del /q config.h
ren /q config.h.backup config.h || cancel 21

if not x%err == x cancel %err
if not exist com.com goto ende
iff not exist tools\ptchsize.exe then
	echo no PTCHSIZE
	goto ende
endiff
tools\ptchsize.exe com.com +6KB
if errorlevel 1 goto ende
move com.com packages\plainedt.std\command.com
if exist com.com goto ende
: pause


del /q shell\*.exe lastmake.mk
touch config.h
%_DBG setdos /y0 %+ %_DBG echo off
dmake -W config.h || quit
%_DBG setdos /y1 %+ %_DBG echo on
: pause

if not exist com.com goto ende
copy /b shell\com.exe +infores + criter\criter + criter\criter1 command.cln
ren com.com command.com
if exist com.com goto ende
iff not exist tools\ptchsize.exe then
	echo no PTCHSIZE
	goto ende
endiff
tools\ptchsize.exe command.com +6KB
if errorlevel 1 goto ende

for %file in (tools\kssf.com tools\vspawn.com tools\ptchldrv.exe tools\ptchsize.exe) (if exist %file copy %file packages\plainedt.std\ %+ if exist %file move %file packages\binary.std\ %+ if exist %file goto ende)

perl get_ver.pl .\command.com

: %_DBG echo on %+ setdos /y1

:: Make HTML documents
pushd docs\html\commands || quit
echo Updating HTML documents
call upd_html.bat
popd

: %_DBG echo on %+ setdos /y1
call mkpkgs.bat

%_DBG setdos /y0 %+ %_DBG echo off
dmake clean dist || quit
%_DBG setdos /y1 %+ %_DBG echo on

rm -frd old

:ende
