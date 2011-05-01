@echo off
:-****************************************************************
:-  NOTICE!  You can edit but then copy this file to CONFIG.BAT! *
:-****************************************************************

:-**********************************************************************
:-- define NASM executable
:-**********************************************************************

set XNASM=nasm

:**********************************************************************
:- define your COMPILER type here, pick one of them
:**********************************************************************

:- Open Watcom C
:- set COMPILER=WATCOM
:- Turbo C 2.01
:- set COMPILER=TC2
:- Turbo C++ 1.01
set COMPILER=TURBOCPP
:- Borland C
:- set COMPILER=BC5

:-**********************************************************************
:-- where is the BASE dir of your compiler(s) ??
:-**********************************************************************
                                                
:- set TC2_BASE=c:\tc201
set TP1_BASE=c:\tcpp
:- set BC5_BASE=c:\bc5

:- if WATCOM maybe you need to set your WATCOM environment variables 
:- and path
:- if not \%WATCOM% == \ goto watcom_defined
:- set WATCOM=c:\watcom
:- set PATH=%PATH%;%WATCOM%\binw
:watcom_defined
