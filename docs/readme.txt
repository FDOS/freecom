FreeCom Command Line Interface
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
About
~~~~~
This software is part of the FreeDOS project. Please email
freedos@sunsite.unc.edu for more information, or visit the freedos
archive at "ftp://sunsite.unc.edu/pub/micro/pc-stuff/freedos".  Also,
visit our web page at http://www.freedos.org/.

The command.com web site is at

   http://www.gcfl.net/FreeCom

Please send any bug reports, comments or other info to:

   fd-command@topica.com  (command.com development mailing list)

This software has been developed by the following people:
(listed in approximate chronological order of contributions)

   normat@rpi.edu (Tim Norman)
   mrains@apanix.apana.org.au (Matt Rains)
   ejeffrey@iastate.edu (Evan Jeffrey)
   ska-src@gmx.net (Steffen Kaiser (ska))
   Svante Frey (sfrey@kuai.se)
   Oliver Mueller (ogmueller@t-online.de)
   Aaron Kaufman (morgan@remarque.berkeley.edu)
   Marc Desrochers (bitzero@hotmail.com)
   Rob Lake (rlake@cs.mun.ca)
   John P. Price <linux-guru@gcfl.net>
   Hans B Pufal <hansp@digiweb.com>
	D. Lucas Parker <fuerve@yahoo.com>
	Henry Voigt <Henry.Voigt@gmx.de>
	James Tabor <jimtabor@infohwy.com>

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
environment handling with prompt and path support.
directory utilities.
command-line history with doskey-like features.
batch file processing
input/output redirection and piping
loadhigh support
alias support
filename completion (use TAB)
