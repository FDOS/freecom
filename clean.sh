#!/bin/sh
cd suppl
rm -f all_done untar.exe untar.com untar.obj untar.map gnumake.mak
rm -f compile.me suppl*.lib suppl*.lst echoto.bat vars.bat *.cfg
cd src
rm -f *.obj suppllib.rsp echoto.bat echolib.bat *.cfg gnumake.mak suppl*.lib suppl*.lst
cd ../..
rm -f command.com command.upx
rm -f lastmake.mk context.h_c context.inc strings.h infores info.txt
cd strings    && rm -f strings.h strings.err strings.dat strings.log
cd ../criter  && rm -f criter criter1 context.def context.inc context.h_c
cd ../strings/strings && rm -f *.c makefile
cd ../../tools && rm -f makefile.mak *.icd
cd ..
for i in cmd lib strings strings/strings; do rm -f $i/*.lib $i/*.lst; done
for i in cmd lib shell strings strings/strings tools utils utilsc; do
  rm -f $i/gnumake.mak $i/*.obj $i/*.o $i/*.cfg $i/*.map $i/*.exe
  rm -f $i/*.com $i/*.rsp $i/echoto.bat $i/echolib.bat
done
for i in . criter; do rm -f $i/gnumake.mak; done
