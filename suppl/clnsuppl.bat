@echo off
IF EXIST SKIP goto noSrc
rem for %%a in (compile.me *.h suppl*.lib suppl*.lst vars.bat) do if exist %%a del %%a
for %%a in (all_done untar.exe untar.com untar.obj untar.map *.cfg) do if exist %%a del %%a
IF NOT EXIST src\NUL goto noSrc

rem cd src
rem DEL *.c
rem FOR %%A in (*.*) DO DEL %%A
rem FOR %%A in (NLS\*.*) DO DEL %%A
rem RMDIR nls
rem CD ..
rem RMDIR src

:noSrc
