@echo off
:: The batchfiles recurs*.bat shallt create the following output,
:: though, the absolute filenames are likely be different:
:: Invoked via:		C> RECURS1.BAT >RECURS1.OUT
:: == RECURS1.OUT
:: D:\FREEDOS\SRC\CVS.EC\FREECOM\2\RECURS1.BAT -- #16
:: D:\FREEDOS\SRC\CVS.EC\FREECOM\2\RECURS1.BAT -- #18
:: == RECURS2.OUT
:: D:\FREEDOS\SRC\CVS.EC\FREECOM\2\RECURS2.BAT -- #1
:: D:\FREEDOS\SRC\CVS.EC\FREECOM\2\RECURS2.BAT -- #3
:: == RECURS3.OUT
:: D:\FREEDOS\SRC\CVS.EC\FREECOM\2\RECURS3.BAT -- #1
:: == RECURS4.OUT
:: D:\FREEDOS\SRC\CVS.EC\FREECOM\2\RECURS4.BAT -- #1
:: ==
echo %@BATCHNAME -- #%@BATCHLINE
call recurs2 >recurs2.out
echo %@BATCHNAME -- #%@BATCHLINE
recurs3 >recurs3.out
echo Should not be here! %@BATCHNAME -- #%@BATCHLINE
