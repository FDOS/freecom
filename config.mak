## is the base path ("root") of Turbo C++ v1.01
CC_BASE_PATH = D:\TC101

## Where the pre-compiled SUPPL files are located
## See DOCS\SUPPL.TXT for more information about this library
SUPPL_INC_PATH = D:\FREEDOS\SRC\INCLUDE
SUPPL_LIB_PATH = D:\FREEDOS\SRC\LIB\TC101

## Program locations
BINPATH = $(CC_BASE_PATH)\BIN
LIBPATH = $(CC_BASE_PATH)\LIB
INCLUDEPATH = $(CC_BASE_PATH)\INCLUDE;$(SUPPL_INC_PATH)

## Which programs to use
CC = $(BINPATH)\TCC +$(CFG)
AR = $(BINPATH)\Tlib
LD = $(BINPATH)\Tlink
NASM = nasm.exe

## Add -DDEBUG=1 below to enable debug support for assembly files
NASMFLAGS =
## Add -DDEBUG=1 below to enable FreeCOM-debug support for C files
## Add -DNDEBUG=1 below to disable any debug (even assert)
CFLAGS =

## Memory model of FreeCOM
SHELL_MMODEL=s

# Default configuration
# added strings.h here because $(CFG) is included everywhere already
## Add -D_NO__DOS_DATE if your compiler does not have no dosdate_t (*)
## Add -D_NO__DOS_TIME if your compiler does not have no dostime_t (*)
## Add -D_NO_FMEMCHR if your compiler does not have no _fmemchr() (*)
## Add -D_NO_FMEMCMP if your compiler does not have no _fmemcmp() (*)
##
## (*) Note: Should be detected by CONFIG.H automatically for Turbo C
##     and Borland C compilers.
##
$(CFG): $(CFG_DEPENDENCIES)
  copy &&|
-a
-f-
-ff-
-K
-w+
-O
-Z
-b-
-I$(INCLUDEPATH)
-L$(LIBPATH)
-m$(SHELL_MMODEL)
| $(CFG)


#-k-

#		*Implicit Rules*
.c.exe:
  $(CC) $< 
.c.obj:
  $(CC) -c {$< }
.asm.obj:
	$(NASM) $(NASMFLAGS) -f obj -DMODEL=$(SHELL_MMODEL) $<
