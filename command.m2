command
		# no map file
$(SUPPL_LIB_PATH)\suppl_s.lib+
$(LIBPATH)\cs.lib
|

command.com : command.exe
	copy /b command.exe + criter.mod\criter + criter.mod\criter1 + strings.dat command.com

#		*Compiler Configuration File*
#-h
#-Vf
#-Ff
#-C
#-L$(LIBPATH)

# added strings.h here because $(CFG) is included everywhere already
$(CFG): command.mak strings.h
  copy &&|
-a
-f-
-ff-
-K
-w+
-O
-Z
-k-
-d
-b-
-I$(INCLUDEPATH)
-D_NO__DOS_DATE
-D_NO__DOS_TIME
| $(CFG)
