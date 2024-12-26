@echo off
: IF test script

cls
ECHO.
ECHO TEST SCRIPT FOR THE IF COMMAND
ECHO.
ECHO Testing the EXIST format
ECHO.
ECHO It is assumed that the file TST-IF.BAT does exist, but
ECHO NICHT-DA.TGL does not exist
ECHO.
if exist tst-if.bat       echo File TST-IF.BAT exists	        ((OK))
if not exist tst-if.bat   echo File TST-IF.BAT does not exist   ((ERROR!!??))
if exist nicht-da.tgl     echo File NICHT-DA.TGL exists         ((ERROR!!??))
if not exist nicht-da.tgl echo File NICHT-DA.TGL does not exist	((OK))
echo.
echo You should see "((OK))" two times
echo.
echo If you see an ERROR, check if the specified file is really
echo not there (TST-IF.BAT) or really is there (NICHT-DA.TGL)
echo.
pause

cls
ECHO Testing the ERRORLEVEL format
ECHO.
if exist errlvl.exe goto err_OK
if exist errlvl.com goto err_OK
echo Cannot find an executable of the ERRLVL program, please
echo compile it, in order to run this test
goto err_SKIP

:err_OK
errlvl 0
::echo Errorlevel %? (must be '0')
echo Errorlevel %errorlevel% (must be '0')
if not errorlevel 1 echo Probe #1 passed
if errorlevel 1 echo Probe #2 failed
errlvl 15
::echo Errorlevel %? (must be '15')
echo Errorlevel %ERRORLEVEL% (must be '15')
if errorlevel 15 echo Probe #3 passed
if errorlevel 16 echo Probe #4 failed
if errorlevel 14 echo Probe #5 passed
echo.
echo You should see passed probes 1, 3,& 5 and no failed one.
echo Also make sure that the displayed errorlevels match the one
echo mentioned as "must be 'XX'"
echo.
pause

cls
ECHO Testing the == format
ECHO.
if 1==1 echo Probe #1 passed
if 2==1 echo Probe #2 failed
if not 1==2 echo Probe #3 passed
if not 1==1 echo Probe #4 failed
if 1 == 1 echo Probe #5 passed
if 1 == 2 echo Probe #6 failed
if 1 ==1 echo Probe #7 passed
if 1 ==2 echo Probe #8 failed
if 1== 1 echo Probe #9 passed
if 1== 2 echo Probe #10 failed
if not 1=="1" echo Probe #11 passed
if not "1 2 3 4"=="1 2 3 4" echo Probe #12 failed
if "1 2 3 4"=="1 2 3 4" echo Probe #13 passed
echo.
echo You should see passed probes 1, 3, 5, 7, 9, 11,& 13 and no failed one
echo.
pause

cls
ECHO Testing /I and NOT options
ECHO.
if NOT a==A echo Probe #1 passed
if /I a==A echo Probe #2 passed
if /I NOT a==A echo Probe #3 failed
if NOT /I a==A echo Probe #4 failed
if /i b==A echo Probe #5 failed
if not /i not b==A ECHO Probe #6 passed
if /i a==A echo Probe #7 passed
if /i not exist nicht-da.tgl echo Probe #8 passed
if /i exist nicht-da.tgl echo Probe #9 failed
if not exist nicht-da.tgl echo Probe #10 passed
echo.
echo You should see passed probes 1, 2, 6, 7, 8, & 10 and no failed ones
echo.
pause

cls
ECHO Testing incomplete COMMAND
ECHO you should see multiple error messages "IF: Missing command" or similar message
ECHO.
REM [a==a], [a==a ], [a==a] note the middle one has a space before the end of line and other 2 do not
if a==a ECHO Next 3 commands should fail
if a==a
if a==a 
if a==a
echo.
pause

cls
echo Testing " and extra characters included with ==
echo .
if "a=="a echo Probe #1 passed
if "a == "a echo Probe #2 passed
if a "== a echo Probe #3 passed
if a garbage== a echo Probe #4 passed
if a garbage==garbage a echo Probe #5 failed
if .a == .a echo Probe #6 passed
if " a==" a echo Probe #7 failed, a is not a valid command
if " echo==" echo Probe #8 passed
echo .
echo You should see passed probes 1,2,3,4,6,& 8 with 7 giving a bad command error
pause

echo.
echo Test finished
