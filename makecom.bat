:This batch file takes the command.exe and appends the strings.dat
:file, and creates command.com

copy /b command.exe+strings.dat command.com
del command.exe
