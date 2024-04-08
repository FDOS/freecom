#!/bin/sh

set -e

if [ ! -z "${TRAVIS_BUILD_DIR}" ] ; then
  CI_BUILD_DIR=${TRAVIS_BUILD_DIR}
else
  CI_BUILD_DIR=$(pwd)
fi
echo CI_BUILD_DIR is \"${CI_BUILD_DIR}\"

OWTAR=ow-snapshot.tar.xz

# Get Watcom compiler
if [ ! -d _watcom ] ; then
  if [ ! -f $OWTAR ] ; then
    echo "Downloading OpenWatcom compiler"
    wget --no-verbose https://github.com/open-watcom/open-watcom-v2/releases/download/2023-02-01-Build/$OWTAR
  fi
  echo "Extracting OpenWatcom compiler"
  mkdir _watcom
  tar -C _watcom -xf $OWTAR
fi
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
  git clean -x -d -f -e _output -e _watcom -e $OWTAR >/dev/null 2>&1
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
  git clean -x -d -f -e _output -e _watcom -e $OWTAR >/dev/null 2>&1
  export LNG=${lng}
  ./build.sh wc
  TGT="_output/wc/${LNG}"
  mkdir -p ${TGT}
  mv -i command.com ${TGT}/.
done

echo done
