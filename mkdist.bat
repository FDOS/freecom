@echo off
: Prepare binary distribution

set compiler=tc101
: set compiler=bc5
set lng=english

if exist cmddebug.old del cmddebug.old >nul
if exist command.old del command.old >nul
if exist cmddebug.com ren cmddebug.com *.old >nul
if exist command.com ren command.com *.old >nul

dmake clobber || quit
: pause

: Make the "with debug" binary
set ndebug=
set debug=1

dmake || quit
: pause

if not exist com.com goto ende
ren com.com cmddebug.com
ren com.exe cmddebug.cln
if exist com.com goto ende
: pause

dmake clean || quit
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

if exist cmddebug.old del cmddebug.old >nul
if exist command.old del command.old >nul

:ende
