/* $id$

	Context declarations

*/

#ifndef CONTEXT__H_
#define CONTEXT__H_

#include <portable.h>
#include <mcb.h>
#include "../context.h_c"
#include "../gflags.h_c"

#define MAX_FNAME	89		/* max size of string for one filename */

typedef word ctxt_t;		/* general context type <-> segment */
#define CTXT_INVALID ((ctxt_t)0)

extern volatile ctxt_t ctxtSegm;	/* currently there is just one context
								-> so-called dynamic context
								-> all data is located in there
								it is the location of the subMCB */

#define ctxtMCB ((struct MCB _seg *)SEG2MCB(ctxtSegm))

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
	,CTXT_TAG_STRING
	,CTXT_TAG_EXEC
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
#if 0
#define ctxtProbeItemTag(segm,ofs,tag)							\
	(peekb((segm), (ofs)) == (unsigned char)(Context_Tag)(tag)	\
		 && peekb((segm), (ofs) + 1) != '=')
#else
#define ctxtProbeItemTag(p,tag)							\
	(*(byte*)(p) == (unsigned char)(Context_Tag)(tag)	\
		 && ((byte*)(p))[1] != '=')
#endif
#define CTXT_LENGTH_ITEMNAME (sizeof(unsigned) * 2 + 2)
#define CTXT_LENGTH_ID (sizeof(unsigned) * 4 + 1)

typedef struct {
	unsigned long ec_pos, ec_lnr;
	unsigned ec_idFnam;
} ctxtEC_Batch_t;

void ctxtCreate(void);			/* Create the local context */
void ctxtDestroy(void);			/* Deallocate the local context */
int ctxtAddStatus(const Context_Tag tag);
int ctxtChgSize(const unsigned newsize);
int ctxtPop(const Context_Tag, char ** const);
void ctxtPopTo(Context_Tag tag, unsigned to_del);
int ctxtPush(const Context_Tag, const char * const);
int ctxtRemove(const Context_Tag, const unsigned);
int ctxtGet(const int, const Context_Tag, const unsigned, char ** const);
int ctxtGetS(const int, const Context_Tag, const char * const, char ** const);
int ctxtGetItem(const int, const Context_Tag, const char * const, char ** const);
int ctxtSet(const Context_Tag, const unsigned, const char * const);
int ctxtSetS(const Context_Tag, const char * const, const char * const);
char *ctxtAddress(const Context_Tag tag, const unsigned num);
int ctxtClear(const Context_Tag);
int ctxtView(const Context_Tag, const unsigned);
void ctxtRenumberItems(const Context_Tag);
void ctxtMkItemName(char * const, const Context_Tag, const unsigned num);
char *ctxtMkSymName(const Context_Tag tag, const char * const tail);
int breakVarAssign(ctxt_t context, char * const s, char ** const value);
int chgCtxt(const Context_Tag tag, const char * const, const char * const);
unsigned realNum(const Context_Tag tag, const int num);

/* Purge any internal cached value pointing into the dyn ctxt */
//void ctxtPurgeCache(void);
/* Purge one entry if a class is over its limit; Return 0 on failure */
int ctxtSinglePurge(void);

/* Execution Context settings */
typedef enum ExecContext_Tags {
	EC_TAG_INTERACTIVE = 1		/* I */
	, EC_TAG_SET_STRING			/* S */
	, EC_TAG_BATCH				/* B */
	, EC_TAG_FOR_FIRST			/* F */
	, EC_TAG_FOR_NEXT			/* f */
	, EC_TAG_COMMAND			/* C */
	, EC_TAG_KEEP_RUNNING		/* e */
	, EC_TAG_TERMINATE			/* E */
} ecTag_t;
#define EC_LAST_TAG EC_TAG_TERMINATE
	/* >= tags that may be considered end of the stack */
#define EC_FINAL_TAGS EC_TAG_KEEP_RUNNING
#define ecNames "?ISBFfCeE"
#define EC_TAG_IGNORE (EC_TAG_TERMINATE+1)

#define EC_LENGTH_B  (sizeof(long) * 8 * 2 + CTXT_LENGTH_ID + 3)
#define EC_LENGTH_F  (CTXT_LENGTH_ID * 3 + 2)
#define EC_LENGTH_C  (sizeof(unsigned) * 2 + 3			\
	+ sizeof(FORCE_INTERNAL_COMMAND_STRING))

extern ctxt_flags_t far*ctxtFlagsP;
extern ctxt_flags_t ctxtInitialFlags;
extern ctxt_shared_flags_t ctxtSharedFlags;

extern FLAG lflag_doExit, lflag_doCancel, lflag_doQuit;
extern FLAG lflag_rewindBatchFile;
extern char *lflag_gotoLabel;
#define implicitVerbose (lflag_echo)

extern char* (*ecFctRead[])(char * const);

	/* Make a new context of specified length and type */
//ctxtEC_t far *ecMk(const enum ExecContext_Tags, const unsigned);
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
int ecMkf(const void * const, unsigned, unsigned, const char * const);
	/* Make a S context */
int ecMkS(void);
	/* Make a I context */
int ecMkI(void);
	/* Make an E, or e context */
#define ecMkE()	ecMk(EC_TAG_TERMINATE)
#define ecMke()	ecMk(EC_TAG_KEEP_RUNNING)
int ecMk(const ecTag_t);
	/* Make a B context: name (pos & lcount default to 0) */
int ecMkB(const char * const name);
	/* Make a C/FD context */
int ecMkFD(const int jft, const int sft);
	/* Return a pointer to the most current B context (static buffer) */
ctxtEC_Batch_t *ecLastB(void);
	/* Return the next available special internal variable
		in a dynamically allocated buffer. */
char *ecMkIVar(void);
	/* Free the dynamically allocated buffer and, if necessary,
		mark the internal variable as free again */
void ecFreeIVar(char * const ivar);
	/* Create a string that may enclose "str" with a %@VERBATIM() */
char *ecMkVerbatimStr(const char * const str);
	/* Push an item into the STRING tag */
unsigned ecPushString(const char * const str);
	/* Fetch an item from the STRING tag (return regged string!) */
char *ecString(const unsigned id);
	/* set the TOS of CTXT_TAG_STRING */
void ecSetStringStack(const char * const ctxt);
	/* make a new I or S context */
int ecMkI_S(const byte tag);
	/* Get the arguments of an execution context */
int ecScanArg(const char * const ctxt, const int num, const char * const fmt,...);

#define ctxtP(segm,ofs)		MK_FP((segm), (ofs))


	/** Mode parameters for ecMkvcmd() */
#define EC_CMD_FORCE_INTERNAL 1
#define EC_CMD_SILENT 2			/* disable local ECHO status */
#define EC_CMD_IGNORE_EXIT 4	/* ignore doExit/Cancel/Quit */
#define EC_CMD_NO_TRACE 4		/* disable local trace mode */
#define EC_CMD_NONINTERACTIVE 8	/* disable local interactivity status */


	/* Validate the TOS and return a pointer to it */
//ctxtEC_t far *ecValidateTOS(void);
	/* Set a new TOS; return 0 if target is below LowestSegm() */
//int ecSetTOS(const ctxtEC_t far *);
	/* Remove the topmost context; uses the "size" member only */
void ecPop(void);
	/* Enumerate all values of a stack tag */
int ecEnum(const Context_Tag, const unsigned startID, int (*fct)(unsigned id, char *buf, void * const arg), void * const arg);
	/* Shrink the topmost context by diff bytes */
//int ecShrink(unsigned diff);

/* Functions to handle the particular contexts */
/* These functions return:
	0: to pop the topmost context
	cmdlineIgnore: to ignore the return value and proceed
	else: to perform the returned command line (dynamically allocated)
*/
#define cmdlineIgnore ((char*)1)
	/* EC_TAG_INTERACTIVE -- I context */
char *readInteractive(char * const);
	/* EC_TAG_BATCH -- B-context */ 
char *readBatch(char * const);
	/* EC_TAG_FOR_FIRST -- F-context */ 
char *readFORfirst(char * const);
	/* EC_TAG_FOR_NEXT -- f-context */ 
char *readFORnext(char * const);
	/* EC_TAG_COMMAND -- C-context */ 
char *readCommand(char * const);
	/* EC_TAG_KEEP_RUNNING -- e-context */ 
char *keepMeRunning(char * const);
	/* EC_TAG_TERMINATE -- E-context */ 
char *terminateShell(char * const);
	/* EC_TAG_SET_STRING -- S-context */ 
char *setStringStack(char * const);

/*****************
	Encoded strings
		********************/
#define ES_CLEAR_HIGH 1
#define ES_SET_HIGH 2
#define ES_STRING_DELIM	0x1e
#define ES_PAD_BYTE	0x1f

	/* Transform s from internal quoted form into ASCII */
void esDecode(void * const dst, const char * const src, int maxlen);
char *esEncode(const char * const s);
char *esEncMem(const void * const buf, unsigned len);

#endif
