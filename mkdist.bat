@echo off
: Prepare binary distribution

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
for %stem in (cmddebug command) do for %ext in (new exe com cln) do (set file=%stem.%ext %+ if exist %file move %file old\%file)

dmake clobber || quit


: pause

: Make the "with debug" binary
set ndebug=
set debug=1

dmake com.exe || quit
: pause

if not exist com.exe goto ende
ren com.exe cmddebug.new
if exist com.exe goto ende
: pause

: Disable all debug stuff
set ndebug=Yes
set debug=

ren /q config.h config.h.backup || cancel 20
echo #define IGNORE_ENHANCED_INPUT >config.h
type config.h.backup >>config.h


set err=
dmake || set err=%?

del /q config.h
ren /q config.h.backup config.h || cancel 21

if not x%err == x cancel %err
move com.com packages\plainedt.std\command.com
: pause


dmake -W config.h || quit
: pause

if not exist com.com goto ende
ren com.com command.com
ren com.exe command.cln
if exist com.com goto ende
if exist com.exe goto ende

.\command.com /c get_ver.bat

: echo on %+ setdos /y1

copy cmddebug.new cmddebug.exe
call mkpkgs.bat


dmake clean || quit

ren cmddebug.new cmddebug.exe

rm -frd old

:ende
