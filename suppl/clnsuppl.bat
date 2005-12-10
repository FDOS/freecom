@echo off
IF EXIST SKIP goto noSrc
for %%a in (compile.me *.h suppl*.lib suppl*.lst vars.bat) do if exist %%a del %%a
for %%a in (all_done untar.exe untar.com untar.obj untar.map *.cfg) do if exist %%a del %%a
IF NOT EXIST src\NUL goto noSrc

cd src
DEL *.c
FOR %%A in (*.*) DO DEL %%A
FOR %%A in (NLS\*.*) DO DEL %%A
RMDIR nls
CD ..
RMDIR src

:noSrc
