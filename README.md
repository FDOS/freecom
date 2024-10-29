# FreeCom - The DOS Command Line Interface #
********************************************

Source code distribution of FreeCOM - a Command line interface
program, which substitutes COMMAND.COM

<!-- Upstream Master -->
<!-- [![Build](https://github.com/FDOS/freecom/workflows/Build/badge.svg)](https://github.com/FDOS/freecom/actions) -->
<!-- Local Branch -->
[![Build](../../workflows/Build/badge.svg)](../../actions)

## About ##
This software is part of the FreeDOS project. 
Please visit the freedos web page at http://www.freedos.org/ for more information about FreeDOS.

FreeCOM implements the command processor for the DOS kernel.  It is usually the first program started by the kernel and provides a command line interface to interactively run other DOS programs.

Additionally, FreeCOM implements a BATCH file processor allowing scripting of commands as well.  See http://help.fdos.org/en/hhstndrd/batch/index.htm for more help with batch commands.


## More information ##
[Changes since last official release](https://github.com/FDOS/freecom/commits/master)

[How to compile FreeCOM](docs/compile.txt)


## Bugs and Help ##

The COMMAND.COM web site (including source hosting) is at

   https://github.com/FDOS/freecom

Bugs may be filed at https://github.com/FDOS/freecom/issues
or you may send any bug reports, comments or other info to the
one of the FreeDOS mailing lists.  For subscription and archives, see:

   http://www.freedos.org/forums/

This software has been developed by many people.  Older changes are listed along with who provided the change within docs/HISTORY.TXT file and more recent changes can be seen with git log (HISTORY.TXT is only updated periodically).

Please note that this software is separate from the DOS-C kernel and
is intended to be compatible with a variety of kernels that implement the DOS API (i.e. it should run on FreeDOS DOS-C kernel and on other DOS kernels such as Microsoft's DOS).

## Installation ##
To use this shell, just run the COMMAND.COM file.
You can also add this to your CONFIG.SYS file:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SHELL=<shell path><shell filename>

e.g.

SHELL=C:\FREEDOS\COMMAND.COM /P
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See http://help.fdos.org/en/hhstndrd/command.htm for description of command line options.


## Current Features ##
- environment handling with prompt and path support
- directory utilities
- command-line history with doskey-like features + filename completion (TAB)
- batch file processing
- input/output redirection and piping
- loadhigh support
- alias support
- flow control (IF, FOR, GOTO, labels)
- support for error and other messages in multiple languages


---

# FreeCOM DBCS edition

a fork of FreeCOM (COMMAND.COM for FreeDOS) to support:

* other platform(s) - like NEC PC-9801 series
* handle DBCS (double-byte character set) parameters correctly
* Japanese message

(yes, all tasks are work-in-progess and unstable...)

DBCS edition works on non-DBCS edition of DOSes (v3.1 or later). 

source repository of FreeCOM DBCS edition:  

https://github.com/lpproj/freecom_dbcs2/ (branch: dbcs)  

original source repository of official FreeCOM:

old http://sourceforge.net/p/freedos/svn/HEAD/tree/freecom/trunk/  
new https://github.com/FDOS/freecom  


## How to build (DBCS edition)

When you want to build freecom_dbcs with OpenWatcom, you need:

* Windows (x86/x64)
* OpenWatcom C++ (I'm using [2.0beta](https://github.com/open-watcom/open-watcom-v2/releases))
* Netwide assembler Win32/Win64 version at https://www.nasm.us/

When you want to build freecom_dbcs with ia16-elf-gcc, you need:

* Linux (i386/amd64) or msys2 on Windows (x86/x64)
* gcc-ia16 at https://github.com/tkchia/build-ia16/
* GNU make, gcc toolchain (for the host environment), nasm, bash

When you want to build freecom_dbcs with Turbo C/C++, you need:

* 100% DOS compatible environment (On DOSBox, you will need to use external command.com because of incompatibility with built-in command processor)
* Borland Turbo C++ 1.01 for IBM PC (I guess you can also build with Turbo C 2.0)
* Netwide assembler (DOS version) - for 16bit DOS platform, latest version is 0.98.39 on http://sourceforge.net/projects/nasm/

Build step is same as the original FreeCOM (copy config.b to config.bat, copy config.std to config.mak, modify config.mak and config.bat as you like, then invoke `build.bat`) but build.bat for DBCS editon has some extra options:

* dbcs - build as DBCS edition
* ibmpc - build for IBM PC platform
* nec98 - build for NEC PC-98x1 platform
* generic - build DOS generic platform (without access of platform specific memory area and BIOS)

Specifying a platform (either ibmpc, nec98 or generic) is needed.

For example, to build FreeCOM DBCS edition for PC-98 with XMS-Swap and Japanese messages on Windows with OpenWatcom, build command is:

  `build -r dbcs nec98 japanese`

To build DOS generic non-DBCS FreeCom without XMS-Swap (and english messages), build command is:

  `build -r generic no-xms-swap`

To build FreeCOM DBCS edition for PC-98 with XMS-Swap and Japanese messages on Linux with gcc-ia16:

  `./build.sh -r dbcs nec98 gcc japanese`


## CREDITS

For DBCS edition, some works of preceders are merged:

* suzu (http://dos.minashiro.net/freecomj.html)
    * codes around DBCS and PC-98
    * Japanese translation (and other contributors: guri, Silphire, etc...)
* akm (http://radioc.web.fc2.com/ , http://diarywind.com/blog/e/commandj-11.html)
    * Japanese translation

