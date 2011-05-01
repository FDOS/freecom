@echo off
ms2wlink %1 %2 %3 %4 %5 %6 %7 %8 %9 ,,,, > tmp.lnk
echo op map,statics,verbose >> tmp.lnk
call wlink @tmp.lnk
del tmp.lnk
