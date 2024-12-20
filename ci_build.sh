#!/bin/sh

set -e

if [ ! -z "${TRAVIS_BUILD_DIR}" ] ; then
  CI_BUILD_DIR=${TRAVIS_BUILD_DIR}
else
  CI_BUILD_DIR=$(pwd)
fi
echo CI_BUILD_DIR is \"${CI_BUILD_DIR}\"

OWTAR=ow-snapshot.tar.xz

export PATH=$CI_BUILD_DIR/bin:$PATH:$CI_BUILD_DIR/_watcom/binl64
export WATCOM=$CI_BUILD_DIR/_watcom

# Output directory
rm -rf _output
mkdir _output

# Which ones to build
LANGUAGES="english dutch finnish french german italian polish pt pt_br russian serbian slovene spanish swedish turkish ukr yu437"

# GCC
for lng in ${LANGUAGES} ; do
  # Do full clean for rebuild of each language
  echo "Do full clean"
  git clean -q -x -d -f -e _output -e _watcom -e $OWTAR
  export LNG=${lng}
  ./build.sh gcc
  TGT="_output/gcc/${LNG}"
  mkdir -p ${TGT}
  mv -i command.com ${TGT}/.
done

# Watcom
for lng in ${LANGUAGES} ; do
  # Do full clean for rebuild of each language
  echo "Do full clean"
  git clean -q -x -d -f -e _output -e _watcom -e $OWTAR
  export LNG=${lng}
  ./build.sh wc
  TGT="_output/wc/${LNG}"
  mkdir -p ${TGT}
  mv -i command.com ${TGT}/.
done

# Watcom (DOS) (slow so just English)
mkdir -p _output/wc_dos/english
git clean -q -x -d -f -e _output -e _watcom -e $OWTAR
{
  echo set COMPILER=WATCOM
  echo set WATCOM='C:\\devel\\watcomc'
  echo set MAKE=wmake /ms /h /f
  echo set XNASM='C:\\devel\\nasm\\nasm'
  echo set XUPX=upx --8086 --best
  echo set OLDPATH=%PATH%
  echo set PATH='%WATCOM%\\binw;C:\\bin;%OLDPATH%'
  echo set DOS4G=QUIET
} | unix2dos > config.bat

cp config.std config.mak

dosemu -td -q -K . -E "build.bat wc"
mv -i command.com _output/wc_dos/english/.

echo done
