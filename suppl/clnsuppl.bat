@echo off
for %%a in (compile.me *.h suppl*.lib suppl*.lst vars.bat) do if exist %%a del %%a
for %%a in (all_done untar.exe untar.com untar.obj *.cfg) do if exist %%a del %%a
echo >src\test
IF NOT EXIST src\test goto noSrc

cd src
DEL *.c
FOR %%A in (*.*) DO DEL %%A
FOR %%A in (NLS\*.*) DO DEL %%A
RMDIR nls
CD ..
RMDIR src

:noSrc
