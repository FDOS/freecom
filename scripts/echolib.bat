@echo off
if "%2%3%4%5%6%7%8%9" == "" goto nothing
if '^(' == '(' goto :cmd_shell
echo +%2 & >>%1
if "%3%" == "" goto nothing
echo +%3 & >>%1
if "%4%" == "" goto nothing
echo +%4 & >>%1
if "%5" == "" goto nothing
echo +%5 & >>%1
if "%6" == "" goto nothing
echo +%6 & >>%1
if "%7" == "" goto nothing
echo +%7 & >>%1
if "%8" == "" goto nothing
echo +%8 & >>%1
if "%9" == "" goto nothing
echo +%9 & >>%1
goto :args_check
:cmd_shell
echo +%2 ^& >>%1
if "%3%" == "" goto nothing
echo +%3 ^& >>%1
if "%4%" == "" goto nothing
echo +%4 ^& >>%1
if "%5" == "" goto nothing
echo +%5 ^& >>%1
if "%6" == "" goto nothing
echo +%6 ^& >>%1
if "%7" == "" goto nothing
echo +%7 ^& >>%1
if "%8" == "" goto nothing
echo +%8 ^& >>%1
if "%9" == "" goto nothing
echo +%9 ^& >>%1
:args_check
shift
if "%9" == "" goto nothing
echo echolib.bat arguments overflow
:nothing
