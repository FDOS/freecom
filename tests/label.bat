@echo off
: Test if all the labels work

: Usual spelling
goto label1

:label1
echo Label 1 found

: Prefixed colon
goto :label2

:label2
echo Label 2 found

: Prefixed colon seperated by whitespaces
: Though 4dos searches for ' label3' here?
goto : label3

:label3
echo Label 3 found


