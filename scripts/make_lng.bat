@echo off
: Make all the language files from the *.LNG files
: Bernd Blaauw
if "%1"=="continue" goto loop
set languages=EN English NL Dutch FR French DE German IT Italian PTR PT_BR RU Russian Ser Serbian YUG YU437  call %0  continue %languages%
goto end

:loop
shift
if "%1"=="" goto end
if "%lang%"=="%1" goto compile
if exist %1.lng del %1.lng
shift
goto loop
goto end

:compile
if "%2"=="" goto end
for %%x in ( command.cln xmsswap.cln fixstrs.exe default.lng %2.lng ) do if not exist %%x goto end
fixstrs %2.lng
copy /b command.cln + strings.dat command.com
copy /b xmsswap.cln + strings.dat xmsswap.com
if exist strings.err echo Translation file out of date for language %2 [file: %2.lng]!
if exist strings.dat del strings.dat
if exist strings.h   del strings.h
goto end
:end
if "%lang%"=="" echo Warning: nothing done due to LANG variable not set
