/*	Returns the specified errorlevel
*/
#include <stdio.h>
#include <stdlib.h>

main(int argc, char **argv)
{
	if(argc != 2) {
		fputs("Usage: ERRLVL number\n"
			"Returns the specified number as errorlevel (exit code)\n"
			, stderr);
		return 127;
	}

	return atoi(argv[1]);
}
