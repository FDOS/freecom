echo
echo hi, this is a test
freda
rem a comment
pause
@echo the path is %PATH%
echo the path is %path%
echo Batch file called as : %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
@shift
echo shifted : %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
@shift down
echo shifted  down: %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
pause
echo this is bogus: %bogus%
echo this is a test: %test
echo
echo off
echo
goto next

echo this line will never be reached

:next
echo continue form here...
echo END OF test.bat
