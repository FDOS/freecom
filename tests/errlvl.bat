@echo off
ECHO should print Four Three Two One Zero 3 2 1 each on its own line
SET LVL=4
:loop
errlvl %LVL%
IF ERRORLEVEL 4 GOTO FOUR
IF ERRORLEVEL 3 GOTO THREE
IF ERRORLEVEL 2 GOTO TWO
IF ERRORLEVEL 1 GOTO ONE

echo Zero
GOTO END

:FOUR
echo Four
SET LVL=3
GOTO loop

:THREE
echo Three
SET LVL=2
GOTO loop

:TWO
echo Two
SET LVL=1
GOTO loop

:ONE
echo One
SET LVL=0
GOTO loop

:END
errlvl 3
ECHO %ERRORLEVEL%
errlvl 2
ECHO %ERRORLEVEL%
errlvl 1
ECHO %ERRORLEVEL%
echo Done
