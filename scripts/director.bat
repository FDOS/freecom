: $Id$
: Maintainer: Bernd Blaauw, bblnews@hotmail.com
: to use on a cdrom to create localized versions
@echo off
ctty nul
if [%1]==[makesys] goto makesys
@REM Using auxiliary program to get current dir in variable %basepath%, set by prog
@REM basepath.exe, ftp://garbo.uwasa.fi/pc/link/tsbat.zip (freeware, prof Timo Salmi) 
@REM  TODO: also fix case of multiple FreeDOS installations! (easily done by above)

@REM  TODO: fix Xkeyb support.
@REM  TODO: how about mouse support? cdrom support [atapicdd.sys]?

rem Detecting sample config.sys in freedos install destination dir,
rem UPDATE THE FILESIZE NUMBER in line below (4749) if editing sample
rem before distribution!!!!!!!!!!!!!!!!!!!!!!!
if not exist %cdrom%\FREEDOS\INSTALL\BASEPATH.EXE goto end

set destvar=
%cdrom%\FREEDOS\FDOS\BIN\locate.com config.sys /S:4749! /O:&D /N /F:1 /G
%cdrom%\FREEDOS\INSTALL\BASEPATH.EXE CONFIG.SYS
PUSHD
set destvar=%basepath%
rem destvar has value C:\FDOS\ for example, inclusing ending \ !!!
set basepath=
rem %destvar% is the freedos install directory which was created, so alreadyy exists...
if not exist %destvar%nul goto end
set name1=c:\config.sys
set name2=c:\autoexec.bat
set name3=c:\command.com
goto makeconf
for %%x in ( c:\autoexec.bat c:\config.sys c:\command.com ) do if exist %%x goto FDname
goto makeconf

:FDname
set name1=c:\fdconfig.sys
set name2=%destvar%fdauto.bat
set name3=%destvar%bin\command.com
goto makeconf

:makeconf
cls
if exist %name1% echo Skipped updating %name1%
if exist %name1% goto makeauto
echo Creating system configuration file . . .
echo Creating multimenu templates for %name1%

echo MENU 1 - Load FreeDOS and XMS-memory driver                         > %name1%
echo MENU 2 - Load FreeDOS without drivers                              >> %name1%
echo MENUDEFAULT=1,5                                                    >> %name1%
echo 1?DEVICE=%destvar%BIN\FDXXMS.SYS                                   >> %name1%
echo 1?DOS=HIGH                                                         >> %name1%
echo 1?SHELL=%destvar%BIN\CMDXSWP.COM /P=%name2%                        >> %name1%
echo 2?SHELL=%name3% /P=%name2%                                         >> %name1%
echo !LASTDRIVE=Z                                                       >> %name1%
echo ECHO Loading remaining configuration files . . .                   >> %name1%

goto makeauto



:makeauto
if exist %name2% echo Skipped updating %name2%
if exist %name2% %name2%
echo Creating system startup automation file [%name2%]
echo @echo off                                                                          > %name2%
echo set lang=%lang%                                                                   >> %name2%
if not [%key%]==[false] echo set key=%key%                                             >> %name2%
echo set path=C:\;%destvar%;%destvar%bin                                               >> %name2%
type %cdrom%\FREEDOS\INSTALL\autofill.txt>>%name2%
echo echo BootFiles: %name1%, %name2%, %name3%>> %name2%
echo echo.                                                                             >> %name2%
echo echo Welcome to FreeDOS                                                           >> %name2%
rem adding more files
cls

rem 1024 bytes Environment (almost) full, so clearing things....
set name1=
set name2=
set findfile=
for %%x in ( [] [EN] [en] ) do if [%lang%]==%%x goto final
set workdir=%cdrom%\freedos\install\freecom
if not exist %workdir%\strings.%lang% echo Could not find FreeCOM %lang% messagefile for command.com translation.
if not exist %workdir%\strings.%lang% goto final
set copycmd=/y
copy /b %workdir%\command.cln + %workdir%\strings.%lang%  %name3%
copy /b %workdir%\xms_swap.cln + %workdir%\strings.%lang% %destvar%\bin\cmdxswp.com
cls
echo FreeDOS command interpreter succesfully translated to language %lang%
set workdir=
goto final

:final
REM for easy adding to possible bootloader:
REM we first save command.com if it exists, then run SYS with bootsector2file option, then restore the command.com
if exist c:\command.fd8 del c:\command.fd8
if exist c:\fdbtsect.bin del c:\fdbtsect.bin
if exist c:\command.com ren c:\command.com c:\command.fd8
SYS A: C: C:\FDBTSECT.BIN 
if exist c:\command.com del c:\command.com
if exist c:\command.fd8 ren c:\command.fd8 c:\command.com
rem prevent any SYS C: from overwriting the (localized) newest command.com!!!
%cdrom%\freedos\fdos\bin\attrib +r %name3%
cls
alias boot = %cdrom%\freedos\install\director.bat makesys
ctty con
rem Configuring a graphical program like Seal2 is pointless in VMware!
if [%VM%]==[TRUE] goto the_end
if exist %destvar%seal2\install.exe Pause press a key to start manual Seal Desktop install
if exist %destvar%seal2\install.exe %destvar%seal2\install.exe
goto the_end
:the_end
ctty con

echo Following things have just been done during postconfiguration:
echo.
echo * Set up alternate Config.sys file
echo * Set up alternate Autoexec.bat file
echo * Transfer FREEDOS core files, so your harddisk is almost bootable.
echo * Save bootsector in C:\FDBTSECT.BIN for easy adding to a bootmanager 
echo   - like Boot.ini for example
echo * Translate command.com into the language you used during installation.
echo * loading of some drivers and boot-settings.
echo.
echo Please type BOOT if you want to make your harddisk bootable,
echo else reboot the system now.
popd
pushd
goto end

:makesys
rem being called by the alias called BOOT to transfer bootsector and files,
rem without overwriting the created translated command.com file
sys a: c:
if exist c:\command.com attrib -r -s -h c:\command.com
attrib -r -s -h %name3%
ctty con
cls
echo Your system is now bootable. We recommend to reboot the system
goto end

:end
ctty con
