/*
 * config.h - Usado para configurar o que será compilado para interpretador.
 *
 */

/* Defina DEBUG para adicionar código de  add depuração */
#ifndef DEBUG			/* possivelmente já definido via linha de comando */
//#define DEBUG
#endif

/* Defina para ativar o comando alias, e atalhos. */
#define FEATURE_ALIASES
#define ALIAS_DEFAULT_SIZE 1024

/* Defina para ativar entrada de dados avançada (pre-requisito de Histórico e complementação 
	nome de arquivo  */
#define FEATURE_ENHANCED_INPUT

/* Defina ativar histórico(DOSKEY); requer: Entrada de dados avançada */
#define FEATURE_HISTORY
#define HISTORY_DEFAULT_SIZE 256

/* Defina para ativar complementação de nome de arquivo; requer: Entrada de dados avançada */
#define FEATURE_FILENAME_COMPLETION

/* Defina para ativar para carregar menssagens na memoria */
#define FEATURE_LOAD_MESSAGES

/* Defina para ativar DOS NLS */
#define FEATURE_NLS

/* Defina para ativar teste das teclas F5/F8 na inicialização se /P estiver presente na
	linha de comando */
/* #define FEATURE_BOOT_KEYS */

/* LOG de características da linha de comando */
/* #define FEATURE_CALL_LOGGING */

/* Preserva o último diretorio (CD, CHDIR, CDD, PUSHD, POPD);
	"CD -" lá está chdir */
#define FEATURE_LAST_DIR

/* Ativa o suporte para instalação de extenções de COMANDO (MUX-AE)
 */
#define FEATURE_INSTALLABLE_COMMANDS

/* Nome do executável */
#define COM_NAME "COMMAND.COM"
/* Nome padrão do AUTOEXEC.BAT */
#define AUTO_EXEC "\\autoexec.bat"
/* Arquivo de LOG para registrar algumas caracteristicas; PRECISA estar altamente-qualificado! */
#define LOG_FILE "C:\\FreeCom.log"

/* Define o numero de loops quando redirecionar para o CON se o
	interpretados ficar em um loop - "hangForEver()" - para sempre
   Não defina para remover esta característica */
#define FEATURE_AUTO_REDIRECT_TO_CON 5

/* Quantos arquivo de lote devem ser aninhados minimamente */
#define BATCH_NESTLEVEL_MIN 5

/* Defina para adicionar suporte ao swapout do FreeCOM
	see DOCS\K-SWAP.TXT
*/
#define FEATURE_KERNEL_SWAP_SHELL

/* Defina para adicionar suportr para troca XMS-only do FreeCOM
	Está definição é incompatível com a definição acima.
*/
/* #define FEATURE_XMS_SWAP */

/* Define o tamanho do buffer usado para guardar todos os caminhos antigos para o PUSHD/POPD */
#define DIRSTACK_DEFAULT_SIZE 256

/* Use isto se um programa externo foi terminado por
	^C or ^Break */
#define CBREAK_ERRORLEVEL 3

/* Use este modos_de_arquivos enquanto localizando por complementação do arquivo */
#define FILE_SEARCH_MODE FA_RDONLY | FA_ARCH | FA_DIREC


/* Configuração de menssagens padrão:
  PATTERN: como a string é construida pelo ID (com \n)
  OUTOFMEMORY: string para informar uma condição de falta-de-memória(no \n)
  ID_: O ID do erro quando não há modelo padrão para ser criado, mas
  	a string de falta-de-memoria for mostrada
*/
#define MSG_DFL_PATTERN "String #%u\n"
#define MSG_DFL_OUTOFMEMORY "Falta de memoria!"
#define MSG_ERR_ID_OUTOFMEMORY TEXT_ERROR_OUT_OF_MEMORY


/* Prompt padrão */
#define DEFAULT_PROMPT "$P$G"

/* Prove o maior tamanho que o contexto pode ter */
#define CONTEXT_MAX_SIZE (65535U - 12)

/* Defina este valor para selecionar o valor de FDDEBUG para a inicialização */
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
   comando que não tem uma definição:

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
	***** Não mude a não ser que mude o fonte apropriado!
	************/
#if defined(INCLUDE_CMD_PUSHD) || defined(INCLUDE_CMD_POPD)
#define	INCLUDE_CMD_CDD
#endif

/* Não disponível com TURBOC++ 1.0 ou posteriores: */
#ifdef __TURBOC__

#if __TURBOC__ > 0x297
#define _TC_LATER_
#else
#define _TC_EARLY_
#endif

#endif

/* Define se seu compilador não tem 'dosdate_t' or 'dostime_t' */
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
#error "O tamanho do contexto máximo não pode exeder 65535 - 12 bytes"
#endif

#if defined(FEATURE_KERNEL_SWAP_SHELL) && defined(FEATURE_XMS_SWAP)
#error "FreeCOM não pode suportar ambos o Kernel- e XMS-based swapping simultaneamente
#endif

/* Externamente definida para compilar com suporte a XMS-Swap */
#if defined(XMS_SWAP)
#undef FEATURE_KERNEL_SWAP_SHELL
#define FEATURE_XMS_SWAP
#endif

#if sizeof(char*) > sizeof(short)
/* Compilando em modelo de memória larga --> deslisga AVISO:
	"Convesão pode perder digitos significantes"	*/
#pragma warn -sig
#endif

#include "../include/debug.h"
