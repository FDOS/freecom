@echo off
set FILE=%1
set STR=%2
if "%STR%" == "" goto end_of_script

:loop
shift
if "%2" == "" goto end
set STR=%STR% %2
goto loop
:end

echo %STR%>> %FILE%
:end_of_script

