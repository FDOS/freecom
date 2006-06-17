/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-2000 Steffen Kaiser

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $RCSfile$
   $Locker$	$Name$	$State$

	SUPPL common debugging features

	If used by an application, SUPPL and the application will use the
	same debugging driver and, therefore, debugging extends from the
	application into the functions included from SUPPL. This is
	useful especially while debugging heap actions (alloc/dealloc).

	Note #1: If NDEBUG is defined, all debugging features are turned off!
	Note #2: This #include file MUST be included at the end of the list,
	at least behind all the standard #includes referencing via '<...>'.
	Otherwise it is possible that some redefines do not work or the
	compiler will detect errors within standard include files.

	Understood debugging features:
	SUPPL_DBG_MEMORY:
		Causes that the macro chkMem performs an extensive test of
		the logical memory management of DOS and the C runtime environment.
	SUPPL_DBG_HEAP:
		Causes that the macro 'chkHeap' actively checks the local
		heap for corruption.
		chkHeap includes chkMem!
	SUPPL_LOG_MEMORY:
		Causes to redefine heap-related CLib functions (malloc, calloc,
		realloc, free,& strdup).
	SUPPL_LOG_FUNCTION:
		Note: These package was inspired by Fred Fish's DBUG package
			shipped with DMake v3+.
		When the macro SUPPL_LOG_NO_INFO is #define'ed prior including
		this header file, the macro DBG_INFO() expands to nothing.

		Causes to active all of the DBG_* macros:
		DBG_ENTER() - logs that a function was entered
		DBG_ENTER1/DBG_ENTER2() - as DBG_ENTER, *1 -> declaration, *2 -> fct
		DBG_ARGUMENTS() - logs function arguments
		DBG_RETURN_VALUES() - logs function return values
		DBG_EXIT() - logs that a function was exited
		DBG_LEAVE() - logs that a function was exited (with parameter, no log)
		DBG_LONG_RETURN() - logs that a function was exited (with param & log)
		DBG_RETURN_?() - logs that a function was exited (with param & log)
		DBG_PRINT() - logs arbitary information
		DBG_INFO() - logs arbitary information of information class "inf"
		DBG_STRING() - logs a string in information class "inf"
		DBG_CHANGE_STATE() - changes logging status
		DBG_PUSH_STATE() - duplicates current logging status
		DBG_POP_STATE() - forget current logging status & restore previous one
		DBG_TERMINATE() - terminate application with a log message

*/

#ifndef __SUPPLDBG_H
#define __SUPPLDBG_H

#include <assert.h>
#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <portable.h>

/*
 *	Initialize all macros with dummy values. They are undefined later
 *	right before a debugging specific value is added.
 *	Initializing them here ensures that they are correctly set for
 *	non-debugging.
 */
#define chkHeap		;
#define chkMem		;


#ifdef NDEBUG			/* native debug support disabled? */

/* Turn off every SUPPL specific debugging feature */
#undef SUPPL_DBG_HEAP
#undef SUPPL_DBG_MEMORY
#undef SUPPL_LOG_MEMORY
#undef SUPPL_LOG_FUNCTION

#else

#ifdef SUPPL_DBG_ALL

/* Turn on every SUPPL specific debugging feature */
#define SUPPL_DBG_HEAP
#define SUPPL_DBG_MEMORY
#define SUPPL_LOG_MEMORY
#define SUPPL_LOG_FUNCTION

#endif

#endif		/* !defined(NDEBUG) */

#ifdef SUPPL_ALWAYS_DEBUG_HEAP
#define SUPPL_DBG_HEAP
#endif
#ifdef SUPPL_ALWAYS_DEBUG_MEMORY
#define SUPPL_DBG_MEMORY
#endif

#ifdef SUPPL_DBG_MEMORY

#undef chkMem
#define chkMem		suppl_testMemChain();

#endif


#ifdef SUPPL_DBG_HEAP

/* Define the macro 'chkHeap' to actively test the local heap for
   corruption. Note: The macro must be used without semicolon!
*/
#undef chkHeap
#ifdef __TURBOC__
#include <alloc.h>
#ifdef NDEBUG
#define chkHeap	suppl_testHeap();  chkMem
#else
#define chkHeap	assert(heapcheck() == _HEAPOK); chkMem
#endif
#else
#define chkHeap	chkMem
#endif

#endif

#ifdef SUPPL_LOG_MEMORY
#include "syslog.h"

/* Use internal wrappers for CLib symbols */

#ifdef malloc
#undef malloc
#endif

#ifdef realloc
#undef realloc
#endif

#ifdef calloc
#undef calloc
#endif

#ifdef free
#undef free
#endif

#ifdef strdup
#undef strdup
#endif

#define malloc(size)		suppl_malloc((size))
#define calloc(nelem,size)	suppl_calloc((nelem), (size))
#define realloc(poi,size)	suppl_realloc((poi), (size))
#define free(poi)			suppl_free((poi))
#define strdup(poi)			suppl_strdup((poi))

#endif

#ifdef SUPPL_LOG_FUNCTION

/* The following macros are defined:

	DBG_EXTENSION
		Extension of the logfile

	DBG_LIBRARY
		If redefined by the user, this macros specifies the name
		of the library, the current file is located in.
		Defaults to none.

	DBG_ALWAYS_FLUSHLOG
		If defined, the log file is flushed each time one entry has been
		written successfully into the logfile. (compile-time option
		- see INITSUPL.LOC!)

	DBG_ALWAYS_CLOSELOG
		If defined, the log file is closed each time one entry has been
		written successfully into the logfile. (compile-time option
		- see INITSUPL.LOC!)
		Use this option if system crashes corrupt the logfile or same
		entries won't appear there.

	NONE OF THESE MACROS MUST NOT BE USED WITH A TRAILING SEMICOLON!

	DBG_CLOSELOG
		Close the logfile.

	DBG_FLUSHLOG
		Flush the log file and make sure all data is written to disk
		and the directory entry has been updated.
		This function should be called each time when it is possible
		that the program is going to crash or is going to be
		asynchroneously terminated (e.g. Ctrl-Alt-Del).

	DBG_ENTER(char *function_name, char *function_class)
		Enters the function with name "function_name" and class. (see STATE)
		It must be the first statement immediately following the variable
		declaration, because it will define a variable as well.
		This macro must be pairly used with DBG_LEAVE or DBG_EXIT.
		All of the other macros won't work without DBG_ENTER.

	DBG_ENTER1
		Is the variable declaration part of DBG_ENTER()

	DBG_ENTER2(char *function_name, char *function_class)
		Is the function calling part of DBG_ENTER()

	DBG_ARGUMENTS( (char *format_spec, arguments) )
		Logs the arguments of the current function. The parameters follow
		the same rules as in DBG_PRINT. The only difference to DBG_PRINT
		is that the output can be selectively enabled/disabled.

	DBG_RETURN_VALUES( (char *format_spec, arguments) )
		Logs the retrun values of the current function. The parameters follow
		the same rules as in DBG_PRINT. The only difference to DBG_PRINT
		is that the output can be selectively enabled/disabled.

	DBG_EXIT
		Exits a void function. The "return" statement is already
		included.

	DBG_LEAVE(return_expression)
		As DBG_EXIT, but "return_expression" is passed as an argument
		to "return". The return value is NOT logged automatically.

	DBG_LONG_RETURN(type, char *format_spec, return_expression)
		This macro combines: DBG_RETURN_VALUES and DBG_LEAVE.
		Unfortunately return_expression would be evaluated twice,
		therefore it is evaluated once and stored into a temporary
		variable of the specified type; 'type' must (or, at least,
		should) be of the same type as the function returns. The
		same applies to the format specification, which is passed
		to printf().

	DBG_RETURN_#(return_expression)
		As DBG_RETURN(), '#' is the uppercase letter usually find
		within a printf()-format specification:
		#	type			format_spec
		C	char			"%c"
		S	char*			"\"%s\""
		I	int				"%d"
		U	unsigned		"%u"
		X	int				"0x%x"
		L	long			"%ld"
		UL	unsigned long	"%lu"
		F	double			"%f"
		G	double			"%g"
		P	void *			"%p"

		A 'B' may be prefixed to buffer "return_expression" within a
		temporary variable to suppress side effects and/or multiple
		evalutation of a complex expression.

	DBG_PRINT( char *information_class, (char *format_spec, arguments) )
		Logs ONE line. "format_spec" is a valid printf() format
		string and should not contain no '\n' character. Both the
		format spec and the arguments must be enclosed into
		parenthises.
		The information_class specifies whether or not this information
		is logged (see STATE).

	DBG_INFO( (char *format_spec, arguments) )
		As DBG_PRINT(), but prints message of class "inf".
		Because these macro may generate a lot of strings, they can be
		individually disable with the SUPPL_LOG_NO_INFO macro. It must
		be #define'ed prior including "suppldbg.h"

	DBG_STRING(char *string)
		As DBG_INFO(), but prints a string only

	DBG_PUSH_STATE
		Duplicates the current logging status. Any subsequent DBG_CHANGE_STATE
		will effect the newly generated status only.
		If there is no status currently, this function is ignored.

	DBG_POP_STATE
		Discards the topmost logging status and restores the previous one.
		If there is only one or none pushed status, this function is
		ignored.

	DBG_CHANGE_STATE(char *state_spec)
		Modifies the current logging status according the state_spec.
		If there is no current status, a new one will be created.
		The state_spec is a sequence of log_specs delimited by semicolons.
		A log_spec consists of a log_format_letter and arguments,
		the arguments can be delimited by commas, if necessary.
		No superflous whitespaces or such are allowed!

		The following log_format_letters are understood:

		d#		set the maximum logging depth (#: unsigned number)
				default: no limit
		f?*,...	files to be logged (*: one file name), each filename
				is prefixed by '+' enable, or '-' disable file.
				Wildcards are not allowed. The filename must be specified
				the same way as the compiler named them in their __FILE__
				macro, this is usually the fully-qualified filename.
				"+" alone logs everything (clears every previous settings),
				"-" alone logs nothing (also clears every previous settings).
				default: log everything.
		c?*,...	functions to be logged (*: one function name)
				Syntax the same as "f*,...", but the string must
				case-sensitively match the string passed to DBG_ENTER()
		l?		globally enable/disable logging (?: + enable, - disable)
		i?		indent output (?: + enable, - disable)
				If disbale the nesting level is inserted as number, otherwise
				as a repetive count of "|  " strings
		C?*,...	classes to be logged
				Syntax the same as "f*,...", but the string must either
				case-sensitively match one of the classes specified as
				function_class (DBG_ENTER) or information_class (DBG_PRINT)
				or one of the SUPPL classes:
				information classes:
					"arg" - DBG_ARGUMENTS
					"rv"  - DBG_RETURN_VALUE
					"fct" - DBG_ENTER & DBG_EXIT/DBG_LEAVE
					"inf" - informative information

				function classes:
					"env" - environment
					"mcb" - MCB
					"mem" - memory
					"str" - string
					"ini" - INI file
					"cmd" - command line handling
					"err" - error wrappers
					"dfn" - filename functions
					"fil" - FILE wrappers
					"lmt" - long mathematics
					"nls" - NLS
					"log" - syslog / logging
					"msc" - misc.

				Note: function and information classes share the same
				name space.
				Note: If the library name is specify with DBG_LIBRARY,
				one must enable the library as well. All SUPPL files
				use the library name "SUPPL". E.g. to enable SUPPL logging,
				it is _not_ sufficient to specify "C+", but "C+,+SUPPL",
				note that "+SUPPL" must trail "+"!
				Defaults to '-' (disable everything).

	DBG_TERMINATE(char *string)
		logs the string, then "program terminated by debug system",
		then terminates the application via exit(DBG_EXIT_CODE)
*/

#ifndef DBG_LIBRARY
#define DBG_LIBRARY 0
#endif

#define DBG_CLOSELOG								\
	suppl_log_close();

#define DBG_FLUSHLOG								\
	suppl_log_flush();

#define DBG_ENTER1									\
	auto suppl_log_local_data_t suppl_log_data;

#define DBG_ENTER2(fct_name, fct_class)				\
	suppl_log_enter(DBG_LIBRARY, __FILE__, __LINE__, &suppl_log_data	\
		, (fct_name), (fct_class));

#define DBG_ENTER(fct_name, fct_class)				\
	DBG_ENTER1										\
	DBG_ENTER2(fct_name, fct_class)

#define DBG_ARGUMENTS( info )						\
	DBG_PRINT( Suppl_class_arg , info )

#define DBG_RETURN_VALUES( info )					\
	DBG_PRINT( Suppl_class_rv , info )

#define DBG_EXIT									\
	{	suppl_log_leave(&suppl_log_data);		\
		return; }

#define DBG_LEAVE(expr)								\
	{	suppl_log_leave(&suppl_log_data);		\
		return (expr); }

#define DBG_LONG_RETURN(type, fmt, expr)			\
	{	type suppl_temp_var;						\
		suppl_temp_var = (expr);					\
		DBG_RETURN_VALUES( (fmt, suppl_temp_var) )	\
		DBG_LEAVE( suppl_temp_var )					\
	}

#define DBG_RETURN(fmt, expr)					\
	{	DBG_RETURN_VALUES( (fmt, (expr)) )		\
		DBG_LEAVE( (expr) )						\
	}

#if 1
#define DBG_PRINT(inf_class, info)					\
	{ if(suppl_log_print_1((inf_class)))			\
			suppl_log_print_2 info;					\
	}
#else
#define DBG_PRINT(inf_class, info)					\
	{ if(suppl_log_class_enabled((inf_class)))		\
			suppl_log_log info;						\
	}
#endif

#ifdef SUPPL_LOG_NO_INFO
#define DBG_INFO(info)								;
#define DBG_STRING(info)								;
#else
#define DBG_INFO( info )						\
	DBG_PRINT( Suppl_class_inf , info )
#define DBG_STRING(str)			DBG_INFO( (str) )
#endif

#define DBG_PUSH_STATE								\
	suppl_log_push();

#define DBG_POP_STATE								\
	suppl_log_pop();

#define DBG_CHANGE_STATE(state)						\
	suppl_log_change((state));

#define DBG_TERMINATE(loginfo)						\
	suppl_log_terminate((loginfo));

#else

/* Generate pseudo-macros */
#define DBG_CLOSELOG							;
#define DBG_FLUSHLOG							;
#define DBG_ENTER1								;
#define DBG_ENTER2(fct_name, fct_class)			;
#define DBG_ENTER(fct_name, fct_class)			;
#define DBG_ARGUMENTS( info )					;
#define DBG_RETURN_VALUES( info )				;
#define DBG_EXIT								return;
#define DBG_LEAVE(expr)							return (expr);
#define DBG_LONG_RETURN(t,f,expr)				return (expr);
#define DBG_RETURN(f,expr)						return (expr);
#define DBG_PRINT(inf_class, info)				;
#define DBG_INFO( info )						;
#define DBG_STRING(str)							;
#define DBG_PUSH_STATE							;
#define DBG_POP_STATE							;
#define DBG_CHANGE_STATE(state)					;
#define DBG_TERMINATE(string)					suppl_log_exit((string));

#endif

/* Format specs used for the DBG_RETURN_*() macros */
#define DBG_RETURN_FMT_SPEC_C	"%c"
#define DBG_RETURN_FMT_SPEC_S	"\"%s\""
#define DBG_RETURN_FMT_SPEC_I	"%d"
#define DBG_RETURN_FMT_SPEC_U	"%u"
#define DBG_RETURN_FMT_SPEC_X	"0x%x"
#define DBG_RETURN_FMT_SPEC_L	"%ld"
#define DBG_RETURN_FMT_SPEC_UL	"%lu"
#define DBG_RETURN_FMT_SPEC_F	"%f"
#define DBG_RETURN_FMT_SPEC_G	"%g"
#define DBG_RETURN_FMT_SPEC_P	"%p"

#define DBG_RETURN_BC(expr)	DBG_LONG_RETURN(char, DBG_RETURN_FMT_SPEC_C, (expr))
#define DBG_RETURN_BS(expr)	DBG_LONG_RETURN(char*, DBG_RETURN_FMT_SPEC_S, (expr))
#define DBG_RETURN_BI(expr)	DBG_LONG_RETURN(int, DBG_RETURN_FMT_SPEC_I, (expr))
#define DBG_RETURN_BU(expr)	DBG_LONG_RETURN(unsigned, DBG_RETURN_FMT_SPEC_U, (expr))
#define DBG_RETURN_BX(expr)	DBG_LONG_RETURN(int, DBG_RETURN_FMT_SPEC_X, (expr))
#define DBG_RETURN_BL(expr)	DBG_LONG_RETURN(long, DBG_RETURN_FMT_SPEC_L, (expr))
#define DBG_RETURN_BUL(expr)	DBG_LONG_RETURN(unsigned long, DBG_RETURN_FMT_SPEC_UL, (expr))
#define DBG_RETURN_BF(expr)	DBG_LONG_RETURN(double, DBG_RETURN_FMT_SPEC_F, (expr))
#define DBG_RETURN_BG(expr)	DBG_LONG_RETURN(double, DBG_RETURN_FMT_SPEC_G, (expr))
#define DBG_RETURN_BP(expr)	DBG_LONG_RETURN(void *, DBG_RETURN_FMT_SPEC_P, (expr))

#define DBG_RETURN_C(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_C, (expr))
#define DBG_RETURN_S(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_S, (expr))
#define DBG_RETURN_I(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_I, (expr))
#define DBG_RETURN_U(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_U, (expr))
#define DBG_RETURN_X(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_X, (expr))
#define DBG_RETURN_L(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_L, (expr))
#define DBG_RETURN_UL(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_UL, (expr))
#define DBG_RETURN_F(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_F, (expr))
#define DBG_RETURN_G(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_G, (expr))
#define DBG_RETURN_P(expr)	DBG_RETURN(DBG_RETURN_FMT_SPEC_P, (expr))

/*
 *	Declarations of functions
 */

/* Some special log functions:
	malloc --> malloc(size) = poi; <success|failure>
	calloc --> malloc(size) = poi; <success|failure>
	realloc --> realloc(poi, size) = poi; <success|failure>
	free --> free(poi);
	strdup -->	malloc(size) = poi;	<success|failure>

	These functions will automatically open the logfile if necessary.
*/
extern void suppl_log_malloc(void const * const newpoi, const size_t size);
/* extern void suppl_log_calloc(void const * const newpoi, const size_t nelem, const size_t size); */
#define suppl_log_calloc(newpoi,nelem,size)		\
	suppl_log_malloc((newpoi), (nelem) * (size))
extern void suppl_log_realloc(void const * const newpoi, const void * const oldpoi, const size_t size);
extern void suppl_log_free(const void * const oldpoi);
extern void suppl_log_strdup(char const * const newpoi, const char * const string);

/* These functions substitutes the associated function and
	call suppl_log_*(), too. */
extern void *suppl_malloc(const size_t size);
extern void *suppl_calloc(const size_t nelem, const size_t size);
extern void *suppl_realloc(void * const poi, const size_t size);
extern void suppl_free(void * const poi);
extern char *suppl_strdup(const char * const string);

/* Function to test if the DOS Memory Chain has been corrupted
	if so, the application is terminated */
extern void suppl_testMemChain(void);
extern void suppl_testHeap(void);

/* These functions are the backend of the logging package */
typedef unsigned suppl_log_lvl_t;	/* nesting level type */
typedef unsigned long suppl_log_linenr_t;	/* compiler line number type */
typedef const char * suppl_log_sptr_t;	/* string pointer */
typedef const char * const suppl_log_csptr_t;	/* string pointer */
typedef struct {
	suppl_log_lvl_t	suppl_l_prv_nestlevel;
	FLAG			suppl_l_prv_fct_enabled;
	suppl_log_sptr_t suppl_l_fctname;
} suppl_log_local_data_t;

void suppl_log_enter(suppl_log_sptr_t library_name
	, suppl_log_csptr_t file_name
	, suppl_log_linenr_t linenr
	, suppl_log_local_data_t *data
	, suppl_log_csptr_t function_name
	, suppl_log_csptr_t function_class);
void suppl_log_leave(suppl_log_local_data_t *data);
int suppl_log_class_enabled(suppl_log_csptr_t classname);
void suppl_log_log(suppl_log_csptr_t fmt, ...);
int suppl_log_print_1(suppl_log_csptr_t inf_class);
void suppl_log_print_2(suppl_log_csptr_t fmt, ...);
void suppl_log_push(void);
void suppl_log_pop(void);
void suppl_log_change(suppl_log_csptr_t state);
void suppl_log_flush(void);
void suppl_log_close(void);
void suppl_log_terminate(suppl_log_csptr_t loginfo);
void suppl_log_exit(suppl_log_csptr_t loginfo);


/*
 *	Strings of the function classes used within SUPPL
 *	defined as constant character arrays to prevent that each file
 *	creates its own local copy of the string in its data segment
 */
extern const char Suppl_appName[];
extern const char Suppl_cmdline[];
extern const char Suppl_dfn[];
extern const char Suppl_dynstr[];
extern const char Suppl_env[];
extern const char Suppl_error[];
extern const char Suppl_farmem[];
extern const char Suppl_inifile[];
extern const char Suppl_inifile1[];
extern const char Suppl_inifile2[];
extern const char Suppl_inifile3[];
extern const char Suppl_mcb[];
extern const char Suppl_misc[];
extern const char Suppl_nls[];
extern const char Suppl_portable[];
extern const char Suppl_SUPPL[];
extern const char Suppl_supplio[];
extern const char Suppl_syslog[];
extern const char Suppl_usr[];

/*
 *	Strings of the log classes used within SUPPL
 *	defined as constant character arrays to prevent that each file
 *	creates its own local copy of the string in its data segment
 */
extern const char Suppl_class_arg[];
extern const char Suppl_class_rv[];
extern const char Suppl_class_fct[];
extern const char Suppl_class_inf[];
extern const char Suppl_class_env[];
extern const char Suppl_class_mcb[];
extern const char Suppl_class_mem[];
extern const char Suppl_class_str[];
extern const char Suppl_class_ini[];
extern const char Suppl_class_cmd[];
extern const char Suppl_class_err[];
extern const char Suppl_class_dfn[];
extern const char Suppl_class_fil[];
extern const char Suppl_class_lmt[];
extern const char Suppl_class_nls[];
extern const char Suppl_class_log[];
extern const char Suppl_class_msc[];
#endif		/* defined(__DBUG_H) */
