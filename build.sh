#!/usr/bin/env bash

set -e

WITH_UPX="no"
SED=sed
#workaround for Windows (set to binary mode)
if [ "$(expr substr $(uname -s) 1 5)" == 'MINGW' ]; then
    SED='sed -b'
fi


export SWAP=YES-DXMS-SWAP____________________
# BEGIN Internal stuff for ska -- If one of these three commands
#       fail for you, your distribution is broken! Please report.
for a in lib/makefile.mak cmd/makefile.mak shell/makefile.mak; do if [ ! -f $a ]; then export SWAP=NO; fi; done
if [ "$SWAP" == "NO" ]; then
    unset XMS_SWAP
    dmake dist
fi
# end
export SWAP=

if [ ! -f config.mak ]; then
  echo File config.mak missing, copying config.std to it
  cp config.std config.mak || exit 1
fi

export XNASM=nasm
export COMPILER=watcom
if [ -z "$WATCOM" ]; then
  export WATCOM=$HOME/watcom
  export PATH=$PATH:$WATCOM/binl
fi
export PATH=$PATH:.

if [ -f lastmake.mk ] || [ "$1" == "-r" ]; then ./clean.sh; fi
if [ "$1" == "-r" ]; then ./clean.sh; shift; fi
if [ "$1" == "clean" ]; then ./clean.sh; exit 0; fi

export XMS_SWAP=1
while (( "$#" )); do
  case "$1" in
    -h)
	echo Build FreeCOM
	echo Usage: %0 [-r] [clean] [no-xms-swap] [debug] [language]
	echo -r: Rebuild -- Clean before proceed
	echo clean: Remove *.OBJ, *.COM, *.LIB, etc. files, then exit
	echo no-xms-swap: Build FreeCOM without XMS-Only Swap support
	echo debug: Build FreeCOM with debug settings.
	echo You can select for which language to built FreeCOM by setting
	echo the environment variable LNG before running this script, e.g.:
	echo EXPORT LNG=german
	echo selects the German language. For available language see STRINGS\*.LNG
	exit 0
	;;
    no-xms-swap)
	unset XMS_SWAP
	;;
    debug)
	export DEBUG=1
	;;
    watcom)
	export COMPILER=watcom
	;;
    wc)
	export COMPILER=watcom
	;;
    gcc)
	export COMPILER=gcc
	;;
    upx)
	WITH_UPX="yes"
	;;
    *)
	break
	;;
  esac
  shift
done


if [ -n "$1" ]; then export LNG=$1; fi
if [ -z "$LNG" ]; then export LNG=english; fi
echo Building FreeCOM for language $LNG

if [ -z "$MAKE" ]; then
    case "$COMPILER" in
	watcom)
	    export MAKE="wmake -ms -h -f makefile.mak"
	    ;;
	gcc)
	    export MAKE="make -f gnumake.mak"
	    ;;
	*)
	    ;;
    esac
    echo Make is $MAKE.
fi

# substitutions for GNU Make

gnumake_subst () {
    $SED -e 's@^!@@' \
	-e 's@^include "\(.*\)"@include \1@' \
	-e 's@^include $(TOP)/config.mak@include $(TOP)/gnuconf.mak@' \
	-e 's@if \(.*\) == \(.*\)[\r$]@ifeq (\1,\2)@' \
	-e 's@^CC =@CC :=@' \
	-e 's@^INCLUDEPATH =@INCLUDEPATH :=@' \
	-e 's/\(-f obj.*$<\)/\1 -o $@/' \
	 < $1/$2 > $1/$3
}

if $MAKE -? 2>&1 | grep -q gnu; then
    gnumake_subst . config.mak gnuconf.mak
    for i in suppl utils utilsc strings criter lib cmd; do
	gnumake_subst $i makefile.mak gnumake.mak
    done
    gnumake_subst suppl/src makefile.mak gnumake.mak
    gnumake_subst strings/strings makefile.mak gnumake.mak
    gnumake_subst shell makefile.mak gnumake.mak
fi

echo
echo Checking SUPPL library
cd suppl
if [ ! -f skip ]; then
    echo Building SUPPL library
    $MAKE all

    cd src
    $MAKE all
    cd ..
fi
cd ..

echo
echo Making basic utilities for build process
echo
cd utils
$MAKE all
cd ../utilsc
$MAKE all
cd ..

echo
echo Making STRINGS resource
echo
cd strings
$MAKE all
cd strings
$MAKE all
cd ../..

echo
echo Making CRITER resource
echo
cd criter
$MAKE all
cd ..

echo
echo Making misc library
echo
cd lib
$MAKE all
cd ..

echo
echo Making commands library
echo
cd cmd
$MAKE all
cd ..

echo
echo Making COMMAND.COM
echo
cd shell
$MAKE all
cd ..

utils/mkinfres.exe -Tinfo.txt infores shell/command.map shell/command.exe
cat shell/command.exe infores criter/criter1 criter/criter strings/strings.dat > command.com
[ -f command.com ] || exit 1

echo
echo Making supplemental tools
echo
cd tools
cat tools.m1 > makefile.mak
../utils/mktools.exe >>makefile.mak
cat tools.m2 >>makefile.mak
if $MAKE -? 2>&1 | grep -q gnu; then
    gnumake_subst . makefile.mak gnumake.mak
fi
$MAKE all
cd ..

echo
echo Patching heap size to 6KB
echo
utils/ptchsize.exe command.com +6KB

if [ $WITH_UPX = "yes" ]; then
  rm -f command.upx
  upx --8086 --best -o command.upx command.com
fi

echo
echo All done. COMMAND.COM is ready for use!
echo
if [ -z "$XMS_SWAP" ]; then
  echo Note: To build FreeCOM without XMS-Only Swap, re-run
  echo ./build.sh -r no-xms-swap $LNG
fi
