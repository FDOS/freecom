@echo off
del CONTEXT.H_C
del CONTEXT.INC
del STRINGS.H
del strings\strings.h
del strings\strings.err
del strings\strings.dat
del strings\strings.lib
del criter\criter
del criter\criter1
del criter\context.def
del command.com
del src\command.exe
del lib\freecom.lib
del cmd\cmds.lib
for %%a in (cmd lib src strings criter) do del %%a\*.obj
for %%a in (cmd lib src strings criter) do del %%a\*.cfg
