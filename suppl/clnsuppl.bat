@echo off
IF EXIST SKIP goto noSrc
rem for %%a in (compile.me *.h suppl*.lib suppl*.lst vars.bat) do if exist %%a del %%a
for %%i in (all_done untar.exe untar.com untar.obj untar.map) do if exist %%i del %%i>nul
for %%i in (compile.me suppl*.lib suppl*.lst echoto.bat vars.bat *.cfg)  do if exist %%i del %%i>nul
IF NOT EXIST src\NUL goto noSrc

cd src
FOR %%A in (*.obj) DO DEL %%A
for %%a in (objlist.txt echolib.bat *.cfg) do if exist %%a del %%a>nul
rem FOR %%A in (NLS\*.*) DO DEL %%A
rem RMDIR nls
CD ..
rem RMDIR src

:noSrc
