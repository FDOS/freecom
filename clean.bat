@echo off
del CONTEXT.H_C
del CONTEXT.INC
del STRINGS.H
del strings\strings.h
del strings\strings.err
del strings\strings.dat
del strings\strings.lib
del strings\strings.log
del criter\criter
del criter\criter1
del criter\context.def
del criter\context.inc
del criter\context.h_c
del command.com
del shell\command.exe
del lib\freecom.lib
del cmd\cmds.lib
del tools\*.icd
del tools\*.exe
del tools\*.com
del utils\*.exe
for %%a in (cmd lib shell strings criter tools utils) do del %%a\*.obj
for %%a in (cmd lib shell strings criter tools utils) do del %%a\*.cfg
