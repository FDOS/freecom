/*
 * config.h - Usado para configurar o que ser� compilado para interpretador.
 *
 */

/* Defina DEBUG para adicionar c�digo de  add depura��o */
#ifndef DEBUG			/* possivelmente j� definido via linha de comando */
/*
  #define DEBUG
*/
#endif

/* Defina para ativar o comando alias, e atalhos. */
#define FEATURE_ALIASES
#define ALIAS_DEFAULT_SIZE 1024

/* Defina para ativar entrada de dados avan�ada (pre-requisito de Hist�rico e complementa��o 
	nome de arquivo  */
#define FEATURE_ENHANCED_INPUT

/* Defina ativar hist�rico(DOSKEY); requer: Entrada de dados avan�ada */
#define FEATURE_HISTORY
#define HISTORY_DEFAULT_SIZE 256

/* Defina para ativar complementa��o de nome de arquivo; requer: Entrada de dados avan�ada */
#define FEATURE_FILENAME_COMPLETION

/* Defina para ativar para carregar menssagens na memoria */
#define FEATURE_LOAD_MESSAGES

/* Defina para ativar DOS NLS */
#define FEATURE_NLS

/* Defina para ativar teste das teclas F5/F8 na inicializa��o se /P estiver presente na
	linha de comando */
/* #define FEATURE_BOOT_KEYS */

/* LOG de caracter�sticas da linha de comando */
/* #define FEATURE_CALL_LOGGING */

/* Preserva o �ltimo diretorio (CD, CHDIR, CDD, PUSHD, POPD);
	"CD -" l� est� chdir */
#define FEATURE_LAST_DIR

/* Ativa o suporte para instala��o de exten��es de COMANDO (MUX-AE)
 */
#define FEATURE_INSTALLABLE_COMMANDS

/* Nome do execut�vel */
#define COM_NAME "COMMAND.COM"
/* Nome padr�o do AUTOEXEC.BAT */
#define AUTO_EXEC "\\autoexec.bat"
/* Arquivo de LOG para registrar algumas caracteristicas; PRECISA estar altamente-qualificado! */
#define LOG_FILE "C:\\FreeCom.log"

/* Define o numero de loops quando redirecionar para o CON se o
	interpretados ficar em um loop - "hangForEver()" - para sempre
   N�o defina para remover esta caracter�stica */
#define FEATURE_AUTO_REDIRECT_TO_CON 5

/* Quantos arquivo de lote devem ser aninhados minimamente */
#define BATCH_NESTLEVEL_MIN 5

/* Defina para adicionar suporte ao swapout do FreeCOM
	see DOCS\K-SWAP.TXT
*/
#define FEATURE_KERNEL_SWAP_SHELL

/* Defina para adicionar suportr para troca XMS-only do FreeCOM
	Est� defini��o � incompat�vel com a defini��o acima.
*/
/* #define FEATURE_XMS_SWAP */

/* Define o tamanho do buffer usado para guardar todos os caminhos antigos para o PUSHD/POPD */
#define DIRSTACK_DEFAULT_SIZE 256

/* Use isto se um programa externo foi terminado por
	^C or ^Break */
#define CBREAK_ERRORLEVEL 3

/* Use este modos_de_arquivos enquanto localizando por complementa��o do arquivo */
#define FILE_SEARCH_MODE FA_RDONLY | FA_ARCH | FA_DIREC


/* Configura��o de menssagens padr�o:
  PATTERN: como a string � construida pelo ID (com \n)
  OUTOFMEMORY: string para informar uma condi��o de falta-de-mem�ria(no \n)
  ID_: O ID do erro quando n�o h� modelo padr�o para ser criado, mas
  	a string de falta-de-memoria for mostrada
*/
#define MSG_DFL_PATTERN "String #%u\n"
#define MSG_DFL_OUTOFMEMORY "Falta de memoria!"
#define MSG_ERR_ID_OUTOFMEMORY TEXT_ERROR_OUT_OF_MEMORY


/* Prompt padr�o */
#define DEFAULT_PROMPT "$P$G"

/* Prove o maior tamanho que o contexto pode ter */
#define CONTEXT_MAX_SIZE (65535U - 12)

/* Defina este valor para selecionar o valor de FDDEBUG para a inicializa��o */
#define FDDEBUG_INIT_VALUE 1

#define INCLUDE_CMD_BEEP
#define INCLUDE_CMD_BREAK
#define INCLUDE_CMD_CHDIR
#define INCLUDE_CMD_CDD
#define INCLUDE_CMD_CLS
#define INCLUDE_CMD_COPY
#define INCLUDE_CMD_CTTY
#define INCLUDE_CMD_DATE
#define INCLUDE_CMD_DEL
#define INCLUDE_CMD_DIR
#define INCLUDE_CMD_DIRS
#define INCLUDE_CMD_LOADFIX
#define INCLUDE_CMD_LOADHIGH
#define INCLUDE_CMD_MEMORY
#define INCLUDE_CMD_MKDIR
#define INCLUDE_CMD_PATH
#define INCLUDE_CMD_PAUSE
#define INCLUDE_CMD_PROMPT
#define INCLUDE_CMD_PUSHD
#define INCLUDE_CMD_POPD
#define INCLUDE_CMD_REM
#define INCLUDE_CMD_RENAME
#define INCLUDE_CMD_RMDIR
#define INCLUDE_CMD_SET
#define INCLUDE_CMD_TIME
#define INCLUDE_CMD_TRUENAME
#define INCLUDE_CMD_TYPE
#define INCLUDE_CMD_VER
#define INCLUDE_CMD_VERIFY
#define INCLUDE_CMD_VOL
#define INCLUDE_CMD_QUESTION
#define INCLUDE_CMD_WHICH
#ifdef DEBUG
#define INCLUDE_CMD_FDDEBUG
#endif
/*
   comando que n�o tem uma defini��o:

   exit
   call
   echo
   goto
   for
   if
   shift

 */

/********
	***** Desfazer dependencias
	***** N�o mude a n�o ser que mude o fonte apropriado!
	************/
#if defined(INCLUDE_CMD_PUSHD) || defined(INCLUDE_CMD_POPD)
#define	INCLUDE_CMD_CDD
#endif

/* N�o dispon�vel com TURBOC++ 1.0 ou posteriores: */
#ifdef __TURBOC__

#if __TURBOC__ > 0x297
#define _TC_LATER_
#else
#define _TC_EARLY_
#endif

#endif

/* Define se seu compilador n�o tem 'dosdate_t' or 'dostime_t' */
#ifdef _TC_EARLY_
	/* TC++1 */
#define _NO__DOS_DATE
#define _NO__DOS_TIME
#define _NO_FMEMCHR
#define _NO_FMEMCMP
#endif


	/* definido por MKDIST.BAT */
#ifdef IGNORE_ENHANCED_INPUT
#undef FEATURE_ENHANCED_INPUT
#endif

#ifndef FEATURE_ENHANCED_INPUT
#undef FEATURE_HISTORY
#undef FEATURE_FILENAME_COMPLETION
#endif

#ifdef INCLUDE_CMD_PUSHD
#define FEATURE_DIRSTACK
#endif

#if CONTEXT_MAX_SIZE > 65535U - 12
#error "O tamanho do contexto m�ximo n�o pode exeder 65535 - 12 bytes"
#endif

#if defined(FEATURE_KERNEL_SWAP_SHELL) && defined(FEATURE_XMS_SWAP)
#error "FreeCOM n�o pode suportar ambos o Kernel- e XMS-based swapping simultaneamente
#endif

/* Externamente definida para compilar com suporte a XMS-Swap */
#if defined(XMS_SWAP)
#undef FEATURE_KERNEL_SWAP_SHELL
#define FEATURE_XMS_SWAP
#endif

#ifdef __TURBOC__
#if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
/* Compilando em modelo de mem�ria larga --> deslisga AVISO:
	"Conves�o pode perder digitos significantes"	*/
#pragma warn -sig
#endif
#endif

#include "../include/debug.h"
