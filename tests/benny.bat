@echo off
: Test of Benny levels

: Run after a program has been invoked, which does set the error level
: to 1==A:, 2==B:, ..., 26==Z: (else: error)

SET drv=
FOR %%Z IN (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) DO IF ERRORLEVEL H%%Z SET drv=%%Z
IF ERRORLEVEL 27 SET drv=
IF "%drv%"=="" ECHO ERROR: No drive letter - Exit code == %ERRORLEVEL%
IF NOT "%drv%"=="" ECHO Drive letter: %drv%:
