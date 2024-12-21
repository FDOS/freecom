@echo off
if "%2%3%4%5%6%7%8%9" == "" goto nothing
set AMP=&
if "%AMP%" == "" set AMP=^^^&
echo %2 %3 %4 %5 %6 %7 %8 %9 %AMP%>>%1
shift
if not "%9" == "" echo echolib.bat arguments overflow
:nothing
