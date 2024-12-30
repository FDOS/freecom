@echo off
if "%1"=="" goto main
for %%i in (%2 %3 %4 %5 %6 %7 %8 %9) do if exist %1%%i del %1%%i>nul
goto end

:main
cd suppl
call clnsuppl.bat
cd ..
call %0 command.com command.upx 
call %0 .\ lastmake.mk context.h_c context.inc strings.h infores info.txt
call %0 strings\ command.cln xmsswap.cln
call %0 strings\ strings.h   strings.err strings.dat
call %0 strings\ strings.lib strings.lst strings.log
call %0 strings\strings\ strings.lib strings.lst strings.rsp makefile 
call %0 criter\  criter criter1 context.def context.inc context.h_c
call %0 cmd\     cmds.lib    cmds.lst    cmds.rsp
call %0 lib\     freecom.lib freecom.lst freecom.rsp
call %0 shell\   command.exe command.map command.rsp
call %0 tools\   makefile.mak

call %0 strings\*.     obj     exe
call %0 strings\strings\*. c obj
call %0 tools\*.   icd obj map exe com
call %0 utils\*.       obj map exe

for %%i in (cmd lib shell criter) do if exist %%i\*.obj del %%i\*.obj>nul
for %%i in (cmd lib shell string tools utils) do if exist %%i\*.cfg del %%i\*.cfg>nul
for %%i in (cmd lib shell string tools utils strings strings\strings) do if exist %%i\echoto.bat del %%i\echoto.bat>nul
for %%i in (cmd lib shell string tools utils) do if exist %%i\echolib.bat del %%i\echolib.bat>nul

:end

