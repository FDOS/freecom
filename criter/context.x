# Context description file
#
#	Contains persistent information to be preserved during the
#	execution of an external program
#
# separated to allow to use one file for both assembly and C
# TOOLS\MKCTXT.EXE creates the following files:
#	context.h_c: contains the structure in C syntax
#	context.h_a: contains the definition of the structure in assembly
#	context.inc: contains the EQUs of the offsets of a value into the structure
#		two definitions per offset:
#		<value> - offset from the very beginning of the context
#		<package>_<value> - offset of the value relative to the beginning
#			of the specific package

# "typedef" opens a new "typedef struct {".
# "package" groups different values together, if the package
# is a shared module, too, a pointer to this position is passed
# to the module. Within C, they are represented by struct's
# "structure" is more or less the same, but it is ensured that all
# structures defined for one package are physically located in above the
# pointer to the package
# An empty line (no comment lines!) end a structure.

# Type:
# byte - one single byte (unsigned char)
# word - two bytes (unsigned short int)
# pointer - far pointer (void far*)
# constant - #define of _numerical_ constant
# reference - label at exactly this position (unsigned char [])


typedef kswap_t	# Type currently defined

# Internal structurical settings
word envSegm 0		# segment to be passed forth as environment 
word envSize 0			# minimum size of environment segment 
word dyn_ctxt 0		# dynamically changed portions of context 
pointer prg 0		# program to be executed 
pointer cmdline 0	# command line arguments of program 
word execErr 0		# exit code of DOS-4B of external program
pointer shell 0		# absolute path to shell to be executed 

structure flags ctxt_flags_t
byte f_dispPrompt  0	# display prompt on interactive cmdline
byte f_echo 0		 	# batch script echo mode
byte f_swap 0			# do swapping by default
#* flags below default to zero
byte f_canexit 0		# may allowed to exit this shell
byte f_interactive 0	# set if the current command had been entered
#							interactively via command line
byte f_call 0			# invoke via CALL by default
byte f_trace 0			# batch script trace mode by default
byte f_debug 0			# FreeCOM debugging
byte f_persistentMSGs 0	# keep messages in memory
word f_errorlevel 0
word f_base_shiftlevel 0
word f_shiftlevel 0		# argument shift level
word f_batchlevel 0		# how many batch nesting levels

word ofs_criter _lowlevel_err_handler		# offset of criter module within this context 
reference cbreak_hdlr	# start of dummy ^Break handler active if FreeCOM
					# is NOT running itself
