/* $id$

	Allocate all memory

	Allocates all DOS memory and deallocates it on exit
	-> joins all unallocated blocks.

	$Log$
	Revision 1.1.2.1  2001/07/07 20:37:17  skaus
	Update #6

*/

#include <stdio.h>
#include <stdlib.h>

#include <suppl.h>

#define BLOCKS 128

word segm[BLOCKS];

main(void)
{	int i;
	word len, probe;

	i = -1;
	while(++i < BLOCKS) {
		for(len = 0, probe = 0x8000; probe; probe >>= 1) {
			word block;
			if((block = DOSalloc(len | probe, 0x80)) != 0) {
				len |= probe;
				DOSfree(block);
			}
		}
		if(!len)
			break;
		if((segm[i] = DOSalloc(len, 0x80)) == 0) {
			puts("Failed to reallocate block");
			break;
		}
		printf("Block: 0x%04x (len=%u paragraphes)\n", segm[i], len);
	}

	while(i--)
		DOSfree(segm[i]);
	return 0;
}
