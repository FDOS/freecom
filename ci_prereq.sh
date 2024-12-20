#!/bin/sh

set -e

sudo add-apt-repository ppa:tkchia/build-ia16
sudo apt update

# Common tools
sudo apt install nasm upx

# GCC-IA16 for Linux
sudo apt install gcc-ia16-elf libi86-ia16-elf

# Watcom for Linux
OWTAR=ow-snapshot.tar.xz
if [ ! -d _watcom ] ; then
  if [ ! -f $OWTAR ] ; then
    echo "Downloading OpenWatcom compiler"
    wget --no-verbose https://github.com/open-watcom/open-watcom-v2/releases/download/2023-02-01-Build/$OWTAR
  fi
  echo "Extracting OpenWatcom compiler"
  mkdir _watcom
  tar -C _watcom -xf $OWTAR
fi
