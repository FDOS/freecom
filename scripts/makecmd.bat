@echo off
: Copyright (C) Bernd Blaauw
: Creates localized FreeCOM-versions depending on which language files
: are available in the current directory
: Note: all versions use default lang (compiled in) for critical errors, usually English
: Usage: makecmd
:        makecmd YY Name
: where no arguments creates all variants
: where YY Name just creates version for specified language
if "%1"=="continue" goto loop
SET MAKEALL=%1
if "%1"=="" SET MAKEALL=EN
if "%1"=="" del strings.* > NUL
set languages=%1 %2
if "%1"=="" set languages=NL Dutch ES Spanish DE German FI Finnish FR French IT Italian PO Polish PTR PT_BR RU Russian SE Swedish Ser Serbian SI Slovene TR Turkish UKR Ukr YUG YU437
call %0  continue %languages%
:: do english last and keep the strings.dat
if "%1"=="" call %0 continue EN English
set languages=
set MAKEALL=
goto end

:loop
shift
if "%1"=="" goto end
set lang=%1
goto compile
::if exist %1.lng del %1.lng
goto end

:compile
if "%2"=="" goto end
for %%x in ( command.cln xmsswap.cln fixstrs.exe default.lng %2.lng ) do if not exist %%x goto end
if exist strings.err del strings.err
if exist strings.log del strings.log
fixstrs %2.lng
copy strings.dat strings.%1 > NUL
if "%MAKEALL%"=="%1" copy /b command.cln + strings.dat command.com > NUL
if "%MAKEALL%"=="%1" copy /b xmsswap.cln + strings.dat xmsswap.com > NUL
if exist strings.err echo Translation file out of date for language %2 [file: %2.lng]!
if exist strings.log echo Translation file out of date for language %2 [file: %2.lng]!
:: leave strings.dat if matches MAKEALL, which must be last language specified, i.e. leave a default STRINGS.DAT
if NOT "%MAKEALL%"=="%1" if exist strings.dat del strings.dat
if NOT "%MAKEALL%"=="%1" if exist strings.log del strings.log
if exist strings.h   del strings.h
shift
goto loop
:end
if "%lang%"=="" echo Warning: nothing done due to LANG variable not set
