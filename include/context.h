/* $id$

	Context declarations

*/

#ifndef CONTEXT__H_
#define CONTEXT__H_

#include <portable.h>
#include <mcb.h>

#define MAX_FNAME	89		/* max size of string for one filename */

typedef word ctxt_t;		/* general context type <-> segment */
#define CTXT_INVALID ((ctxt_t)0)

extern ctxt_t ctxtMain;		/* currently there is just one context
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
#include <algndflt.h>
#if sizeof(ctxtCB_t) != 16
#error "The size of type ctxtCB_t must be equal to 16"
#endif
#define ctxtMCB ((struct MCB _seg *)SEG2MCB(ctxtMain))
#define ctxtp ((ctxtCB_t _seg *)ctxtMain)
#define ctxtExecContext		\
	((ctxtEC_t far*)MK_FP(ctxtSegm + ctxtp->ctxt_size, ctxtp->ctxt_eOffs))
extern ctxtCB_t ctxtInitialCB;
extern ctxtEC_t ctxtInitialEC;

/* Major IDs of context tags */
typedef enum {
	CTXT_TAG_ERROR = 0
	,CTXT_TAG_EXEC
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

/* Minor IDs of CTXT_TAG_EXEC */
enum {
	CTXT_EXEC_ICMDS = 1		/* status of ICMDS == arg of cmd_icmds() */
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
int ctxtAddStatus(const Context_Tag tag);
int ctxtChgSize(const unsigned newsize);
int ctxtPop(const Context_Tag, char ** const);
int ctxtPush(const Context_Tag, const char * const);
int ctxtGet(const int, const Context_Tag, const unsigned, char ** const);
int ctxtGetS(const int, const Context_Tag, const char * const, char ** const);
int ctxtSet(const Context_Tag, const unsigned, const char * const);
int ctxtSetS(const Context_Tag, const char * const, const char * const);
char far*ctxtAddress(const Context_Tag tag, const unsigned num);
int ctxtClear(const Context_Tag);
int ctxtView(const Context_Tag, const unsigned);
void ctxtRenumberItems(const Context_Tag);
void ctxtMkItemName(char * const, const Context_Tag, const unsigned num);
int breakVarAssign(ctxt_t context, char * const s, char ** const value);
int chgCtxt(const Context_Tag tag, const char * const, const char * const);
unsigned realNum(const Context_Tag tag, const int num);

/* Purge any internal cached value pointing into the dyn ctxt */
void ctxtPurgeCache(void);
/* Purge one entry if a class is over its limit; Return 0 on failure */
int ctxtSinglePurge(void);

/* Execution Context settings */
enum ExecContext_Tags {
	EC_TAG_INTERACTIVE 			/* I */
	, EC_TAG_BATCH				/* B */
	, EC_TAG_FOR_FIRST			/* F */
	, EC_TAG_FOR_NEXT			/* f */
	, EC_TAG_COMMAND			/* C */
	, EC_TAG_KEEP_RUNNING		/* e */
	, EC_TAG_TERMINATE			/* E */
};
#define EC_LAST_TAG EC_TAG_TERMINATE

typedef struct {
	FLAG f_exit;			/* exit all contexts */
	FLAG f_interactive;		/* set if the current command had been entered
								interactively via command line */
	FLAG f_call;			/* invoke via CALL by default */
	FLAG f_swap;			/* do swapping by default */
	FLAG f_dispPrompt;		/* display prompt on interactive cmdline */
	FLAG f_echo;			/* batch script echo mode */
	FLAG f_trace;			/* batch script trace mode by default */
	unsigned f_base_shiftlevel;
	unsigned f_shiftlevel;	/* argument shift level */
	unsigned f_batchlevel;	/* how many batch nesting levels */
	char * f_goto;			/* label to goto, if a B context is active */
} ctxt_flags_t;
extern ctxt_flags_t ctxtFlags;
#define F(flags)	ctxtFlags.CTXT_join(f_,flags)

/* flags currently in use are declared in command.h */

	/* Make a silent & hidden C context */
int ecMkHC(const char * const str, ...);
	/* Make a silent C context */
int ecMkSC(const char * const str, ...);
	/* Make a C context, for which the 1st argument is made "Verbatim" */
int ecMkV1C(const char * const str, ...);
	/* Make a F context: param, varname, cmd */
int ecMkF(const char * const, const char * const, const char * const);
	/* Make a I context */
int ecMkI(void);
	/* Make a F context: fullname (pos & lcount default to 0) */
int ecMkB(const char * const fullname);
	/* Make a C/FD context */
int ecMkFD(int jft, int sft);
	/* Return a pointer to the most current F context */
ctxtEC_Batch_t far *ecLastF(void);
	/* Return the next available special internal variable
		in a dynamically allocated buffer. */
char *ecMkIVar(void);
	/* Free the dynamically allocated buffer and, if necessary,
		mark the internal variable as free again */
char *ecFreeIVar(char * const ivar);

	/* Remove the topmost context; uses the "size" member only */
void ecPop(void);

/*****************
	Encoded strings
		********************/
#define ES_CLEAR_HIGH 1
#define ES_SET_HIGH 2
#define ES_STRING_DELIM	0x1e
#define ES_PAD_BYTE	0x1f

#endif
