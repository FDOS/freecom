#!/bin/bash

set -e

export SWAP=YES-DXMS-SWAP____________________
# BEGIN Internal stuff for ska -- If one of these three commands
#       fail for you, your distribution is broken! Please report.
for a in lib/lib.mak cmd/cmd.mak shell/command.mak; do if [ ! -f $a ]; then export SWAP=NO; fi; done
if [ "$SWAP" == "NO" ]; then
    unset XMS_SWAP
    dmake dist
fi
# end
export SWAP=

if [ ! -f config.mak ]; then
  echo Please copy config.std into config.mak and update the
  echo settings therein.
  exit 1
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

while (( "$#" )); do
  case "$1" in
    -h)
	echo Build FreeCOM
	echo Usage: %0 [-r] [clean] [xms-swap] [debug] [language]
	echo -r: Rebuild -- Clean before proceed
	echo clean: Remove *.OBJ, *.COM, *.LIB, etc. files, then exit
	echo xms-swap: Build FreeCOM with XMS-Only Swap support
	echo debug: Build FreeCOM with debug settings.
	echo You can select for which language to built FreeCOM by setting
	echo the environment variable LNG before running this script, e.g.:
	echo EXPORT LNG=german
	echo selects the German language. For available language see STRINGS\*.LNG
	exit 0
	;;
    xms-swap)
	export XMS_SWAP=1
	;;
    debug)
	export DEBUG=1
	;;
    wc)
	export COMPILER=watcom
	;;
    gcc)
	export COMPILER=gcc
	;;
    *)
	break
	;;
  esac
  shift
done


if [ -n "$1" ]; then export LNG=$1; fi
if [ -z "$LNG" ]; then export LNG=ENGLISH; fi
echo Building FreeCOM for language $LNG

if [ -z "$MAKE" ]; then
    case "$COMPILER" in
	watcom)
	    export MAKE="wmake -ms -h -f"
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
    sed -e 's@^!@@' \
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
    for i in suppl utils strings criter lib cmd; do
	gnumake_subst $i $i.mak gnumake.mak
    done
    gnumake_subst suppl/src suppl.mak gnumake.mak
    gnumake_subst strings/strings strings.mak gnumake.mak
    gnumake_subst shell command.mak gnumake.mak
fi

echo
echo Checking SUPPL library
cd suppl
if [ ! -f skip ]; then
    echo Building SUPPL library
    $MAKE suppl.mak all

    cd src
    $MAKE suppl.mak all
    cd ..
fi
cd ..

echo
echo Making basic utilities for build process
echo
cd utils
$MAKE utils.mak all
cd ..

echo
echo Making STRINGS resource
echo
cd strings
$MAKE strings.mak all
cd strings
$MAKE strings.mak all
cd ../..

echo
echo Making CRITER resource
echo
cd criter
$MAKE criter.mak all
cd ..

echo
echo Making misc library
echo
cd lib
$MAKE lib.mak all
cd ..

echo
echo Making commands library
echo
cd cmd
$MAKE cmd.mak all
cd ..

echo
echo Making COMMAND.COM
echo
cd shell
$MAKE command.mak all
cd ..

utils/mkinfres.exe /tinfo.txt infores shell/command.map shell/command.exe
cat shell/command.exe infores criter/criter1 criter/criter strings/strings.dat > command.com
[ -f command.com ] || exit 1

echo
echo Making supplemental tools
echo
cd tools
cat tools.m1 > tools.mak
../utils/mktools.exe >>tools.mak
cat tools.m2 >>tools.mak
if $MAKE -? 2>&1 | grep -q gnu; then
    gnumake_subst . tools.mak gnumake.mak
fi
$MAKE tools.mak all
cd ..

echo
echo Patching heap size to 6KB
echo
tools/ptchsize.exe command.com +6KB

echo
echo All done. COMMAND.COM is ready for usage!
echo
if [ -z "$XMS_SWAP" ]; then
  echo Note: To build the XMS-Only Swap featured FreeCOM, re-run
  echo ./build.sh -r xms-swap $LNG
fi
