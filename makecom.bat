@echo off
:This batch file takes the command.exe and appends the strings.dat
:file, and creates command.com

if not exist command.exe goto err1

copy /b command.exe+strings.dat command.com
del command.exe
goto ende

:err1
echo You must make COMMAND.EXE first, e.g. typing
echo MAKE -f COMMAND.MAK    or  DMAKE

:ende
