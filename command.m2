command
command.map
$(SUPPL_LIB_PATH)\suppl_s.lib+
$(LIBPATH)\cs.lib
|

infores : config.h command.h command.exe command.map
	utils\mkinfres.exe infores command.map command.exe

command.com : command.exe infores
	copy /b command.exe + infores + criter.mod\criter + criter.mod\criter1 + strings.dat command.com

#		*Compiler Configuration File*
#-h
#-Vf
#-Ff
#-C
#-L$(LIBPATH)

# added strings.h here because $(CFG) is included everywhere already
$(CFG): command.mak strings.h
  copy &&|
-a-
-f-
-ff-
-K
-w+
-O
-Z
-k-
-b-
-I$(INCLUDEPATH)
-D_NO__DOS_DATE
-D_NO__DOS_TIME
| $(CFG)
