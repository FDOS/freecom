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
	1		location of _heaplen variable
			length := 4
			value  := offset within file where the (unsigned)_heaplen
						variable is located
	2		Aliases present in FreeCOM
			length := 2
			value  := default size of aliases buffer
	3		History present in FreeCOM
			length := 2
			value  := default size of history buffer
	4		Dirstack present in FreeCOM
			length := 2
			value  := default size of dirstack buffer
	5		Internal command line size
			length := 2
			value  := size of internal command line

*/

#ifndef INFORES__H_
#define INFORES__H_

typedef enum {
	INFO_END = 0
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

#endif
