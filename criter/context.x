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

word envSegm 0		# segment to be passed forth as environment 
pointer prg 0		# program to be executed 
pointer cmdline 0	# command line arguments of program 
word execErr 0		# exit code of DOS-4B of external program
pointer shell 0		# absolute path to shell to be executed 
word canexit 0		# true unless FreeCOM was invoked with /P 
word dfltSwap 0		# default value of swapOnExec
word debug 0		# Debug status
word envSize 0		# size of environment segment 
word dyn_ctxt 0		# dynamically changed portions of context 
word ofs_criter _lowlevel_err_handler		# offset of criter module within this context 
reference cbreak_hdlr	# start of dummy ^Break handler active if FreeCOM
					# is NOT running itself
