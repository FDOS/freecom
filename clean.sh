#!/bin/sh
cd suppl
rm -f all_done untar.exe untar.com untar.obj untar.map
rm -f compile.me suppl*.lib suppl*.lst vars.bat *.cfg
cd src && rm -f *.obj objlist.txt echolib.bat *.cfg suppl*.lst
cd ../..
rm -f lastmake.mk context.h_c context.inc strings.h command.com infores info.txt
cd strings    && rm -f strings.h   strings.err strings.dat
cd ../strings && rm -f strings.lib strings.lst strings.log
cd ../criter  && rm -f criter criter1 context.def context.inc context.h_c
cd ../cmd     && rm -f cmds.lib    cmds.lst    cmds.rsp    echolib.bat
cd ../lib     && rm -f freecom.lib freecom.lst freecom.rsp echolib.bat
cd ../shell   && rm -f command.exe command.map command.rsp echoto.bat
cd ../strings && rm -f *.cfg *.obj *.exe
cd ../strings/strings && rm -f strings.lib strings.lst strings.rsp *.c *.obj *.o makefile
cd ../../tools && rm -f tools.mak *.icd *.cfg *.obj *.map *.exe *.com
cd ../utils   && rm -f *.cfg *.obj *.map *.exe
for i in cmd lib shell criter; do cd ../$i && rm -f *.obj *.cfg; done
cd .. && rm -f gnuconf.mak $(find * -name gnumake.mak)
