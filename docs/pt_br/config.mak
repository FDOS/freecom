## Diretório base do Turbo C++ v1.01
CC_BASE_PATH = D:\TC101

## Onde os arquivo SUPPL(suplementares) pre-compilados estão
## Veja DOCS\SUPPL.TXT para informações sobre ela
SUPPL_INC_PATH = .\SUPPL
SUPPL_LIB_PATH = $(SUPPL_INC_PATH)

## Localização dos programas
BINPATH = $(CC_BASE_PATH)\BIN
LIBPATH = $(CC_BASE_PATH)\LIB
INCLUDEPATH = $(CC_BASE_PATH)\INCLUDE;$(SUPPL_INC_PATH)

## Que programas usar
CC = $(BINPATH)\TCC +$(CFG)
AR = $(BINPATH)\Tlib
LD = $(BINPATH)\Tlink
NASM = nasm.exe

## Adicione -DDEBUG=1 a seguir para ativar o suporte a depuração para arquivos assembly
NASMFLAGS =
## Adicione -DDEBUG=1 a seguir para ativar o suporte a depuração para arquivos C
## Adicione -DNDEBUG=1 a seguir para desativar qualquer suporte a depuração
CFLAGS =

## Modelo de memoria do FreeCOM
SHELL_MMODEL=s

!if $(XMS_SWAP)
__XMS_SWAP = -DXMS_SWAP=1
!endif

# Configuração padrão
# adicionado strings.h aqui por que $(CFG) já está incluido em tudo
## Adicione -D_NO__DOS_DATE se seu compilador não tem o dosdate_t (*)
## Adicione -D_NO__DOS_TIME se seu compilador não tem o dostime_t (*)
## Adicione -D_NO_FMEMCHR se seu compilador não tem o _fmemchr() (*)
## Adicione -D_NO_FMEMCMP se seu compilador não tem o _fmemcmp() (*)
##
## (*) Nota: Deve ser detectado pelo CONFIG.H automaticamente pelo Turbo C
##     e compiladores Borland C.
##
$(CFG): $(CFG_DEPENDENCIES)
  copy &&|
-1-
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
-DNDEBUG
-UDEBUG
$(__XMS_SWAP)
| $(CFG)


#-k-

#		*Regras Implicitas*
.c.exe:
  $(CC) $< 
.c.obj:
  $(CC) -c {$< }
.asm.obj:
	$(NASM) $(NASMFLAGS) -f obj -DMODEL=$(SHELL_MMODEL) $<
