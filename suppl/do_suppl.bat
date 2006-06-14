@echo off

:- Quaff the Options from the Makefile
if not exist vars.bat goto err1
call vars.bat

:- Extract the source files (Force overwrite)
rem untar.exe -f suppl.tgz
rem if errorlevel 1 goto ende

cd src

:- Cleaning up left over stuff a bit
if exist *.obj del *.obj

:- Fetch the config file
copy ..\%CFG% .
:- Compile all the sources
%CC% %CFLAGS% -I.. -c *.c
if errorlevel 1 goto ende

:- Prepare Linker Response File
if exist objlist.txt del objlist.txt
if "%OS%"=="Windows_NT" goto linkWinNT
for %%x in (*.obj) do echo +%%x & >> objlist.txt
goto linkDOS
:linkWinNT
for %%x in (*.obj) do echo +%%x ^& >> objlist.txt
:linkDOS
echo , ..\%SUPPL%.lst >> objlist.txt 

:- Create the library
%AR% /C ..\%SUPPL%.LIB @objlist.txt
if errorlevel 1 goto ende

Echo Library created
Echo Removing sources files
:- Prevent DEL *.* from questioning us
rem DEL *.c
rem DEL *.h
DEL *.obj
DEL objlist.txt
DEL %CFG% tccdos.cfg
rem FOR %%A in (*.*) DO DEL %%A
rem FOR %%A in (NLS\*.*) DO DEL %%A
rem RMDIR nls
CD ..
rem RMDIR src

Echo done.
Echo All DONE >all_done

goto ende

:err1
echo Missing VARS.BAT from make -f suppl.mak all
goto ende

:ende
