@echo off
::=DISPLAY_FLAGS
::=pause
goto runME
ECHO Am I junk /?
:RUNME
call disp_arg 1 2 3 4 5 6 7 8 9 10
: Should arrive here!
pause Hey, cool man /?
disp_arg 1 2 3 4 5 6 7 8 9 10
: Should never arrive here!
pause if the program reaches this point, ARRGH!!
