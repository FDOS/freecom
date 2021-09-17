@echo off
rem This is part of the FreeCOM FreeDOS package to replace default command.com with language specific version
del command.com
if exist cmd-%lang%.com ren cmd-%lang%.com command.com
del cmd-*.com
