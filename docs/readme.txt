FreeCom Command Line Interface
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
About
~~~~~
This software is part of the FreeDOS project. Please email
freedos@sunsite.unc.edu for more information, or visit the freedos
archive at "ftp://sunsite.unc.edu/pub/micro/pc-stuff/freedos".  Also,
visit our web page at http://www.freedos.org/.

The command.com web site is at

   http://freedos.sourceforge.net/freecom

Please send any bug reports, comments or other info to:

   freedos-freecom@lists.sourceforge.net   (command.com development mailing list)

This software has been developed by many people, which are enlisted
within HISTORY.TXT now.

Please note that this software is separate from the DOS-C kernel and
may or may not work properly under it.  Of course I am making every
effort to make it compatible.

Installation
~~~~~~~~~~~~
To use this shell, just run the COMMAND.COM file.
You can also add this to your CONFIG.SYS file:

SHELL=<shell path><shell filename>

e.g.

SHELL=C:\FREEDOS\COMMAND.COM /P

Current Features
~~~~~~~~~~~~~~~~
- environment handling with prompt and path support
- directory utilities
- command-line history with doskey-like features + filename completion (TAB)
- batch file processing
- input/output redirection and piping
- loadhigh support
- alias support
- flow control (IF, FOR, GOTO, labels)
- non-complete multi language support
