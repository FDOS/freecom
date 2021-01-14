#!/bin/sh

set -e

if [ ! -z "${TRAVIS_BUILD_DIR}" ] ; then
  CI_BUILD_DIR=${TRAVIS_BUILD_DIR}
else
  CI_BUILD_DIR=$(pwd)
fi
echo CI_BUILD_DIR is \"${CI_BUILD_DIR}\"

# Get Watcom compiler
if [ ! -d _watcom ] ; then
  if [ ! -f ow-snapshot.tar.gz ] ; then
    echo "Downloading OpenWatcom compiler snaphot"
    wget --quiet https://github.com/open-watcom/open-watcom-v2/releases/download/Current-build/ow-snapshot.tar.gz
  fi
  echo "Extracting OpenWatcom compiler snaphot"
  mkdir _watcom
  tar -C _watcom -xf ow-snapshot.tar.gz
fi
export PATH=$CI_BUILD_DIR/bin:$PATH:$CI_BUILD_DIR/_watcom/binl64
export WATCOM=$CI_BUILD_DIR/_watcom

# Output directory
rm -rf _output
mkdir _output

# Which ones to build
# serbian disabled, parse error
# yu437 disabled, parse error
LANGUAGES="english dutch finnish french german italian polish pt_br russian slovene spanish swedish turkish ukr"

# GCC
for lng in ${LANGUAGES} ; do
  # Do full clean for rebuild of each language
  echo "Do full clean"
  git clean -x -d -f -e _output -e _watcom -e ow-snapshot.tar.gz >/dev/null 2>&1
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
  git clean -x -d -f -e _output -e _watcom -e ow-snapshot.tar.gz >/dev/null 2>&1
  export LNG=${lng}
  ./build.sh wc
  TGT="_output/wc/${LNG}"
  mkdir -p ${TGT}
  mv -i command.com ${TGT}/.
done

echo done
