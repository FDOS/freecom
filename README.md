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
