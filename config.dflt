## is the base path ("root") of Turbo C++ v1.01
CC_BASE_PATH = D:\TC101

## Where the pre-compiled SUPPL files are located
## See DOCS\SUPPL.TXT for more information about this library
SUPPL_INC_PATH = .\SUPPL
SUPPL_LIB_PATH = $(SUPPL_INC_PATH)

## Program locations
BINPATH = $(CC_BASE_PATH)\BIN
LIBPATH = $(CC_BASE_PATH)\LIB
INCLUDEPATH = $(CC_BASE_PATH)\INCLUDE;$(SUPPL_INC_PATH)

## Which programs to use
CC = $(BINPATH)\TCC +$(CFG)
TLIB = $(BINPATH)\Tlib
TLINK = $(BINPATH)\Tlink
NASM = nasm.exe

## Add -DDEBUG=1 below to enable debug support for assembly files
NASMFLAGS =
## Add -DDEBUG=1 below to enable FreeCOM-debug support for C files
## Add -DNDEBUG=1 below to disable any debug (even assert)
CFLAGS =

# Default configuration
# added strings.h here because $(CFG) is included everywhere already
$(CFG): $(CFG_DEPENDENCIES)
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
