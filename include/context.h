/* $id$

	Context declarations

*/

#ifndef CONTEXT__H_
#define CONTEXT__H_

#include <portable.h>
#include <mcb.h>
#include "../context.h_c"

#define MAX_FNAME	89		/* max size of string for one filename */

typedef word ctxt_t;		/* general context type <-> segment */
#define CTXT_INVALID ((ctxt_t)0)

extern volatile ctxt_t ctxtMain;	/* currently there is just one context
								-> so-called dynamic context
								-> all data is located in there
								it is the location of the subMCB */
#define ctxtSegm (ctxtMain + 1)	/* segment of the subMCB creating the
								pseudo-environment segment */

#include <algnbyte.h>
typedef struct CtxtCB {
	byte ctxt_type;		/* type of sub-MCB <-> 'Z': no following MCB */
	word ctxt_owner;	/* owner of this dyn context (future use) */
	word ctxt_size;		/* size of this sub-MCB */
	byte ctxt_eOffs;	/* offset of start of exec contexts */
	byte ctxt_reserved[10];
} ctxtCB_t;
typedef struct CtxtEC {
	byte ctxt_type;
	word ctxt_length;
} ctxtEC_t;
typedef struct {	/* B context */
	dword ec_pos;		/* file position */
	dword ec_lnum;		/* line number */
	char ec_fname[1];	/* file name; dynamically extended */
} ctxtEC_Batch_t;
typedef struct {	/* f context */
	byte ec_ffblk[21];	/* DOS specific ffblk continuation info */
	word ec_varname;	/* displacement of varname in F-context */
	word ec_cmd;		/* displacement of command in F-context */
	char ec_prefix[1];	/* argument prefix; dynamically extended */
} ctxtEC_For_t;
#include <algndflt.h>
#if sizeof(ctxtCB_t) != 16
#error "The size of type ctxtCB_t must be equal to 16"
#endif
#define ctxtMCB ((struct MCB _seg *)SEG2MCB(ctxtMain))
#define ctxtECHighestSegm()	(nxtMCB(FP_SEG(ctxtMCB)))
#define ctxtECLowestSegm()	(ctxtSegm + (env_firstFree(ctxtSegm) + 15) / 16)
#define ctxtp ((ctxtCB_t _seg *)ctxtMain)
#define ctxtExecContext		\
	((ctxtEC_t far*)MK_FP(ctxtSegm + ctxtp->ctxt_size, ctxtp->ctxt_eOffs))
extern ctxtCB_t ctxtInitialCB;
extern ctxtEC_t ctxtInitialEC;
#define ecData(ec,type)	((type far*)((byte far*)(ec) + sizeof(ctxtEC_t)))
#define ctxtECLenPar() (ctxtMCB->mcb_size - 1 - ctxtp->ctxt_size)

/* Major IDs of context tags */
typedef enum {
	CTXT_TAG_ERROR = 0
	,CTXT_TAG_FLAG
	,CTXT_TAG_HISTORY
	,CTXT_TAG_DIRSTACK
	,CTXT_TAG_LASTDIR
	,CTXT_TAG_SWAPINFO
	,CTXT_TAG_ARG
	,CTXT_TAG_IVAR
/** pseudo tags */
/* ALIAS must be the last tag */
	,CTXT_TAG_ALIAS
} Context_Tag;
#define CTXT_LAST_TAG CTXT_TAG_ALIAS


/* Minor IDs of CTXT_TAG_SWAPINFO */
enum {
	CTXT_SWAPINFO_PRGNAME = 1
	, CTXT_SWAPINFO_CMDLINE
	, CTXT_SWAPINFO_SHELLNAME
};

/* Minor IDs of CTXT_TAG_FLAG */
enum {
	CTXT_FLAG_ICMDS = 1		/* status of ICMDS == arg of cmd_icmds() */
};

#define ctxtAlias ctxtSegm
#define ctxtHistory ctxtSegm
#define ctxtDirstack ctxtSegm

#define ctxtEnvironment 0		/* the environment is the default
									environment segment */


typedef struct {
	unsigned c_sizemax, c_sizecur;	/* total size of tag */
	unsigned c_nummax,  c_nummin;	/* min/max number assigned */
} ctxt_info_t;

extern ctxt_info_t ctxt_info[];
#define CTXT_join(a,b)	a##b
#define CTXT_INFO_STRUCT(tag)	ctxt_info[(tag)]
#define CTXT_INFO(tag,member)	CTXT_INFO_STRUCT(tag).CTXT_join(c_,member)

#define ctxtFromTag(tag)	ctxtSegm		/* all tags in the same context */
#define ctxtIsInfoTag(tag)	((unsigned)(tag) <= CTXT_LAST_TAG)
#define ctxtCheckInfoTag(tag)	assert(ctxtIsInfoTag(tag))
/* probe if the item tag is of the format: <tag><count>=
	because just <tag>= is not valid as _item_, but may contain
	status information */
#define ctxtProbeItemTag(segm,ofs,tag)							\
	(peekb((segm), (ofs)) == (unsigned char)(Context_Tag)(tag)	\
		 && peekb((segm), (ofs) + 1) != '=')
#define CTXT_ITEMNAME_LENGTH (sizeof(unsigned) * 2 + 2)

void ctxtCreate(void);			/* Create the local context */
void ctxtDestroy(void);			/* Deallocate the local context */
int ctxtAddStatus(const Context_Tag tag);
int ctxtChgSize(const unsigned newsize);
int ctxtPop(const Context_Tag, char ** const);
int ctxtPush(const Context_Tag, const char * const);
int ctxtGet(const int, const Context_Tag, const unsigned, char ** const);
int ctxtGetS(const int, const Context_Tag, const char * const, char ** const);
int ctxtGetItem(const int, const Context_Tag, const char * const, char ** const);
int ctxtSet(const Context_Tag, const unsigned, const char * const);
int ctxtSetS(const Context_Tag, const char * const, const char * const);
char far*ctxtAddress(const Context_Tag tag, const unsigned num);
int ctxtClear(const Context_Tag);
int ctxtView(const Context_Tag, const unsigned);
void ctxtRenumberItems(const Context_Tag);
void ctxtMkItemName(char * const, const Context_Tag, const unsigned num);
char *ctxtMkSymName(const Context_Tag tag, const char * const tail);
int breakVarAssign(ctxt_t context, char * const s, char ** const value);
int chgCtxt(const Context_Tag tag, const char * const, const char * const);
unsigned realNum(const Context_Tag tag, const int num);

/* Purge any internal cached value pointing into the dyn ctxt */
void ctxtPurgeCache(void);
/* Purge one entry if a class is over its limit; Return 0 on failure */
int ctxtSinglePurge(void);

/* Execution Context settings */
typedef enum ExecContext_Tags {
	EC_TAG_INTERACTIVE 			/* I */
	, EC_TAG_BATCH				/* B */
	, EC_TAG_FOR_FIRST			/* F */
	, EC_TAG_FOR_NEXT			/* f */
	, EC_TAG_COMMAND			/* C */
	, EC_TAG_COMMAND_IGNORE_EXIT	/* c */
	, EC_TAG_KEEP_RUNNING		/* e */
	, EC_TAG_TERMINATE			/* E */
} ecTag_t;
#define EC_LAST_TAG EC_TAG_TERMINATE
	/* >= tags that may be considered end of the stack */
#define EC_FINAL_TAGS EC_TAG_KEEP_RUNNING
#define ecNames "IBFfCceE"

#if 0
typedef struct {
	FLAG f_dispPrompt;		/* display prompt on interactive cmdline */
	FLAG f_echo;			/* batch script echo mode */
	FLAG f_swap;			/* do swapping by default */
/** flags below default to zero **/
	FLAG f_canexit;			/* may allowed to exit this shell */
	FLAG f_interactive;		/* set if the current command had been entered
								interactively via command line */
	FLAG f_call;			/* invoke via CALL by default */
	FLAG f_trace;			/* batch script trace mode by default */
	FLAG f_debug;			/* FreeCOM debugging */
	FLAG f_persistentMSGs;	/* keep messages in memory */
	unsigned f_errorlevel;
	unsigned f_base_shiftlevel;
	unsigned f_shiftlevel;	/* argument shift level */
	unsigned f_batchlevel;	/* how many batch nesting levels */
} ctxt_flags_t;
#endif
extern ctxt_flags_t far*ctxtFlagsP;
extern ctxt_flags_t ctxtInitialFlags;
#define ctxtFlags	(*ctxtFlagsP)
#define F(flags)	ctxtFlags.CTXT_join(f_,flags)

extern FLAG swap, echoBatch, dispPrompt, rewindBatchFile, traceMode;
extern FLAG interactive, called;
extern FLAG doExit, doCancel, doQuit;
extern char *gotoLabel;
#define implicitVerbose (interactive? dispPrompt: echoBatch)

extern char* (*ecFunction[])(ctxtEC_t far * const);

	/* Make a new context of specified length and type */
ctxtEC_t far *ecMk(const enum ExecContext_Tags, const unsigned);
	/* Make a silent & hidden c context */
int ecMkc(const char * const str, ...);
	/* Make a silent & hidden C context */
int ecMkHC(const char * const str, ...);
	/* Make a silent C context */
int ecMkSC(const char * const str, ...);
	/* Make a C context, for which the 1st argument is made "Verbatim"
		The "silence" is taken from echoBatch. */
int ecMkV1C(const char * const str, ...);
	/* Make a F context: (param[], params), varname, cmd */
int ecMkF(char ** const, const int, const char * const, const char * const);
	/* Make a f context: ffblk, varname, cmd, prefix */
int ecMkf(const void * const, const char far* const, const char far* const, const char * const);
	/* Make a I context */
int ecMkI(void);
	/* Make a B context: name (pos & lcount default to 0) */
int ecMkB(const char * const name);
	/* Make a C/FD context */
int ecMkFD(const int jft, const int sft);
	/* Return a pointer to the most current B context */
ctxtEC_Batch_t far *ecLastB(void);
	/* Return the next available special internal variable
		in a dynamically allocated buffer. */
char *ecMkIVar(void);
	/* Free the dynamically allocated buffer and, if necessary,
		mark the internal variable as free again */
void ecFreeIVar(char * const ivar);
	/* Create a string that may enclose "str" with a %@VERBATIM() */
char *ecMkVerbatimStr(const char * const str);

	/* Validate the TOS and return a pointer to it */
ctxtEC_t far *ecValidateTOS(void);
	/* Set a new TOS; return 0 if target is below LowestSegm() */
int ecSetTOS(const ctxtEC_t far *);
	/* Remove the topmost context; uses the "size" member only */
void ecPop(void);
	/* Shrink the topmost context by diff bytes */
int ecShrink(unsigned diff);

/* Functions to handle the particular contexts */
/* These functions return:
	0: to pop the topmost context
	cmdlineIgnore: to ignore the return value and proceed
	else: to perform the returned command line (dynamically allocated)
*/
#define cmdlineIgnore ((char*)1)
	/* EC_TAG_INTERACTIVE -- I context */
char *readInteractive(ctxtEC_t far * const);
	/* EC_TAG_BATCH -- B-context */ 
char *readbatchline(ctxtEC_t far * const);
	/* EC_TAG_FOR_FIRST -- F-context */ 
char *readFORfirst(ctxtEC_t far * const);
	/* EC_TAG_FOR_NEXT -- f-context */ 
char *readFORnext(ctxtEC_t far * const);
	/* EC_TAG_COMMAND -- C-context */ 
char *readCommand(ctxtEC_t far * const);
	/* EC_TAG_COMMAND_IGNORE_EXIT -- c-context */ 
char *readCommandIgnoreExit(ctxtEC_t far * const);
	/* EC_TAG_KEEP_RUNNING -- e-context */ 
char *keepMeRunning(ctxtEC_t far * const);
	/* EC_TAG_TERMINATE -- E-context */ 
char *terminateShell(ctxtEC_t far * const);


/*****************
	Encoded strings
		********************/
#define ES_CLEAR_HIGH 1
#define ES_SET_HIGH 2
#define ES_STRING_DELIM	0x1e
#define ES_PAD_BYTE	0x1f

	/* Transform s from internal quoted form into ASCII */
void esDecode(char * const s);
char *esEncode(const char * const s);

#endif
