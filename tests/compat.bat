@ECHO OFF
SET DEV=1
IF NOT "%1"=="EXTENDED" IF NOT "%1"=="" GOTO %1
REM This is intended to be a primary batch file for standard MSDOS compatible testing
REM Run with option EXTENDED for testing advanced FreeCOM commands supported
REM Requires errlvl.exe in same directory and current directory writable
REM Note: if piping output to a file without being visible, press Enter twice after a small delay
REM Note: if updating, then change SET DEV=0 at top to SET DEV=1 for additional PAUSEs
REM This file uses many quirks and tests and is spaghetti like for testing batch file processing
REM
REM commands with a * are post MSDOS version 6 commands
REM commands with a ** are not included in FreeCOM by default but optionally compiled in
REM neither * nor ** commands are be tested if 1st argument is not EXTENDED
REM unsupported:         SETLOCAL ENDLOCAL ASSOC COLOR LOCK UNLOCK ELSE
REM
REM BATCH file commands: REM ECHO GOTO IF *ELSE NOT EXIST FOR PAUSE SHIFT EXIT *SETLOCAL *ENDLOCAL
REM EXECUTION commands:  CALL COMMAND LOADFIX LOADHIGH LH PATH *ASSOC
REM GENERAL commands:    SET CLS *COLOR TYPE PROMPT BREAK VERIFY VER *TITLE
REM VOLUME commands:     VOL *LOCK *UNLOCK
REM DIRECTORY commands:  CD CHDIR MD MKDIR RD RMDIR *PUSHD *POPD
REM FILE commands:       COPY DEL ERASE DIR MOVE REN RENAME TRUENAME
REM DATE/TIME commands:  DATE TIME
REM INTL & CON commands: CHCP CTTY
REM note: environment variables also expanded in REMarks
REM INTERNAL VARIABLES:  %%ERRORLEVEL%% (%ERRORLEVL%)
REM ENV VARIABLES:       %%COMSPEC%% %%DIRCMD%% %%PROMPT%% %%PATH%%
REM FreeCOM commands:    ALIAS BEEP CDD DIRS DOSKEY HISTORY LFNFOR **MEMORY ? WHICH WHERE
IF "%DEV%"=="1" pause
REM PROMPT options: 
REM $b vertical bar (pipe) *** can't use actual symbol as COMMAND will try to do the redirection
REM $d system date
REM $e esc char
REM $g greater than symbol
REM $h destructive backspace
REM $i less than symbol
REM $n current (default) drive letter
REM $p current (default) path including drive
REM $q =
REM $t sytem time
REM $v DOS/COMMAND.COM version string
REM $_ \r\n
REM $$ $
REM WILDCARDS: * and ?
REM PIPING/REDIRECTION: gt overwrite gtgt append pipe and ls pipe to another program
REM These are remarks and should be ignored

REM this will fail on MS/PC DOS COMMAND.COM but not CMD, ignored by FreeCOM
TITLE FreeCOM tests
IF "%DEV%"=="1" pause

REM testing ECHO
IF "%1"=="EXTENDED" ECHO /?
ECHO ON
ECHO this command should be echo'd and then this comment shown
@ECHO OFF
ECHO this comment should be shown but the command not echo'd
ECHO this should say ECHO is off
ECHO
ECHO and this should say ECHO is on
ECHO ON
@ECHO
@ECHO OFF
ECHO and this should be followed by a blank line
ECHO,
ECHO the following command shows the version 3 times  
ECHO *** these will not match other COMMAND.COMs !!!
ECHO testing variations of explicit no echo (using @) with echo on
ECHO and then explicit no echo (using @) with echo off
ECHO and again with just ECHO OFF
ECHO ON
@VER
@ECHO OFF
@VER
VER
IF "%DEV%"=="1" pause
REM determine if running under CMD on Windows, ver /r not supported and sets errorlevel
ECHO if running under CMD on Windows this will say syntax incorrect
ver /r
SET SHELL_IS_CMD=0
if "%ERRORLEVEL%"=="1" SET SHELL_IS_CMD=1
REM determine if running under COMMAND.COM, it doesn't support labels longer than 8 chars
GOTO tst_cmd_freecom
:tst_cmd_
SET SHELL_IS_COMMAND=1
GOTO t_cmd_end
:tst_cmd_freecom
SET SHELL_IS_COMMAND=0
:t_cmd_end
ECHO SHELL_IS_COMMAND is %SHELL_IS_COMMAND% (0 for CMD/FREECOM, 1 for COMMAND)
IF "%DEV%"=="1" pause

REM TESTING IF and GOTO and comparison to arguments
IF "%1"=="EXTENDED" GOTO ext_ver
:continue_ver
GOTO tst_goto
ECHO THIS SHOULD NOT BE SHOWN!
:tst_goto
ECHO VER complete
GOTO skip_ext_ver
REM this section only evaluated if EXTENDED passed on command line
:ext_ver
ECHO EXTENDED version tests
ECHO show version help
VER /?
ECHO show kernel version
VER /R
ECHO show shell warranty
VER /W
ECHO show shell redistribution info
VER /D
ECHO show shell contributors
VER /C
GOTO continue_ver

REM continue normal testing
:skip_ext_ver
ECHO Testing IF and ERRORLEVEL
REM %errorleve% not supported by MSDOS, last command was ver /r so 0 if supported (dos), 1 if not (win)
ECHO ERRORLEVEL %ERRORLEVEL% should be 0 or "" on COMMAND (DOS) and 1 with CMD (WIN)
ECHO should print Four Three Two One Zero each on its own line
SET LVL=4
:loop
errlvl %LVL%
IF ERRORLEVEL 4 GOTO FOUR
IF ERRORLEVEL 3 GOTO THREE
IF ERRORLEVEL 2 GOTO TWO
IF ERRORLEVEL 1 GOTO ONE

echo Zero
GOTO tst_show_errlvl

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

:tst_show_errlvl
ECHO if %%ERRORLEVEL%% is supported shows 3 2 1 each on its own line, else ECHO is off 3 times
errlvl 3
ECHO %ERRORLEVEL%
errlvl 2
ECHO %ERRORLEVEL%
errlvl 1
ECHO %ERRORLEVEL%
ECHO,
ECHO Testing PAUSE
IF "%1"=="EXTENDED" PAUSE /?
REM CMD and COMMAND will ignore prompt and always show default pause message
PAUSE Press any key and then screen will clear & the default pause message will show
CLS
PAUSE

ECHO Testing label variations work
: Usual spelling
goto label1
ECHO If you see me then label1 didn't work
:label1
echo Label 1 found
: Prefixed colon
goto :label2
ECHO If you see me then label2 didn't work
:label2
echo Label 2 found
: Prefixed colon seperated by whitespaces
: Though 4dos searches for ' label3' here?
: doesn't work in CMD or COMMAND, so we check if a bad ver argument has errorlevel 1 and skip it
IF NOT "%1"=="EXTENDED"  GOTO label3
REM the following line may terminate the batch file if unsupported instead of showing didn't work
goto : label3
ECHO If you see me then label3 didn't work
:label3
echo Label 3 found
IF "%DEV%"=="1" pause

ECHO Testing IF and EXISTS
ECHO Testing the EXIST format, you should see "((OK))" two times
echo.
ECHO WARNING!!! this will overwrite NOTAFILE.ERR if it exists and delete it!
ECHO This is a test file and should be deleted > NOTAFILE.ERR
ECHO.
if exist NOTAFILE.ERR     echo File NOTAFILE.ERR exists	        ((OK))
if not exist NOTAFILE.ERR echo File NOTAFILE.ERR does not exist ((ERROR!!??))
REM this will delete NOTAFILE.ERR, but should not display anything in the process
ERASE NOTAFILE.ERR
if exist NOTAFILE.ERR     echo File NOTAFILE.ERR exists         ((ERROR!!??))
if not exist NOTAFILE.ERR echo File NOTAFILE.ERR does not exist	((OK))
echo.
IF "%DEV%"=="1" pause

:tst_SHIFT
ECHO invoking self [%0] with showargs as label to goto then testing shift
ECHO This will print the batch name showargs and empty arguments from 2 to 9
CALL %0 showargs
ECHO back from self
IF "%DEV%"=="1" pause
ECHO This will print the batch name showargs and arguments matching from 2 to 9
CALL %0 showargs 2 3 4 5 6 7 8 9
ECHO back from self
IF "%DEV%"=="1" pause
ECHO This will print the batch name showargs and args from 2-9 then repeating to 10 and to 11
CALL %0 showargs 2 3 4 5 6 7 8 9 10 11
ECHO back from self
IF "%DEV%"=="1" pause
GOTO end_tst_SHIFT

ECHO oops why do you see me?
PAUSE

:showargs
echo 0=%0
echo 1=%1
echo 2=%2
echo 3=%3
echo 4=%4
echo 5=%5
echo 6=%6
echo 7=%7
echo 8=%8
echo 9=%9
SHIFT
ECHO "%9%" is it blank? no then keep looping
IF NOT "%9"=="" GOTO showargs
GOTO END

:end_tst_SHIFT
ECHO,
ECHO Testing CALL and COMMAND /C
ECHO should see in bat 1, in bat 2, in bat 3, back to bat 2, back to bat 1
ECHO @ECHO OFF > _b1.bat
ECHO echo in bat 1 >> _b1.bat
ECHO call _b2.bat >> _b1.bat
ECHO echo back in bat 1 >> _b1.bat
ECHO @ECHO OFF > _b2.bat
ECHO echo in bat 2 >> _b2.bat
ECHO call _b3.bat >> _b2.bat
ECHO echo back in bat 2 >> _b2.bat
ECHO @ECHO OFF > _b3.bat
ECHO echo in bat 3 >> _b3.bat
CALL _b1.bat
IF "%DEV%"=="1" pause
REM using COMMAND /C form instead of call
ECHO COMSPEC is %COMSPEC%
%COMSPEC% /C _b1.bat
DEL _b1.bat
DEL _b2.bat
DEL _b3.bat
IF "%DEV%"=="1" pause

:tst_FOR
ECHO,
ECHO Testing FOR
IF "%1"=="EXTENDED" FOR /?
if %SHELL_IS_CMD%==1 GOTO benny_done
ECHO BENNY level test - IF ERRORLEVEL HA or IF ERRORLEVEL xa
ECHO where A is A-Z for errorcode 1 to 26 and a is a-z for errorcode 1 to 26
ECHO this will fail on Windows COMMAND and 4DOS with HA was unexpected at this time.
REM benny test for last drive, requires program to return last drive as errorlevel so we fake as Y using ERRLVL
ERRLVL 25
SET drv=:
FOR %%Z IN (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) DO IF ERRORLEVEL H%%Z SET drv=%%Z
IF ERRORLEVEL 27 SET drv=
IF "%drv%"=="" ECHO ERROR: No drive letter - Exit code == %ERRORLEVEL%
IF NOT "%drv%"=="" ECHO Faked last drive letter: %drv%: (should be Y)
ERRLVL 25
SET drv=:
FOR %%Z IN (Z Y X W V U T S R Q P O N M L K J I H G F E D C B A) DO IF ERRORLEVEL H%%Z SET drv=%%Z
IF ERRORLEVEL 27 SET drv=
IF "%drv%"=="" ECHO ERROR: No drive letter - Exit code == %ERRORLEVEL%
IF NOT "%drv%"=="" ECHO Faked last drive letter: %drv%: (should be A)
ERRLVL 3
SET drv=:
FOR %%Z IN (a b c d e f g h i j k l m n o p q r s t u v w x y z) DO IF ERRORLEVEL x%%Z SET drv=%%Z
IF ERRORLEVEL 27 SET drv=
IF "%drv%"=="" ECHO ERROR: No drive letter - Exit code == %ERRORLEVEL%
IF NOT "%drv%"=="" ECHO Faked last drive letter: %drv%: (should be C)
SET drv=
:benny_done
FOR %%A IN (3 2 1) DO ECHO %%A
IF "%DEV%"=="1" pause

:tst_CTTY
if %SHELL_IS_CMD%==1 GOTO skip_ctty
IF "%1"=="EXTENDED" CTTY /?
ECHO.
ECHO Testing CTTY
ctty nul
echo not to be seen
ctty con
ECHO if nothing was shown then successful
IF "%DEV%"=="1" pause
:skip_ctty

:tst_load
if %SHELL_IS_CMD%==1 GOTO skip_load
ECHO;
ECHO LOADFIX, LOADHIGH, and LH
LOADFIX /?
LOADHIGH /?
LH /?
errlvl 33
ECHO %ERRORLEVEL% is 33
REM this is external for MSDOS so don't run for COMMAND
IF "%SHELL_IS_COMMAND%"=="1" GOTO skip_lfx
LOADFIX errlvl 32
ECHO %ERRORLEVEL% is 32
:skip_lfx
LOADHIGH errlvl 31
ECHO %ERRORLEVEL% is 31
LH errlvl 30
ECHO %ERRORLEVEL% is 30
:: TODO add a program that shows it load address to verify lh and loadfix loaded program correctly
IF "%DEV%"=="1" pause
:skip_load
ECHO.
ECHO testing if can continue a batch file if changed while executing
ECHO @ECHO ON > _b1.bat
ECHO ECHO 2nd line of _b1.bat >> _b1.bat
ECHO COPY /A _b1.bat + _b2.bat _b1.bat >> _b1.bat
REM next line is 1 char short of filling line, should not print blank line
ECHO ECHO 4th line of _b1.bat    and only line of _b2> _b2.bat
ECHO _b1.bat is:
TYPE _b1.bat
ECHO _b2.bat is:
TYPE _b2.bat
ECHO running _b1.bat now:
CALL _b1.bat
ECHO should restore to being off here
@ECHO OFF
ECHO _b1.bat should now include _b2.bat and all the lines should have printed
TYPE _b1.bat
IF "%DEV%"=="1" pause

ECHO TESTING PATH MD CD RD
IF "%1"=="EXTENDED" PATH /?
IF "%1"=="EXTENDED" MD /?
IF "%1"=="EXTENDED" MKDIR /?
IF "%1"=="EXTENDED" CD /?
IF "%1"=="EXTENDED" CHDIR /?
IF "%1"=="EXTENDED" RD /?
IF "%1"=="EXTENDED" RMDIR /?
IF "%1"=="EXTENDED" CDD /?
ECHO @ECHO Hello from bat 1 > _b1.bat
MKDIR TESTING
COPY _b1.bat TESTING
ECHO PATH is %PATH%
PATH
SET __PATH=%PATH%
PATH TESTING
ECHO PATH is now %PATH%
PATH
CALL _b1.bat
SET PATH=%__PATH%
ECHO PATH is restored to %PATH%
PATH
ECHO current path is:
CD
CD TESTING
ECHO new path is:
if %SHELL_IS_CMD%==1 ECHO %CD%
if %SHELL_IS_CMD%==0 IF %SHELL_IS_COMMAND%==0 CDD
if %SHELL_IS_COMMAND%==1 CD
del _b1.bat
CD ..
RD TESTING
MD TESTING
copy _b1.bat TESTING
CHDIR TESTING
CALL _b1.bat
CHDIR ..
REM CDD TESTING
if %SHELL_IS_CMD%==0 IF %SHELL_IS_COMMAND%==0 CDD TESTING
if %SHELL_IS_CMD%==1 CD TESTING
if %SHELL_IS_COMMAND%==1 CD TESTING
ECHO still in same path?
CD
REM CDD -
if %SHELL_IS_CMD%==0 IF %SHELL_IS_COMMAND%==0 CDD -
if %SHELL_IS_CMD%==1 CD ..
if %SHELL_IS_COMMAND%==1 CD ..
ECHO should be back to original path
CD
del TESTING\_b1.bat
RMDIR TESTING
DEL _b1.bat
IF EXIST _b2.bat DEL _b2.bat
IF EXIST _b3.bat DEL _b3.bat
IF "%DEV%"=="1" pause

ECHO.
ECHO testing passing long command to FreeCOM and calling a program with a long command
ECHO this tests setting CMDLINE for caller and FreeCOM reading CMDLINE as callee
ECHO CMDLINE is '%CMDLINE%'
ECHO running a long command:
ECHO ON
%COMSPEC% /c ECHO 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 12345
%COMSPEC% /c ECHO 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
@ECHO OFF
echo now test the environment substitution
echo path is %path%
echo PATH is %PATH%
echo Absent variable is %Absent%
echo Bad format : %bad format
echo Testing double %%comspec%%
IF "%DEV%"=="1" pause

CLS
IF "%1"=="EXTENDED" CLS /?
IF "%1"=="EXTENDED" SET /?
ECHO env var Pfun is '%Pfun%'
IF NOT %SHELL_IS_COMMAND%==1 SET Pfun
IF %SHELL_IS_COMMAND%==1 ECHO %Pfun%
SET Pfun=
SET Pfun=PFUNy
ECHO env var PFUNy is '%Pfun%'
IF NOT %SHELL_IS_COMMAND%==1 SET Pfun
IF %SHELL_IS_COMMAND%==1 ECHO %Pfun%
SET Pfun=
IF %SHELL_IS_COMMAND%==1 GOTO end_Pfun
if %SHELL_IS_CMD%==1 GOTO end_Pfun
SET /U Pfun=PFUNy
ECHO env var PFUNY is '%Pfun%'
ECHO %Pfun%
SET Pfun=
SET /C Pfun=Pfuny
ECHO env var Pfuny is '%Pfun%'
SET Pfun
SET Pfun=
VER /? > _b1.txt
SET /E Pfun=VER /?
ECHO env var Pfun is '%Pfun%'
SET Pfun
SET Pfun=
SET /E Pfun=TYPE _b1.txt
DEL _b1.txt
ECHO env var Pfun is '%Pfun%'
SET Pfun
SET Pfun=
ECHO env var Pfun is '%Pfun%'
SET Pfun
:end_Pfun
IF "%DEV%"=="1" pause

CLS
ECHO.
ECHO Testing running a program where part of the program name is passed in as an argument
CALL %0 tst_arg lvl 3
GOTO skip_tst_arg
:tst_arg
ERR%2 %3
IF ERRORLEVEL == 3 ECHO It Worked!
GOTO END
:skip_tst_arg
ECHO And back
IF "%DEV%"=="1" pause

ECHO.
ECHO BREAK and VERIFY, should should each OFF, ON, OFF again (BREAK not shown for some shells)
BREAK
VERIFY
BREAK ON
BREAK
BREAK OFF
BREAK
VERIFY ON
VERIFY
VERIFY OFF
VERIFY
IF "%1"=="EXTENDED" BREAK /?
IF "%1"=="EXTENDED" VERIFY /?
IF "%DEV%"=="1" pause

ECHO.
ECHO Some builtin variables:
ECHO current path is %CD%
IF "%DEV%"=="1" pause

ECHO.
ECHO test copy to NUL and redirect to NUL
COPY compat.bat NUL > NUL
ECHO nothing should have displayed
IF "%DEV%"=="1" pause


GOTO end_with_funny_prompt
:RESTORE
PROMPT $p$g
GOTO END
:end_with_funny_prompt
ECHO PROMPT is set to vb date vb lt drive gt path == time\n$Run COMPAT RESTORE gtgt
PROMPT $b $d $b $g$h$L$n$g $p $q$q $t$_$v$$Run COMPAT RESTORE $g$g
REM this marks the end of the BATCH file since EXIT will terminate the shell
REM we expect this to fail on MS/PC DOS command.com so don't do it there
IF NOT "%SHELL_IS_COMMAND%"=="1" TITLE Command Prompt
SET SHELL_IS_CMD=
SET SHELL_IS_COMMAND=
SET DEV=
:END
