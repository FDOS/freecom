@echo off
cd strings
fixstrs
copy strings.dat ..
del strings.dat
copy strings.h ..
del strings.h
cd ..
