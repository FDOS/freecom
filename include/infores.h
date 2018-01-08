/* $Id$

	Info resource

	Structure of the resource:
	<tag1>
	<tag2>
	...
	<tagN>

	a <tag> is of the structure:
	Offset	Type	Meaning
	0		1 BYTE	tag type
	1		1 BYTE	tag length
	2..N+2	N BYTE	tag value

	length may 0..255;

	tag type:
	Type	Meaing
	0		end of structure (may be absent)
	1		minimum extra paragaphes required by C host environment
			length := 2
			value  := number of paragraphes that need to be additionally
						allocated by the program minimally
	2		location of _heaplen variable
			length := 4
			value  := offset within file where the (unsigned)_heaplen
						variable is located
	3		Aliases present in FreeCOM
			length := 2
			value  := default size of aliases buffer
	4		History present in FreeCOM
			length := 2
			value  := default size of history buffer
	5		Dirstack present in FreeCOM
			length := 2
			value  := default size of dirstack buffer
	6		Internal command line size
			length := 2
			value  := size of internal command line

*/

#ifndef INFORES__H_
#define INFORES__H_

typedef enum {
	INFO_END = 0
	,INFO_EXTRA_SPACE
	,INFO_POS_HEAPLEN
	,INFO_ALIASES
	,INFO_HISTORY
	,INFO_DIRSTACK
	,INFO_BUFSIZE
} infotags;

typedef struct {
	unsigned char type;
	unsigned char length;
} infotag;

#include <algnbyte.h>
struct EXE_header {
	unsigned short sig, lastFill, numBlocks, numReloc;
	unsigned short header;
	unsigned short extraMin, extraMax;
	unsigned short fSS, fSP, checksum, fIP, fCS;
	unsigned short offReloc, ovrlyNum;
};
#include <algndflt.h>

#endif
