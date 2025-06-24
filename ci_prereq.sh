#!/bin/sh

set -e

sudo add-apt-repository -y ppa:dosemu2/ppa
sudo add-apt-repository ppa:tkchia/build-ia16
sudo apt update

# Common tools
sudo apt install nasm upx
sudo apt install dosemu2 dos2unix

# GCC-IA16 for Linux
sudo apt install gcc-ia16-elf libi86-ia16-elf

# Tools for DOS builds
mkdir -p _downloads
mkdir -p _watcom
cd _downloads

HERE=$(pwd)

# download and unpack Open Watcom snapshot
[ -f ow-snapshot.tar.xz ] || wget --no-verbose https://github.com/open-watcom/open-watcom-v2/releases/download/Current-build/ow-snapshot.tar.xz
tar -C ../_watcom -xf ow-snapshot.tar.xz

#IBIBLIO_PATH='http://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/distributions/1.2/repos'
IBIBLIO_PATH='https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/1.3'

BASE=${IBIBLIO_PATH}/base

#    get FreeDOS kernel
[ -f kernel.zip ] || wget --no-verbose ${BASE}/kernel.zip

#    get FreeCOM
[ -f freecom.zip ] || wget --no-verbose ${BASE}/freecom.zip

DEVEL=${IBIBLIO_PATH}/devel

#    get gnumake for DOS
[ -f djgpp_mk.zip ] || wget --no-verbose ${DEVEL}/djgpp_mk.zip

#    get nasm for DOS
[ -f nasm.zip ] || wget --no-verbose ${DEVEL}/nasm.zip

#    get upx for DOS
[ -f upx.zip ] || wget --no-verbose ${DEVEL}/upx.zip

#    get Watcom for DOS
[ -f watcomc.zip ] || wget --no-verbose ${DEVEL}/watcomc.zip

#    get Turbo C 2.01 (maybe encrypted) tar file
if [ -n "${TC201_ARCHIVE_FILENAME}" ] && [ ! -f ${TC201_ARCHIVE_FILENAME} ] ; then
   echo "Downloading Turbo C 2.01"
   wget --no-verbose ${TC201_ARCHIVE_PATHNAME}/${TC201_ARCHIVE_FILENAME}
fi

# Unpack the DOS binaries
mkdir -p ${HOME}/.dosemu/drive_c
cd ${HOME}/.dosemu/drive_c && (

  mkdir -p bin

  # Boot files
  unzip -LL -q ${HERE}/kernel.zip
  cp -p bin/kernl386.sys ./kernel.sys
  unzip -LL -q ${HERE}/freecom.zip
  cp -p bin/command.com ./command.com
  cp -p /usr/share/dosemu/dosemu2-cmds-0.3/c/fdconfig.sys .

  # Development files
  unzip -LL -q ${HERE}/djgpp_mk.zip
  cp -p devel/djgpp/bin/make.exe bin/.
  unzip -LL -q ${HERE}/upx.zip
  cp -p devel/upx/upx.exe bin/.
  echo PATH to make and upx binaries is 'c:/bin'

  unzip -LL -q ${HERE}/nasm.zip
  echo PATH to nasm binary is 'c:/devel/nasm'

  unzip -LL -q ${HERE}/watcomc.zip
  echo PATH to watcom binaries is 'c:/devel/watcomc/binw'

  # Turbo C
  if [ -f ${HERE}/${TC201_ARCHIVE_FILENAME} ] && [ -n "${TC201_ARCHIVE_PASSPHRASE}" ] ; then
    echo Decrypting and unpacking Turbo C 2.01
    echo "${TC201_ARCHIVE_PASSPHRASE}" | gpg --decrypt --batch --passphrase-fd 0 ${HERE}/${TC201_ARCHIVE_FILENAME} | tar -jxf -
  elif [ -f ${HERE}/${TC201_ARCHIVE_FILENAME} ] ; then
    echo Unpacking Turbo C 2.01
    tar -jxf ${HERE}/${TC201_ARCHIVE_FILENAME}
  else
    echo No Turbo C 2.01 archive available
  fi
)
