#include <stdio.h>

unsigned char buf[256];

main(void)
{	copy_seg(get_ds(), buf, get_ds(), 0x80, 128);
	if(buf[0] >= 127) {
		printf("Invalid command line length: %d\n", buf[0]);
		exit(10);
	}
	buf[buf[0]+1] = 0;
	printf("%s<<\n", &buf[1]);
}
