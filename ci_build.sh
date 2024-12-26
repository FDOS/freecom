#!/bin/sh

set -e

if [ ! -z "${TRAVIS_BUILD_DIR}" ] ; then
  CI_BUILD_DIR=${TRAVIS_BUILD_DIR}
else
  CI_BUILD_DIR=$(pwd)
fi
echo CI_BUILD_DIR is \"${CI_BUILD_DIR}\"

# Open Watcom Environment Setup
export WATCOM=$CI_BUILD_DIR/_watcom
export PATH=$CI_BUILD_DIR/bin:$PATH:$WATCOM/binl64

# Output directory
rm -rf _output
mkdir _output

# Which ones to build
LANGUAGES="english dutch finnish french german italian polish pt pt_br russian serbian slovene spanish swedish turkish ukr yu437"

# GCC
for lng in ${LANGUAGES} ; do
  # Do full clean for rebuild of each language
  echo "Do full clean"
  git clean -q -x -d -f -e _output -e _watcom -e _download
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
  git clean -q -x -d -f -e _output -e _watcom -e _download
  export LNG=${lng}
  ./build.sh wc
  TGT="_output/wc/${LNG}"
  mkdir -p ${TGT}
  mv -i command.com ${TGT}/.
done

# Let's use the just compiled command.com for the DOS builds and get some
# testing of it for free. Note case insensitive target in case the freecom
# package ever has an upper or mixed case file.
echo Copying the GCC Linux built command.com to use for the DOS builds
cp -v _output/gcc/english/command.com ${HOME}/.dosemu/drive_c/[Cc][Oo][Mm][Mm][Aa][Nn][Dd].[Cc][Oo][Mm]

# Watcom (DOS) (slow so just English)
mkdir -p _output/wc_dos/english
git clean -q -x -d -f -e _output -e _watcom -e _download
{
  echo set COMPILER=WATCOM
  echo set WATCOM='C:\\devel\\watcomc'
  echo set MAKE=wmake /ms /h /f makefile.mak
  echo set XNASM='C:\\devel\\nasm\\nasm'
  echo set XUPX=upx --8086 --best
  echo set OLDPATH=%PATH%
  echo set PATH='%WATCOM%\\binw;C:\\bin;%OLDPATH%'
  echo set DOS4G=QUIET
} | unix2dos > config.bat

cp config.std config.mak

dosemu -td -q -K . -E "build.bat wc"
mv -i command.com _output/wc_dos/english/.

# Turbo C 2.01 (DOS) (slow so just Spanish)
if [ -d ${HOME}/.dosemu/drive_c/tc201 ] ; then
  export LNG=spanish

  mkdir -p _output/tc2_dos/${LNG}
  git clean -q -x -d -f -e _output -e _watcom -e _downloads
  {
    echo set LNG=${LNG}

    echo set COMPILER=TC2
    echo set TC2_BASE='C:\\tc201'
    echo set XNASM=nasm
    echo set XUPX=upx --8086 --best
    echo set OLDPATH=%PATH%
    echo set PATH='%TC2_BASE%;C:\\devel\\nasm;C:\\bin;%WATCOM%\\binw;%OLDPATH%'
  } | unix2dos > config.bat

  cp config.std config.mak

  dosemu -td -q -K . -E "build.bat"
  mv -i command.com _output/tc2_dos/${LNG}/.
fi

echo done
