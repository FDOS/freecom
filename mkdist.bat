@echo off
: Prepare binary distribution

set compiler=tc101
: set compiler=bc5
set lng=english

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
if exist com.com goto ende
: pause

: Disable all debug stuff
set ndebug=Yes
set debug=
dmake || quit
: pause

if not exist com.com goto ende
ren com.com command.com
ren com.exe command.cln
if exist com.com goto ende

dmake clean || quit

ren cmddebug.new cmddebug.exe

rm -frd old

:ende
