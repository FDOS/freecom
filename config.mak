## is the base path ("root") of Turbo C++ v1.01
CC_BASE_PATH = D:\TC101

## Where the pre-compiled SUPPL files are located
## See DOCS\SUPPL.TXT for more information about this library
SUPPL_INC_PATH = .\SUPPL
SUPPL_LIB_PATH = $(SUPPL_INC_PATH)

## Which programs to use
CC = $(CC_BASE_PATH)\TCC +$(CFG)
TASM = TASM
TLIB = $(CC_BASE_PATH)\Tlib
TLINK = $(CC_BASE_PATH)\Tlink
LIBPATH = $(CC_BASE_PATH)\LIB
INCLUDEPATH = $(CC_BASE_PATH)\INCLUDE;$(SUPPL_INC_PATH)
