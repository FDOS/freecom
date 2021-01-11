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

# Just one for now
export LNG=english

# Output directory
rm -rf _output
mkdir _output

# GCC
git clean -x -d -f -e _output -e _watcom -e ow-snapshot.tar.gz
./build.sh gcc
TGT="_output/gcc/${LNG}"
mkdir -p ${TGT}
mv -i command.com ${TGT}/.

# Watcom
git clean -x -d -f -e _output -e _watcom -e ow-snapshot.tar.gz
./build.sh wc
TGT="_output/wc/${LNG}"
mkdir -p ${TGT}
mv -i command.com ${TGT}/.

echo done
