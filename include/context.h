/* $id$

	Context declarations

*/

#ifndef CONTEXT__H_
#define CONTEXT__H_

#include <portable.h>

#define MAX_FNAME	89		/* max size of string for one filename */

typedef word ctxt_t;		/* general context type <-> segment */
#define CTXT_INVALID ((ctxt_t)0)

extern ctxt_t ctxt;			/* currently there is just one context
								-> so-called dynamic context
								-> all data is located in there */
/* Major IDs of context tags */
typedef enum {
	CTXT_TAG_ERROR = 0

		/* with size restriction */
#define CTXT_FIRST_TAG CTXT_TAG_HISTORY
	,CTXT_TAG_HISTORY = 2
	,CTXT_TAG_DIRSTACK
	,CTXT_TAG_BATCH
	,CTXT_TAG_LASTDIR
	,CTXT_TAG_SWAPINFO
} Context_Tag;
#define CTXT_LAST_TAG CTXT_TAG_SWAPINFO

/* Minor IDs of CTXT_TAG_SWAPINFO */
enum {
	CTXT_SWAPINFO_PRGNAME = 1
	, CTXT_SWAPINFO_CMDLINE
	, CTXT_SWAPINFO_SHELLNAME
};

#define ctxtAlias ctxt
#define ctxtHistory ctxt
#define ctxtBatch ctxt
#define ctxtDirstack ctxt

#define ctxtEnvironment 0		/* the environment is the default
									environment segment */

#define ctxtIsValid()	(ctxt != CTXT_INVALID)

typedef struct {
	unsigned c_sizemax, c_sizecur;	/* total size of tag */
	unsigned c_nummax,  c_nummin;	/* min/max number assigned */
} ctxt_info_t;

extern ctxt_info_t ctxt_info[];
#define CTXT_TAG_ALIAS (CTXT_LAST_TAG + 1)
#define CTXT_join(a,b)	a##b
#define CTXT_INFO_STRUCT(tag)	ctxt_info[(tag) - CTXT_FIRST_TAG]
#define CTXT_INFO(tag,member)	CTXT_INFO_STRUCT(tag).CTXT_join(c_,member)

/*
#define alias_maxsize	CTXT_INFO(CTXT_TAG_ALIAS, sizemax)
#define hist_maxsize	CTXT_INFO(CTXT_TAG_HISTORY, sizemax)
#define dstack_maxsize	CTXT_INFO(CTXT_TAG_DIRSTACK, sizemax)
*/

#define ctxtFromTag(tag)	ctxt		/* all tags in the same context */
#define ctxtIsInfoTag(tag) 		\
	((tag) >= CTXT_FIRST_TAG && (tag) <= CTXT_TAG_ALIAS)
#define ctxtCheckInfoTag(tag)	assert(ctxtIsInfoTag(tag))
/* probe if the item tag is of the format: <tag><count>=
	because just <tag>= is not valid as _item_, but may contain
	status information */
#define ctxtProbeItemTag(segm,ofs,tag)							\
	(peekb((segm), (ofs)) == (unsigned char)(Context_Tag)(tag)	\
		 && peekb((segm), (ofs) + 1) != '=')
#define CTXT_ITEMNAME_LENGTH (sizeof(unsigned) * 2 + 2)

void ctxtCreate(void);			/* Create the local context */
void ctxtCreateMemBlock(unsigned length);
int ctxtAddStatus(const Context_Tag tag);
//int ctxtChgSize(const unsigned newsize);
int ctxtPop(const Context_Tag, char ** const);
int ctxtPush(const Context_Tag, const char * const);
int ctxtGet(const int, const Context_Tag, const unsigned, char ** const);
int ctxtSet(const Context_Tag, const unsigned, const char * const);
char far*ctxtAddress(const Context_Tag tag, const unsigned num);
int ctxtClear(const Context_Tag);
int ctxtView(const Context_Tag, const unsigned);
void ctxtRenumberItems(const Context_Tag);
void ctxtMkItemName(char * const, const Context_Tag, const unsigned num);
int breakVarAssign(ctxt_t context, char * const s, char ** const value);
int chgCtxt(const Context_Tag tag, const char * const, const char * const);
unsigned realNum(const Context_Tag tag, const int num);

#endif
