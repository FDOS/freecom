@echo off
: Prepare binary distribution

set compiler=bc5
set lng=english

if exist com_dbg.old del com_dbg.old >nul
if exist command.old del command.old >nul
if exist com_dbg.com ren com_dbg.com *.old >nul
if exist command.com ren command.com *.old >nul

dmake clobber || quit
: pause

: Make the "with debug" binary
set ndebug=
set debug=1

dmake || quit
: pause

if not exist com.com goto ende
ren com.com com_dbg.com
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
if exist com.com goto ende

dmake clean || quit

if exist com_dbg.old del com_dbg.old >nul
if exist command.old del command.old >nul

:ende
