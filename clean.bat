@echo off
if "%1"=="" goto main
for %%i in (%2 %3 %4 %5 %6 %7 %8 %9) do if exist %1%%i del %1%%i>nul
goto end

:main
cd suppl
call clnsuppl.bat
cd ..
call %0 .\ command.com command.upx 
call %0 .\ lastmake.mk context.h_c context.inc strings.h infores info.txt gnuconf.mak
call %0 criter\ criter criter1 context.def context.inc context.h_c
call %0 strings\ command.cln xmsswap.cln strings.h strings.err strings.dat strings.log
call %0 strings\strings\ makefile 
call %0 strings\strings\*. c
call %0 tools\ makefile.mak
call %0 tools\*. icd

for %%i in (cmd lib strings strings\strings) do call %0 %%i\*. lib lst
for %%i in (cmd lib shell strings strings\strings tools utils utilsc) do call %0 %%i\*. obj o cfg map exe com rsp
for %%i in (cmd lib shell strings strings\strings tools utils utilsc) do call %0 %%i\ echoto.bat echolib.bat gnumake.mak
for %%i in (. criter) do call %0 %%i\ gnumake.mak

:end

