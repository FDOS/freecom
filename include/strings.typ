/*
 * STRINGS.TYP - Shared types of STRINGS.DAT implementation
 *		required to ensure FIXSTRS and FREECOM use the same
 *		binary representation
 *
 * 2000/07/09 ska
 * started
 */


typedef unsigned string_count_t;	/* number of strings */
typedef unsigned string_size_t;		/* number of size of strings */
typedef struct 
{
  string_count_t index;
  string_size_t size;
} string_index_t;

#define STRING_NONE ((string_count_t)-1)
#define STRING_RESOURCE_MINOR_ID 3
extern string_count_t strCnt;
extern unsigned msgSegm;
