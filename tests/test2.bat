@echo off
echo Testing : %0 %1 %2 %3 %4 %5 %6 %7 %8
shift
echo Testing : %0 %1 %2 %3 %4 %5 %6 %7 %8
shift
echo Testing : %0 %1 %2 %3 %4 %5 %6 %7 %8

echo now test the environment substitution
echo path is %path%
echo PATH is %PATH%
echo Absent variable is %Absent%
echo Bad format : %bad format
echo Testing double %%comspec%%

echo Call another batch file...
call test3.bat t3p1 t3p2 t3p3

echo .
echo Returned to TEST2
echo .
echo Testing : %0 %1 %2 %3 %4 %5 %6 %7 %8
shift
echo Testing : %0 %1 %2 %3 %4 %5 %6 %7 %8
shift
echo Testing : %0 %1 %2 %3 %4 %5 %6 %7 %8
echo END OF test2.bat
