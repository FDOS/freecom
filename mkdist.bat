@echo off
: Prepare binary distribution
: set DBG=Yes

set _DBG=REM
if NOT "%DBG%"=="" set _DBG=

%_DBG setdos /y1 %+ %_DBG echo on

set compiler=tc101
: set compiler=bc5
set lng=english

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

%_DBG setdos /y0 %+ %_DBG echo off
dmake || quit
%_DBG setdos /y1 %+ %_DBG echo on
: pause

if exist packages\debug.std\kssf.com del /q packages\debug.std\kssf.com
if exist packages\debug.std\command.com del /q packages\debug.std\command.com
if not exist com.com goto ende
move com.com packages\debug.std\command.com
if exist com.com goto ende
if exist tools\kssf.com move tools\kssf.com packages\debug.std\kssf.com
if exist tools\kssf.com goto ende
:noKSSFDBG
: pause

: Disable all debug stuff
set ndebug=Yes
set debug=

ren /q config.h config.h.backup || cancel 20
echo #define IGNORE_ENHANCED_INPUT >config.h
type config.h.backup >>config.h


if exist packages\plainedt.std\kssf.com del /q packages\plainedt.std\kssf.com
if exist packages\binary.std\kssf.com del /q packages\binary.std\kssf.com
set err=
%_DBG setdos /y0 %+ %_DBG echo off
dmake || set err=%?
%_DBG setdos /y1 %+ %_DBG echo on

del /q config.h
ren /q config.h.backup config.h || cancel 21

if not x%err == x cancel %err
move com.com packages\plainedt.std\command.com
: pause


%_DBG setdos /y0 %+ %_DBG echo off
dmake -W config.h || quit
%_DBG setdos /y1 %+ %_DBG echo on
: pause
if exist tools\kssf.com copy /b tools\kssf.com packages\plainedt.std\kssf.com
if exist tools\kssf.com move tools\kssf.com packages\binary.std\kssf.com
if exist tools\kssf.com goto ende

if not exist com.com goto ende
copy /b com.exe + criter.mod\criter + criter.mod\criter1 command.cln
ren com.com command.com
if exist com.com goto ende

.\command.com /c get_ver.bat

: %_DBG echo on %+ setdos /y1

call mkpkgs.bat

%_DBG setdos /y0 %+ %_DBG echo off
dmake clean || quit

dmake command.mak || quit
%_DBG setdos /y1 %+ %_DBG echo on

rm -frd old

:: Make HTML documents
pushd docs\html\commands || quit
echo Updating HTML documents
perl db2html
popd

:ende
