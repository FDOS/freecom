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

typedef context_t	# Type currently defined

# Type	Name		Init-value (assembly)	# Explanation
package	int24		# INT-24 shared module
byte	autoFail	0	# INT-24 shall return FAIL action code always

package	cb_catch	# ^Break shared module
byte	cntrlBreak	0	# ^Break occured while FreeCOM was active
word	psp1		0	# 1st PSP to consider "FreeCOM is active"
word	psp2		0	# 2nd PSP to consider "FreeCOM is active"

package	respawn		# Respawner module
byte	exitCode	0	# exit code of external program
byte	exitReason	-1	# 0: normal, 1: ^C, 2: CritErr, 3: TSR
						# -2: DOS-4B-00 failed, -1: invalid, e.g.
						#	set by FreeCOM _before_ activating
						#	respawner to verify if it was respawned
						#	or called as the external program

pointer	prgname	0		# pointer to filename of external program
structure	execblock	# EXEC block when external program is spawned
word	prgSegmExtern	0	# segment of environment to pass the child
pointer	cmdlineExtern	0	# command line passed to program
pointer fcb1Extern	0	# FCB of 1st argument
pointer fcb2Extern	0	# FCB of 2nd argument

structure	execblockFcom	# EXEC block when FreeCOM is re-spawned
word	prgSegmFcom	0	# segment of environment to pass the child
pointer	cmdlineFcom	0	# command line passed to program
pointer fcb1Fcom	0	# FCB of 1st argument
pointer fcb2Fcom	0	# FCB of 2nd argument

package internal	# Internal settings
word	saved_fd0	-1	# Saved file descriptor 0 (stdin)
word	saved_fd1	-1	# Saved file descriptor 1 (stdout)
word	saved_fd2	-1	# Saved file descriptor 2 (stderr)
word	saved_fd3	-1	# Saved file descriptor 3 (stdprn)
word	saved_fd4	-1	# Saved file descriptor 4 (stdaux)
word	can_exit	1	# Exit allowed (/P not specified)
